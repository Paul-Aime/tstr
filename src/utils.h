#ifndef UTILS_H
#define UTILS_H

#include "RtAudio.h"
#include "mydef.h"

void load_impulse_response(char *ir_path, MY_TYPE **ir_buffer, unsigned long *ir_size);
void configure_stream(int argc, char *argv[],
                      unsigned int bufferFrames,
                      unsigned int oDevice, unsigned int iDevice,
                      unsigned int oOffset, unsigned int iOffset,
                      unsigned int *ptr_channels, unsigned int *ptr_fs,
                      RtAudio::StreamParameters *iParams,
                      RtAudio::StreamParameters *oParams,
                      RtAudio *ptr_adac);
void usage(void);

template <class T>
T *intervals(T *arr, int arr_size);

template <class T>
void print_array(T *arr, unsigned long size, int n_elt_per_row = 5, int max_elt = 20);

template <class T>
T mean(T *arr, unsigned long arr_size);

#include "utils.tcc"

#endif