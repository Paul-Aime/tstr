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
  RtAudioCallback ptr_callback = &reverb_f2;

  // Parameters
  unsigned int bufferFrames = 2048;
  unsigned long ir_size = 0; // 0 to have full size

  // Store impulse response in a buffer
  char ir_path[] = "./data/impres";
  MY_TYPE *ir_buffer;
  load_impulse_response(ir_path, &ir_buffer, &ir_size);

  /*
  Configure stream
  */
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
  MY_TYPE *prev_conv_buffer = (MY_TYPE *)malloc(sizeof(MY_TYPE) * (ir_size - 1)); // ? Bon ? 
  if (curr_conv_buffer == NULL || prev_conv_buffer == NULL)
  {
    fputs("Memory error", stderr);
    exit(2);
  }
  unsigned long stats_size = 1024;
  double *stats = (double *)malloc(sizeof(double) * stats_size);
  unsigned long statpos = 0;

  int fft_size = get_nextpow2(ir_size + bufferFrames - 1);
  MY_TYPE *Xr = (MY_TYPE *)malloc(sizeof(MY_TYPE) * fft_size);
  MY_TYPE *Xi = (MY_TYPE *)malloc(sizeof(MY_TYPE) * fft_size);
  MY_TYPE *Yr = (MY_TYPE *)malloc(sizeof(MY_TYPE) * fft_size);
  MY_TYPE *Yi = (MY_TYPE *)malloc(sizeof(MY_TYPE) * fft_size);

  MY_TYPE *Hr = (MY_TYPE *)malloc(sizeof(MY_TYPE) * fft_size);
  MY_TYPE *Hi = (MY_TYPE *)malloc(sizeof(MY_TYPE) * fft_size);
  // memcpy(Hr, ir_buffer, ir_size * sizeof(MY_TYPE));
  // fftr((double *)Hr, (double *)Hi, fft_size);

  struct data_struct data = {ir_buffer, ir_size,
                             curr_conv_buffer, prev_conv_buffer,
                             stats, stats_size, statpos,
                             Xr, Xi, Yr, Yi, Hr, Hi, fft_size};

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
  std::cout << "\nStream latency = " << adac.getStreamLatency() << " frames" << std::endl;

  /*
  Start stream
  */
  try
  {
    adac.startStream();

    char input;
    std::cout << "\nRunning ... press <enter> to quit (buffer frames = " << bufferFrames << ").\n";
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

cleanup:
  if (adac.isStreamOpen())
    adac.closeStream();

  // Print options
  std::cout << "          options.flags: " << options.flags << std::endl;
  std::cout << "options.numberOfBuffers: " << options.numberOfBuffers << std::endl;
  std::cout << "       options.priority: " << options.priority << std::endl;
  std::cout << std::endl;

  // Analyze timing
  double buffer_len = double(bufferFrames) * 1. / (double(fs));
  double *pduration = intervals<>(data.stats, data.statpos);
  double mean_duration = mean(pduration, data.statpos);
  std::cout << "        Number of processed buffers: " << data.statpos << std::endl;
  std::cout << "               Duration of a buffer: " << buffer_len << std::endl;
  std::cout << "Mean processing duration per buffer: " << mean_duration << std::endl;
  // std::cout << "     Processing duration per buffer: " << std::endl;
  // std::cout << std::endl;
  // print_array<>(pduration, data.statpos, 5, 50);

  return 0;
}