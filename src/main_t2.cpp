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
#include "mydef.h"
#include "utils.h"

void usage(void);
void load_impulse_response(char *ir_path, MY_TYPE **ir_buffer, unsigned long *ir_size);
void configure_stream(int argc, char *argv[],
                      unsigned int bufferFrames,
                      unsigned int oDevice, unsigned int iDevice,
                      unsigned int oOffset, unsigned int iOffset,
                      unsigned int *ptr_channels, unsigned int *ptr_fs,
                      RtAudio::StreamParameters *iParams,
                      RtAudio::StreamParameters *oParams,
                      RtAudio *ptr_adac);

int main(int argc, char *argv[])
{
  /*
  Store impulse response in a buffer
  */
  char ir_path[] = "./data/impres";
  MY_TYPE *ir_buffer;
  unsigned long ir_size;

  load_impulse_response(ir_path, &ir_buffer, &ir_size);

  /*
  Configure stream
  */
  unsigned int bufferFrames = 512;
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
  unsigned long statpos = -1;

  struct data_struct data = {ir_buffer, ir_size,
                             curr_conv_buffer, prev_conv_buffer,
                             stats, stats_size, statpos};

  /*
  Open stream
  */
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

  /*
  Start stream
  */
  try
  {
    adac.startStream();

    // Print options
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

  // TODO save stats in a csv file
  double buffer_len = double(bufferFrames) * 1. / (double(fs));
  double *pduration = intervals<>(data.stats, data.statpos);
  std::cout << "Number of processed buffers: " << data.statpos - 1 << std::endl;
  std::cout << "Duration of a buffer: " << buffer_len << std::endl;
  std::cout << "Processing duration for each buffer: " << std::endl;
  print_array<>(pduration, data.statpos - 1, 5, 50);
  std::cout << std::endl;

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

void load_impulse_response(char *ir_path, MY_TYPE **ir_buffer, unsigned long *ir_size)
{
  /*
  Store impulse response in a buffer
  */
  FILE *ptr_ir_file;

  // open file
  ptr_ir_file = fopen(ir_path, "rb");
  if (ptr_ir_file == NULL)
  {
    fputs("File error", stderr);
    exit(1);
  }

  // obtain file size
  fseek(ptr_ir_file, 0, SEEK_END);
  *ir_size = ftell(ptr_ir_file) / sizeof(MY_TYPE);
  fseek(ptr_ir_file, 0, SEEK_SET);

  // allocate memory to contain the whole file
  *ir_buffer = (MY_TYPE *)malloc(sizeof(MY_TYPE) * *ir_size);
  if (ir_buffer == NULL)
  {
    fputs("Memory error", stderr);
    exit(2);
  }

  // copy the file into the buffer
  size_t result = fread(*ir_buffer, sizeof(MY_TYPE), *ir_size, ptr_ir_file);
  if (result != *ir_size)
  {
    fputs("Reading error", stderr);
    exit(3);
  }

  fclose(ptr_ir_file);
}

void configure_stream(int argc, char *argv[],
                      unsigned int bufferFrames,
                      unsigned int oDevice, unsigned int iDevice,
                      unsigned int oOffset, unsigned int iOffset,
                      unsigned int *ptr_channels, unsigned int *ptr_fs,
                      RtAudio::StreamParameters *iParams,
                      RtAudio::StreamParameters *oParams,
                      RtAudio *ptr_adac)
{
  // Minimal command-line checking
  if (argc < 3 || argc > 7)
    usage();

  if (ptr_adac->getDeviceCount() < 1)
  {
    std::cout << "\nNo audio devices found!\n";
    exit(1);
  }

  // Inline parameters
  *ptr_channels = (unsigned int)atoi(argv[1]);
  *ptr_fs = (unsigned int)atoi(argv[2]);
  if (argc > 3)
    iDevice = (unsigned int)atoi(argv[3]);
  if (argc > 4)
    oDevice = (unsigned int)atoi(argv[4]);
  if (argc > 5)
    iOffset = (unsigned int)atoi(argv[5]);
  if (argc > 6)
    oOffset = (unsigned int)atoi(argv[6]);

  // Let RtAudio print messages to stderr.
  ptr_adac->showWarnings(true);

  // Set the same number of channels for both input and output.
  // RtAudio::StreamParameters iParams, oParams;
  iParams->deviceId = iDevice;
  iParams->nChannels = *ptr_channels;
  iParams->firstChannel = iOffset;
  oParams->deviceId = oDevice;
  oParams->nChannels = *ptr_channels;
  oParams->firstChannel = oOffset;

  // Set default devices if needed
  if (iDevice == 0)
    iParams->deviceId = ptr_adac->getDefaultInputDevice();
  if (oDevice == 0)
    oParams->deviceId = ptr_adac->getDefaultOutputDevice();
}
