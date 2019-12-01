#include <iostream>
#include "func.h"
#include "RtAudio.h"

int reverb(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
           double streamTime, RtAudioStreamStatus status, void *data)
{
  // Since the number of input and output channels is equal, we can do
  // a simple buffer copy operation here.
  if (status)
    std::cout << "Stream over/underflow detected." << std::endl;

  struct my_struct *pdata = (struct my_struct *)data;

  // cast
  MY_TYPE *outBuffer = (MY_TYPE *)outputBuffer;
  MY_TYPE *inBuffer = (MY_TYPE *)inputBuffer;

  unsigned long L = (unsigned long)nBufferFrames;
  unsigned long M = (unsigned long)pdata->ir_size;
  // unsigned long M_ = (unsigned long) pdata->conv_buffer_size; // = M-1

  // TODO Compute current buffer convolution
  // Overlapping part with previous buffer
  // for loop 1:min(L, M-1)
  // output_buffer(n) = conv_samples(n) + conv_buffer(n)
  for (int i = 0; i < std::min(L, M - 1); i++)
  {
    outBuffer[i] = conv_sample(i, inBuffer, L, pdata->conv_buffer, M);
    outBuffer[i] += pdata->conv_buffer[i];
  }

  // Non overlapping part (may be null)
  // for loop min(L, M-1): L
  // output_buffer(n) = conv_samples(n)
  if (M - 1 < L)
  {
    for (int i = M - 1; i < L; i++)
    {
      outBuffer[i] = conv_sample(i, inBuffer, L, pdata->conv_buffer, M);
    }
  }

  // TODO Save next buffer overlapping part
  // if M-1 > L // le buffer_conv dépasse
  // for loop n = 1: M-1 -L
  // conv_buffer(n) = conv_buffer(L+n)
  if (M - 1 > L)
  {
    for (int i = 0; i < M - 1 - L; i++)
    {
      pdata->conv_buffer[i] = pdata->conv_buffer[L + i];
    }
  }

  // for loop n = 1:M-1
  // conv_buffer(n) += conv_samples(L+n)
  for (int i = 0; i < M - 1; i++)
  {
    pdata->conv_buffer[i] += conv_sample(L + i, inBuffer, L, pdata->conv_buffer, M);
  }

  // * Then no memcpy needed if doinf like said up there, i think
  // memcpy(outputBuffer, inputBuffer, p_impress->size);

  /* // print infos
  std::cout << "nBufferFrames: " << nBufferFrames << std::endl;
  std::cout << "streamTime: " << streamTime << std::endl;
  */

  return 0;
}

MY_TYPE conv_sample(int idx, MY_TYPE *x, unsigned long x_size, MY_TYPE *h, unsigned long h_size)
{
  MY_TYPE c = 0;
  int kmin;
  int kmax;
  if (idx > h_size)
  {
    kmin = idx - h_size;
  }
  else
  {
    kmin = 0;
  }
  if (idx <= x_size)
  {
    kmax = idx; // Todo vérifier si -1
  }
  else
  {
    kmax = x_size; // Todo vérifier si -1
  }

  for (int k = kmin; k < kmax; k++)
  {
    c += x[k] * h[idx - k];
  }

  return c;
}