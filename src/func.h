#ifndef FUNC_H
#define FUNC_H

#include "RtAudio.h"

// TODO Documentation

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
  unsigned long ir_size; // size M

  MY_TYPE *conv_buffer;
  unsigned long conv_buffer_size; // size M-1
};

struct data_struct
{
  MY_TYPE *ir_buffer;
  unsigned long ir_size; // size M

  MY_TYPE *curr_conv_buffer;
  MY_TYPE *prev_conv_buffer;
};

int reverb_t(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
             double streamTime, RtAudioStreamStatus status, void *data);

int reverb_t2(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
              double streamTime, RtAudioStreamStatus status, void *data);

int reverb_f(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
             double streamTime, RtAudioStreamStatus status, void *data);

int inout(void *outputBuffer, void *inputBuffer, unsigned int /*nBufferFrames*/,
          double /*streamTime*/, RtAudioStreamStatus status, void *data);

MY_TYPE conv_sample(int idx, MY_TYPE *x, unsigned long x_size, MY_TYPE *h, unsigned long h_size);
int conv(MY_TYPE *x, unsigned long x_size, MY_TYPE *h, unsigned long h_size, MY_TYPE *y, unsigned long y_size);

#endif