/******************************************/
/*
  duplex.cpp
  by Gary P. Scavone, 2006-2007.

  This program opens a duplex stream and passes
  input directly through to the output.
*/
/******************************************/

#include "RtAudio.h"
#include "AudioFile.h"
#include <iostream>
#include <cstdlib>
#include <cstring>

/*
typedef char MY_TYPE;
#define FORMAT RTAUDIO_SINT8
*/

typedef signed short MY_TYPE;
#define FORMAT RTAUDIO_SINT16

/*
typedef S24 MY_TYPE;
#define FORMAT RTAUDIO_SINT24

typedef signed long MY_TYPE;
#define FORMAT RTAUDIO_SINT32

typedef float MY_TYPE;
#define FORMAT RTAUDIO_FLOAT32

typedef double MY_TYPE;
#define FORMAT RTAUDIO_FLOAT64
*/

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

int inout(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
          double streamTime, RtAudioStreamStatus status, void *data)
{
  // Since the number of input and output channels is equal, we can do
  // a simple buffer copy operation here.
  if (status)
    std::cout << "Stream over/underflow detected." << std::endl;

  unsigned int *bytes = (unsigned int *)data;
  memcpy(outputBuffer, inputBuffer, *bytes);

  // print infos
  /*
  std::cout << "nBufferFrames: " << nBufferFrames << std::endl;
  std::cout << "streamTime: " << streamTime << std::endl;
  */

  return 0;
}

int main(int argc, char *argv[])
{

  std::string imp_resp_folder = "./data/IMreverbs1";
  int imp_resp_idx = 3; // wrt to following strings array
  std::string imp_resp_names[10] = {
      "Five Colums",               // 0
      "Five Colums Long",          // 1
      "French 18th Century Salon", // 2
      "Going Home",                // 3
      "In The Silo Revised",       // 4
      "Narrow Bumpy Space",        // 5
      "Nice Drum Room",            // 6
      "Parking Garage",            // 7
      "Rays",                      // 8
      "Trig Room"                  // 9
  };
  std::string imp_resp_path = imp_resp_folder + "/" + imp_resp_names[imp_resp_idx] + ".wav";

  AudioFile<double> audioFile;
	audioFile.load(imp_resp_path);
	audioFile.printSummary();
  
  // Accessed samples by :
  // audioFile.samples

  unsigned int channels, fs, bufferBytes;
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

  bufferBytes = bufferFrames * channels * sizeof(MY_TYPE);
  try
  {
    adac.openStream(&oParams, &iParams, FORMAT, fs, &bufferFrames, &inout, (void *)&bufferBytes, &options);
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
