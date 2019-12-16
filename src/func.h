#ifndef FUNC_H
#define FUNC_H

#include "RtAudio.h"
#include "mydef.h"

int reverb_t(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
             double streamTime, RtAudioStreamStatus status, void *data);

int reverb_t2(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
              double streamTime, RtAudioStreamStatus status, void *data);

int reverb_f(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
             double streamTime, RtAudioStreamStatus status, void *data);

int reverb_f2(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
             double streamTime, RtAudioStreamStatus status, void *data);

int inout(void *outputBuffer, void *inputBuffer, unsigned int /*nBufferFrames*/,
          double /*streamTime*/, RtAudioStreamStatus status, void *data);

MY_TYPE conv_sample(int idx, MY_TYPE *x, unsigned long x_size, MY_TYPE *h, unsigned long h_size);
int conv(MY_TYPE *x, unsigned long x_size, MY_TYPE *h, unsigned long h_size, MY_TYPE *y, unsigned long y_size);
int fconv(MY_TYPE *x, unsigned long x_size, MY_TYPE *h, unsigned long h_size, MY_TYPE *y, unsigned long y_size);
int fconv2(MY_TYPE *x, unsigned long x_size, MY_TYPE *h, unsigned long h_size, MY_TYPE *y, unsigned long y_size, struct data_struct * data);


#endif