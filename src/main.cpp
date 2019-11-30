/******************************************/
/*
  duplex.cpp
  by Gary P. Scavone, 2006-2007.

  This program opens a duplex stream and passes
  input directly through to the output.
*/
/******************************************/

#include "RtAudio.h"
#include <iostream>
#include <cstdlib>
#include <cstring>

/*
typedef char MY_TYPE;
#define FORMAT RTAUDIO_SINT8

typedef signed short MY_TYPE;
#define FORMAT RTAUDIO_SINT16

typedef S24 MY_TYPE;
#define FORMAT RTAUDIO_SINT24

typedef signed long MY_TYPE;
#define FORMAT RTAUDIO_SINT32

typedef float MY_TYPE;
#define FORMAT RTAUDIO_FLOAT32
*/

typedef double MY_TYPE;
#define FORMAT RTAUDIO_FLOAT64

struct my_struct
{
  MY_TYPE *ir_buffer;
  unsigned long ir_size;
  unsigned long inout_buffer_size;
  MY_TYPE *conv_buffer; // Size M-1
};

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

int reverb(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
           double streamTime, RtAudioStreamStatus status, void *data)
{
  // Since the number of input and output channels is equal, we can do
  // a simple buffer copy operation here.
  if (status)
    std::cout << "Stream over/underflow detected." << std::endl;

  struct my_struct *pdata = (struct my_struct *)data;

  // TODO
  // for loop 1:M-1
  // output_buffer(n) = conv_samples(n) + conv_buffer(n) 

  // TODO
  // for loop M:L
  // output_buffer(n) = conv_samples(n)

  // TODO
  // for loop L:L+M-1
  // conv_buffer(n) = conv_samples(n) // ! NOT +=

  // * TODO Then no memcpy needed if doinf like said up there, i think
  // memcpy(outputBuffer, inputBuffer, p_impress->size);

  /* // print infos
  std::cout << "nBufferFrames: " << nBufferFrames << std::endl;
  std::cout << "streamTime: " << streamTime << std::endl;
  */

  return 0;
}

int main(int argc, char *argv[])
{
  /*
  Store impulse response in a buffer
  */

  char ir_path[] = "./data/impres";
  FILE *ptr_ir_file;
  long ir_size;
  MY_TYPE *impres_buffer;

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
  impres_buffer = (MY_TYPE *)malloc(sizeof(MY_TYPE) * ir_size);
  if (impres_buffer == NULL)
  {
    fputs("Memory error", stderr);
    exit(2);
  }

  // copy the file into the buffer
  size_t result = fread(impres_buffer, sizeof(MY_TYPE), ir_size, ptr_ir_file);
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
  unsigned int bufferFrames = 512;
  RtAudio::StreamParameters iParams, oParams;
  iParams.deviceId = iDevice;
  iParams.nChannels = channels;
  iParams.firstChannel = iOffset;
  oParams.deviceId = oDevice;
  oParams.nChannels = channels;
  oParams.firstChannel = oOffset;

  if (iDevice == 0)
    iParams.deviceId = adac.getDefaultInputDevice();
  if (oDevice == 0)
    oParams.deviceId = adac.getDefaultOutputDevice();

  RtAudio::StreamOptions options;
  //options.flags |= RTAUDIO_NONINTERLEAVED;

  unsigned long bufferBytes = bufferFrames * channels * sizeof(MY_TYPE);

  // creating structure containing data to be pass as arguments to the callback
  struct my_struct data = {impres_buffer, ir_size, bufferBytes};

  try
  {
    adac.openStream(&oParams, &iParams, FORMAT, fs, &bufferFrames, &reverb, (void *)&data, &options);
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
