/******************************************/
/*
  main.cpp
  by Mona D. & Paul A.

  This program opens a duplex stream and
  apply a reverb by convolution.
*/
/******************************************/

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <cmath> // pow
#include <chrono>
#include <thread>
#include <fstream> // write files

#include "func.h"
#include "somefunc.h"
#include "RtAudio.h"
#include "mydef.h"
#include "utils.h"

int main(int argc, char *argv[])
{
  char ir_path[] = "./data/impres";

  // Callback executed between input and ouput buffer
  RtAudioCallback ptr_callback = &reverb_t;

  // Parameters to analyse processing time
  int n_buffers = 100; // Number of buffer per point
  unsigned int buffs_size[] = {128, 256, 512, 1024, 2048, 4096};
  // unsigned int buffs_size[] = {128, 256};
  unsigned long irs_size[] = {80, 100, 120, 160, 190, 220, 320,
                              380, 440, 600, 700, 800, 1300, 1600,
                              1900, 2500, 3000, 3500, 5100, 6100, 7100,
                              10000, 12000, 14000, 20000, 24000, 28000, 41000,
                              49000, 57000, 67000};

  int n_buff_size = sizeof(buffs_size) / sizeof(buffs_size[0]);
  int n_ir_size = sizeof(irs_size) / sizeof(irs_size[0]);

  // 2D array to store durations
  double **proc_duration = (double **)malloc(sizeof(double *) * n_buff_size);
  for (int i = 0; i < n_buff_size; i++)
    proc_duration[i] = (double *)malloc(n_ir_size * sizeof(double));

  // 2D array to store number of processed buffers
  int **n_proc_buffers = (int **)malloc(sizeof(int *) * n_buff_size);
  for (int i = 0; i < n_buff_size; i++)
    n_proc_buffers[i] = (int *)malloc(n_ir_size * sizeof(int));

  // For loop on different (n_buffer_frames, ir_size) points
  for (int m = 0; m < n_buff_size; m++)
  {
    unsigned int n_buffer_frames = buffs_size[m];

    for (int n = 0; n < n_ir_size; n++)
    {
      unsigned long ir_size = irs_size[n];

      /*
      Create structure containing data to be pass as arguments to the callback
      */
      struct data_struct data;

      unsigned long ir_full_size = 0; // To read the whole ir
      load_impulse_response(ir_path, &(data.ir_buffer), &ir_full_size);
      data.ir_size = ir_size;

      // Normalization of impulse response
      MY_TYPE ir_size_sum = 0;
      for (int i = 0; i < ir_size; i++)
      {
        ir_size_sum += data.ir_buffer[i];
      }
      for (int i = 0; i < ir_size; i++)
      {
        data.ir_buffer[i] /= ir_size_sum;
      }

      data.curr_conv_buffer = (MY_TYPE *)malloc(sizeof(MY_TYPE) * (ir_size + n_buffer_frames - 1));
      data.prev_conv_buffer = (MY_TYPE *)malloc(sizeof(MY_TYPE) * (ir_size - 1));

      data.fft_size = get_nextpow2(ir_size + n_buffer_frames - 1);

      data.Xr = (double *)malloc(sizeof(double) * data.fft_size);
      data.Xi = (double *)malloc(sizeof(double) * data.fft_size);
      data.Yr = (double *)malloc(sizeof(double) * data.fft_size);
      data.Yi = (double *)malloc(sizeof(double) * data.fft_size);

      data.Hr = (double *)malloc(sizeof(double) * data.fft_size);
      data.Hi = (double *)malloc(sizeof(double) * data.fft_size);

      // Make sur Hr and Hi are initially full of zeros
      for (int i = 0; i < data.fft_size; i++)
      {
        data.Hr[i] = 0;
        data.Hi[i] = 0;
      }

      memcpy(data.Hr, (double *)data.ir_buffer, data.ir_size * sizeof(double));
      fftr((double *)(data.Hr), (double *)(data.Hi), data.fft_size);

      data.stats_size = 1024;
      data.stats = (double *)malloc(sizeof(double) * data.stats_size);
      data.statpos = -1;

      /*
      Configure stream
      */
      unsigned int channels, fs; // Inputs `argv[1]` and `argv[2]`
      unsigned int oDevice = 0, iDevice = 0;
      unsigned int oOffset = 0, iOffset = 0;

      RtAudio::StreamParameters iParams, oParams;
      RtAudio adac;
      configure_stream(argc, argv, n_buffer_frames,
                       oDevice, iDevice, oOffset, iOffset,
                       &channels, &fs, &iParams, &oParams, &adac);

      RtAudio::StreamOptions options;
      //options.flags |= RTAUDIO_NONINTERLEAVED;

      /*
      Open stream
      */
      try
      {
        adac.openStream(&oParams, &iParams, FORMAT, fs, &n_buffer_frames, ptr_callback, (void *)&data, &options);
      }
      catch (RtAudioError &e)
      {
        std::cout << '\n'
                  << e.getMessage() << '\n'
                  << std::endl;
        exit(1);
      }

      /*
      Start stream
      */
      try
      {
        adac.startStream();

        // Get buffer len info in seconds
        double buffer_len = n_buffer_frames * (1. / fs);

        // Print parameters
        std::cout << "\n          ir_size: " << ir_size << std::endl;
        std::cout << "  n_buffer_frames: " << n_buffer_frames << std::endl;

        // Sleep for a while
        int sleep_for_ms = (int)(buffer_len * 1e3 * n_buffers);
        std::chrono::milliseconds timespan(sleep_for_ms);
        std::this_thread::sleep_for(timespan);

        // Stop the stream.
        adac.stopStream();
      }
      catch (RtAudioError &e)
      {
        std::cout << '\n'
                  << e.getMessage() << '\n'
                  << std::endl;
        goto cleanup;
      }

    cleanup: // TODO try to remove
      if (adac.isStreamOpen())
        adac.closeStream();

      // Print options
      std::cout << "          options.flags: " << options.flags << std::endl;
      std::cout << "options.numberOfBuffers: " << options.numberOfBuffers << std::endl;
      std::cout << "       options.priority: " << options.priority << std::endl;
      std::cout << std::endl;

      // Print parameters
      std::cout << "          ir_size: " << ir_size << std::endl;
      std::cout << "  n_buffer_frames: " << n_buffer_frames << std::endl;
      std::cout << std::endl;

      // Analyze timing
      double buffer_len = double(n_buffer_frames) * 1. / (double(fs));
      unsigned long n_proc_buffs = data.statpos + 1;
      double *pduration = intervals<>(data.stats, n_proc_buffs);
      double mean_duration = mean(pduration, n_proc_buffs);
      std::cout << "        Number of processed buffers: " << n_proc_buffs << std::endl;
      std::cout << "               Duration of a buffer: " << buffer_len << std::endl;
      std::cout << "Mean processing duration per buffer: " << mean_duration << std::endl;
      // std::cout << "     Processing duration per buffer: " << std::endl;
      // std::cout << std::endl;
      // print_array<>(pduration, n_proc_buffs, 5, 50);

      proc_duration[m][n] = mean_duration;
      n_proc_buffers[m][n] = (int)n_proc_buffs;

      // Free memory
      free(data.curr_conv_buffer);
      data.curr_conv_buffer = NULL;
      free(data.prev_conv_buffer);
      data.prev_conv_buffer = NULL;
      free(data.Xr);
      data.Xr = NULL;
      free(data.Xi);
      data.Xi = NULL;
      free(data.Yr);
      data.Yr = NULL;
      free(data.Yi);
      data.Yi = NULL;
      free(data.Hr);
      data.Hr = NULL;
      free(data.Hi);
      data.Hi = NULL;
      // free(data.stats);
      // data.stats = NULL;
    }
  }

  std::cout << "\n\n\n"
            << std::endl;

  // Print processing duration array
  for (int i = 0; i < n_buff_size; i++)
  {
    print_array<>(proc_duration[i], n_ir_size, 5, n_ir_size);
    std::cout << std::endl;
  }

  std::cout << "\n\n\n"
            << std::endl;

  // --- Save stats as csv files
  std::ofstream csv_proc_duration;
  std::ofstream csv_n_proc_buffers;
  csv_proc_duration.open("./out/proc_duration_tmp.csv");
  csv_n_proc_buffers.open("./out/n_proc_buffers_tmp.csv");
  for (int m = 0; m < n_buff_size; m++)
  {
    for (int n = 0; n < n_ir_size; n++)
    {
      // data
      csv_proc_duration << proc_duration[m][n] << ',';
      csv_n_proc_buffers << n_proc_buffers[m][n] << ',';
    }
    csv_proc_duration << '\n';
    csv_n_proc_buffers << '\n';
  }
  csv_proc_duration.close();
  csv_n_proc_buffers.close();
  // Header cols
  std::ofstream csv_cols_irs_size;
  csv_cols_irs_size.open("./out/cols_irs_size_tmp.csv");
  for (int i = 0; i < n_ir_size; i++)
  {
    csv_cols_irs_size << irs_size[i] << ',';
  }
  csv_cols_irs_size.close();
  // Header rows
  std::ofstream csv_rows_buffs_size;
  csv_rows_buffs_size.open("./out/rows_buffs_size_tmp.csv");
  for (int i = 0; i < n_buff_size; i++)
  {
    csv_rows_buffs_size << buffs_size[i] << ',';
  }
  csv_rows_buffs_size.close();

  // TODO remove last comma from csv files

  return 0;
}