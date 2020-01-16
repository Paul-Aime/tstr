#ifndef FUNC_H
#define FUNC_H

#include "RtAudio.h"
#include "mydef.h"

int inout(void *outputBuffer, void *inputBuffer, unsigned int /*nBufferFrames*/,
          double /*streamTime*/, RtAudioStreamStatus status, void *data);

int tconv(MY_TYPE *x, unsigned long x_size,
           MY_TYPE *h, unsigned long h_size,
           MY_TYPE *y, unsigned long y_size);
int fconv(MY_TYPE *x, unsigned long x_size,
           MY_TYPE *y, unsigned long y_size,
           struct data_struct *pdata); // (Hr, Hi) is inside pdata

int reverb_t(void *output_buffer, void *input_buffer, unsigned int n_buffer_frames,
              double streamTime, RtAudioStreamStatus status, void *data);
int reverb_f(void *output_buffer, void *input_buffer, unsigned int n_buffer_frames,
              double streamTime, RtAudioStreamStatus status, void *data);

int overlap_add(struct data_struct *pdata, MY_TYPE *out_buffer, unsigned int n_buffer_frames);
int overlap_add2(struct data_struct *pdata, MY_TYPE *out_buffer, unsigned int n_buffer_frames);



#endif