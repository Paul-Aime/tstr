#ifndef SOMEFUNC_H
#define SOMEFUNC_H

#include "RtAudio.h"

// TODO Documentation

int reverb2(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
           double streamTime, RtAudioStreamStatus status, void *data);

int fft(double *x, double *y, const int m);
int ifft(double *x, double *y, const int m);
int fftr(double *x, double *y, const int m);
int ifftr(double *x, double *y, const int l);
static int checkm(const int m);
int get_nextpow2(int n);
char *getmem(int leng, unsigned size);
double *dgetmem(int leng);
double get_process_time();

#endif