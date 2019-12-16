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
  // Callback executed between input and ouput buffer
  RtAudioCallback ptr_callback = &reverb_f;

  // Parameters to analyse processing time
  int n_buff_size_min = 6; // linspace on input buffer size axis
  int n_buff_size_max = 10;
  int n_buff_size = n_buff_size_max - n_buff_size_min + 1;
  int n_ir_size_min = 6; // linspace on impulse response buffer size axis
  int n_ir_size_max = 12;
  int n_ir_size = n_ir_size_max - n_ir_size_min + 1;
  int n_buffers = 1000; // Number of buffer per point

  // 2D array to store durations
  double **proc_duration = (double **)malloc(sizeof(double *) * n_buff_size);
  for (int i = 0; i < n_buff_size; i++)
    proc_duration[i] = (double *)malloc(n_ir_size * sizeof(double));

  // 2D array to store number of processed buffers
  int **n_proc_buffers = (int **)malloc(sizeof(int *) * n_buff_size);
  for (int i = 0; i < n_buff_size; i++)
    n_proc_buffers[i] = (int *)malloc(n_ir_size * sizeof(int));

  // 1D array to store buffers' size
  unsigned int *buffs_size = (unsigned int *)malloc(sizeof(unsigned int) * n_buff_size);

  // 1D array to store impulse response size
  unsigned long *irs_size = (unsigned long *)malloc(sizeof(unsigned long) * n_ir_size);

  // For loop on different (bufferFrames, ir_size) points
  for (int m = 0; m < n_buff_size; m++)
  {
    unsigned int bufferFrames = pow(2., (double)(n_buff_size_min + m));
    buffs_size[m] = bufferFrames;

    for (int n = 0; n < n_ir_size; n++)
    {
      unsigned long ir_size = pow(2., (double)(n_ir_size_min + n));
      irs_size[n] = ir_size;

      std::cout << "\n\n\n####################" << std::endl;
      std::cout << n_buff_size_min + m << ", " << n_ir_size_min + n << std::endl;
      std::cout << bufferFrames << ", " << ir_size << std::endl;
      std::cout << "\n\n\n####################" << std::endl;

      /*
      Store impulse response in a buffer
      */
      char ir_path[] = "./data/impres";
      MY_TYPE *ir_buffer;
      //unsigned long ir_size = 512; // 4096

      load_impulse_response(ir_path, &ir_buffer, &ir_size);

      /*
      Configure stream
      */
      // unsigned int bufferFrames = 512; // 512
      unsigned int channels, fs; // Inputs `argv[1]` and `argv[2]`
      unsigned int oDevice = 0, iDevice = 0;
      unsigned int oOffset = 0, iOffset = 0;

      RtAudio::StreamParameters iParams, oParams;
      RtAudio adac;
      configure_stream(argc, argv, bufferFrames,
                       oDevice, iDevice, oOffset, iOffset,
                       &channels, &fs, &iParams, &oParams, &adac);

      RtAudio::StreamOptions options;
      //options.flags |= RTAUDIO_NONINTERLEAVED;

      /*
      Create structure containing data to be pass as arguments to the callback
      */
      MY_TYPE *curr_conv_buffer = (MY_TYPE *)malloc(sizeof(MY_TYPE) * (ir_size + bufferFrames - 1));
      MY_TYPE *prev_conv_buffer = (MY_TYPE *)malloc(sizeof(MY_TYPE) * (ir_size + bufferFrames - 1));
      if (curr_conv_buffer == NULL || prev_conv_buffer == NULL)
      {
        fputs("Memory error", stderr);
        exit(2);
      }
      unsigned long stats_size = 1024;
      double *stats = (double *)malloc(sizeof(double) * stats_size);
      unsigned long statpos = 0;

      struct data_struct data = {ir_buffer, ir_size,
                                 curr_conv_buffer, prev_conv_buffer,
                                 stats, stats_size, statpos};

      /*
      Open stream
      */
      try
      {
        adac.openStream(&oParams, &iParams, FORMAT, fs, &bufferFrames, ptr_callback, (void *)&data, &options);
      }
      catch (RtAudioError &e)
      {
        std::cout << '\n'
                  << e.getMessage() << '\n'
                  << std::endl;
        exit(1);
      }

      // Test RtAudio functionality for reporting latency.
      // std::cout << "\nStream latency = " << adac.getStreamLatency() << " frames" << std::endl;

      /*
      Start stream
      */
      try
      {
        adac.startStream();

        // char input;
        // std::cout << "\nRunning ... press <enter> to quit (buffer frames = " << bufferFrames << ").\n";
        // std::cin.get(input);

        // Get buffer len info in seconds
        double buffer_len = double(bufferFrames) * 1. / (double(fs));

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

    cleanup:
      if (adac.isStreamOpen())
        adac.closeStream();

      // Print options
      // std::cout << "          options.flags: " << options.flags << std::endl;
      // std::cout << "options.numberOfBuffers: " << options.numberOfBuffers << std::endl;
      // std::cout << "       options.priority: " << options.priority << std::endl;
      // std::cout << std::endl;

      // Analyze timing // TODO
      // TODO add inside callback timing
      double buffer_len = double(bufferFrames) * 1. / (double(fs)); // TODO may be already declared
      double *pduration = intervals<>(data.stats, data.statpos);
      double mean_duration = mean(pduration, data.statpos);
      std::cout << "        Number of processed buffers: " << data.statpos << std::endl;
      std::cout << "               Duration of a buffer: " << buffer_len << std::endl;
      std::cout << "Mean processing duration per buffer: " << mean_duration << std::endl;
      std::cout << "     Processing duration per buffer: " << std::endl;
      std::cout << std::endl;
      print_array<>(pduration, data.statpos, 5, 50);

      proc_duration[m][n] = mean_duration;
      n_proc_buffers[m][n] = data.statpos;
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
  csv_rows_buffs_size.close();;

  // TODO remove last comma from csv files

  return 0;
}