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

  // Parameters
  char ir_path[] = "./data/impres";
  unsigned int n_buffer_frames = 2048;
  unsigned long ir_size = 60000; // 0 for full size

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

  data.stats_size = 4096;
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

  // Test RtAudio functionality for reporting latency.
  std::cout << "\nStream latency = " << adac.getStreamLatency() << " frames" << std::endl;

  /*
  Start stream
  */
  try
  {
    adac.startStream();

    // Print parameters
    std::cout << "\n          ir_size: " << ir_size << std::endl;
    std::cout << "  n_buffer_frames: " << n_buffer_frames << std::endl;

    char input;
    std::cout << "\nRunning ... press <enter> to quit." << std::endl;
    std::cin.get(input);

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
  unsigned long n_proc_buffers = data.statpos + 1;
  double *pduration = intervals<>(data.stats, n_proc_buffers);
  double mean_duration = mean(pduration, n_proc_buffers);
  std::cout << "        Number of processed buffers: " << n_proc_buffers << std::endl;
  std::cout << "               Duration of a buffer: " << buffer_len << std::endl;
  std::cout << "Mean processing duration per buffer: " << mean_duration << std::endl;
  // std::cout << "     Processing duration per buffer: " << std::endl;
  // std::cout << std::endl;
  // print_array<>(pduration, n_proc_buffers, 5, 50);

  return 0;
}