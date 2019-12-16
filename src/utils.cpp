#include "utils.h"
#include "mydef.h"
#include "RtAudio.h"

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

  if (*ir_size == 0) // obtain file size
  {
    fseek(ptr_ir_file, 0, SEEK_END);
    *ir_size = ftell(ptr_ir_file) / sizeof(MY_TYPE);
  }

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