/******************************************/
/*
  duplex.cpp
  by Gary P. Scavone, 2006-2007.

  This program opens a duplex stream and passes
  input directly through to the output.
*/
/******************************************/

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cstdio>

#include "func.h"
#include "somefunc.h"
#include "RtAudio.h"

void usage(void);

int main(int argc, char *argv[])
{
  /*
  Store impulse response in a buffer
  */
  char ir_path[] = "./data/impres";
  FILE *ptr_ir_file;
  unsigned long ir_size;
  MY_TYPE *ir_buffer;

  // open file
  ptr_ir_file = fopen(ir_path, "rb");
  if (ptr_ir_file == NULL)
  {
    fputs("File error", stderr);
    exit(1);
  }

  // obtain file size
  fseek(ptr_ir_file, 0, SEEK_END);
  ir_size = ftell(ptr_ir_file) / sizeof(MY_TYPE); // TODO check for the division
  fseek(ptr_ir_file, 0, SEEK_SET);

  // allocate memory to contain the whole file
  ir_buffer = (MY_TYPE *)malloc(sizeof(MY_TYPE) * ir_size);
  if (ir_buffer == NULL)
  {
    fputs("Memory error", stderr);
    exit(2);
  }

  // copy the file into the buffer
  size_t result = fread(ir_buffer, sizeof(MY_TYPE), ir_size, ptr_ir_file);
  if (result != ir_size)
  {
    fputs("Reading error", stderr);
    exit(3);
  }

  fclose(ptr_ir_file);

  /*
  Create stream
  */

  // Parameters
  unsigned int bufferFrames = 512;
  unsigned int channels, fs;
  unsigned int oDevice = 5, iDevice = 5;
  unsigned int iOffset = 0, oOffset = 0;

  // Minimal command-line checking
  if (argc < 3 || argc > 7)
    usage();

  RtAudio adac;
  if (adac.getDeviceCount() < 1)
  {
    std::cout << "\nNo audio devices found!\n";
    exit(1);
  }

  // Inline parameters
  channels = (unsigned int)atoi(argv[1]);
  fs = (unsigned int)atoi(argv[2]);
  if (argc > 3)
    iDevice = (unsigned int)atoi(argv[3]);
  if (argc > 4)
    oDevice = (unsigned int)atoi(argv[4]);
  if (argc > 5)
    iOffset = (unsigned int)atoi(argv[5]);
  if (argc > 6)
    oOffset = (unsigned int)atoi(argv[6]);

  // Let RtAudio print messages to stderr.
  adac.showWarnings(true);

  // Set the same number of channels for both input and output.
  RtAudio::StreamParameters iParams, oParams;
  iParams.deviceId = iDevice;
  iParams.nChannels = channels;
  iParams.firstChannel = iOffset;
  oParams.deviceId = oDevice;
  oParams.nChannels = channels;
  oParams.firstChannel = oOffset;

  // Set default devices if needed
  if (iDevice == 0)
    iParams.deviceId = adac.getDefaultInputDevice();
  if (oDevice == 0)
    oParams.deviceId = adac.getDefaultOutputDevice();

  RtAudio::StreamOptions options;
  //options.flags |= RTAUDIO_NONINTERLEAVED;

  // Create structure containing data to be pass as arguments to the callback
  //unsigned long bufferBytes = bufferFrames * channels * sizeof(MY_TYPE);

  MY_TYPE *curr_conv_buffer;
  MY_TYPE *prev_conv_buffer;
  curr_conv_buffer = (MY_TYPE *)malloc(sizeof(MY_TYPE) * (ir_size + bufferFrames - 1));
  prev_conv_buffer = (MY_TYPE *)malloc(sizeof(MY_TYPE) * (ir_size + bufferFrames - 1));
  if (curr_conv_buffer == NULL || prev_conv_buffer == NULL)
  {
    fputs("Memory error", stderr);
    exit(2);
  }

  struct data_struct data = {
      ir_buffer, ir_size, curr_conv_buffer, prev_conv_buffer};

  try
  {
    adac.openStream(&oParams, &iParams, FORMAT, fs, &bufferFrames, &reverb_t2, (void *)&data, &options);
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

  try
  {
    adac.startStream();

    // print options
    std::cout << "options.flags: " << options.flags << std::endl;
    std::cout << "options.numberOfBuffers: " << options.numberOfBuffers << std::endl;
    std::cout << "options.streamName: " << options.streamName << std::endl;
    std::cout << "options.priority: " << options.priority << std::endl;

    char input;
    std::cout << "\nRunning ... press <enter> to quit (buffer frames = " << bufferFrames << ").\n";
    std::cin.get(input);

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

  return 0;
}

void usage(void)
{
  // Error function in case of incorrect command-line
  // argument specifications
  std::cout << "\nuseage: duplex N fs <iDevice> <oDevice> <iChannelOffset> <oChannelOffset>\n";
  std::cout << "    where N = number of channels,\n";
  std::cout << "    fs = the sample rate,\n";
  std::cout << "    iDevice = optional input device to use (default = 0),\n";
  std::cout << "    oDevice = optional output device to use (default = 0),\n";
  std::cout << "    iChannelOffset = an optional input channel offset (default = 0),\n";
  std::cout << "    and oChannelOffset = optional output channel offset (default = 0).\n\n";
  exit(0);
}