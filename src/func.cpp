#include <iostream>
#include <cstring> // memcpy
#include <cstdlib> // malloc, calloc, realloc, free, exit
#include <cstdio>  // fputs, stderr

#include "utils.h"
#include "func.h"
#include "somefunc.h"
#include "RtAudio.h"

int reverb_t(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
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

  // Compute current buffer convolution
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

int reverb_t2(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
              double streamTime, RtAudioStreamStatus status, void *data)
{

  // Check for over/under run
  if (status)
  {
    std::cout << "Stream over/underflow detected." << std::endl;
  }

  // Cast
  struct data_struct *pdata = (struct data_struct *)data;
  MY_TYPE *outBuffer = (MY_TYPE *)outputBuffer;
  MY_TYPE *inBuffer = (MY_TYPE *)inputBuffer;

  // Short variables
  unsigned long L = (unsigned long)nBufferFrames;
  unsigned long M = (unsigned long)pdata->ir_size;

  // Compute current buffer convolution
  if (conv(inBuffer, L, pdata->ir_buffer, M, pdata->curr_conv_buffer, L + M - 1))
  {
    exit(1);
  }

  // Fill outBuffer
  for (int i = 0; i < L; i++)
  {
    outBuffer[i] = pdata->curr_conv_buffer[i];
    if (i < M - 1)
    {
      outBuffer[i] += pdata->prev_conv_buffer[L + i];
    }
  }

  // Fill futur prev_conv_buffer (overlapping part)
  for (int i = 0; i < L + M - 1; i++)
  {
    pdata->prev_conv_buffer[i] = pdata->curr_conv_buffer[i];
  }

  // Fill stats
  if (pdata->statpos >= pdata->stats_size) // The pdata->stats array is not tall enough
  {
    // Re-allocate memory for new array
    pdata->stats = (double *)realloc(pdata->stats, 2 * pdata->stats_size * sizeof(double));
    if (pdata->stats == NULL)
    {
      fputs("Memory re-allocation error", stderr);
      exit(2);
    }
    // Update pdata->stats_size
    pdata->stats_size = 2 * pdata->stats_size;
  }
  pdata->stats[pdata->statpos] = get_process_time();
  pdata->statpos++;
}

int reverb_f(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
             double streamTime, RtAudioStreamStatus status, void *data)
{
  // Check for over/under run
  if (status)
  {
    std::cout << "Stream over/underflow detected." << std::endl;
  }

  // Cast
  struct data_struct *pdata = (struct data_struct *)data;
  MY_TYPE *outBuffer = (MY_TYPE *)outputBuffer;
  MY_TYPE *inBuffer = (MY_TYPE *)inputBuffer;

  // Short variables
  unsigned long L = (unsigned long)nBufferFrames;
  unsigned long M = (unsigned long)pdata->ir_size;

  // Compute current buffer convolution
  if (fconv(inBuffer, L, pdata->ir_buffer, M, pdata->curr_conv_buffer, L + M - 1))
  {
    exit(1);
  }

  // Fill outBuffer
  for (int i = 0; i < L; i++)
  {
    outBuffer[i] = pdata->curr_conv_buffer[i];
    if (i < M - 1)
    {
      outBuffer[i] += pdata->prev_conv_buffer[L + i];
    }
  }

  // Fill futur prev_conv_buffer (overlapping part)
  for (int i = 0; i < L + M - 1; i++)
  {
    pdata->prev_conv_buffer[i] = pdata->curr_conv_buffer[i];
  }

  // Fill stats
  if (++pdata->statpos >= pdata->stats_size) // The pdata->stats array is not tall enough
  {
    // Re-allocate memory for new array
    pdata->stats = (double *)realloc(pdata->stats, 2 * pdata->stats_size * sizeof(double));
    if (pdata->stats == NULL)
    {
      fputs("Memory re-allocation error", stderr);
      exit(2);
    }
    // Update pdata->stats_size
    pdata->stats_size = 2 * pdata->stats_size;
  }
  pdata->stats[pdata->statpos] = get_process_time();
}

int inout(void *outputBuffer, void *inputBuffer, unsigned int /*nBufferFrames*/,
          double /*streamTime*/, RtAudioStreamStatus status, void *data)
{
  // Since the number of input and output channels is equal, we can do
  // a simple buffer copy operation here.
  if (status)
    std::cout << "Stream over/underflow detected." << std::endl;

  unsigned int *bytes = (unsigned int *)data;
  memcpy(outputBuffer, inputBuffer, *bytes);
  return 0;
}

int conv(MY_TYPE *x, unsigned long x_size, MY_TYPE *h, unsigned long h_size, MY_TYPE *y, unsigned long y_size)
{

  // Check for size
  if (y_size != x_size + h_size - 1)
  {
    return 1;
  }

  // Declare variables for current conv boundaries
  int kmin, kmax;

  // Do the actual convolution
  for (int n = 0; n < x_size + h_size - 1; n++)
  {
    // Current conv boundaries
    n < x_size - 1 ? kmax = n : kmax = x_size - 1;
    n >= h_size ? kmin = n - (h_size - 1) : kmin = 0;

    y[n] = 0;
    for (int k = kmin; k <= kmax; k++)
    {
      y[n] += x[k] * h[n - k];
    }
  }

  return 0;
}

int fconv(MY_TYPE *x, unsigned long x_size, MY_TYPE *h, unsigned long h_size, MY_TYPE *y, unsigned long y_size)
{
  // Check for size
  if (y_size != x_size + h_size - 1)
  {
    return 1;
  }

  const int n_fft = (const int)get_nextpow2((unsigned long)y_size);

  MY_TYPE *Xr = (MY_TYPE *)malloc(sizeof(MY_TYPE) * n_fft);
  memcpy(Xr, x, x_size * sizeof(MY_TYPE)); // TODO vérifier si mettre 0 sur la fin
  MY_TYPE *Xi = (MY_TYPE *)malloc(sizeof(MY_TYPE) * n_fft);

  MY_TYPE *Hr = (MY_TYPE *)malloc(sizeof(MY_TYPE) * n_fft);
  memcpy(Hr, h, h_size * sizeof(MY_TYPE)); // TODO vérifier si mettre 0 sur la fin
  MY_TYPE *Hi = (MY_TYPE *)malloc(sizeof(MY_TYPE) * n_fft);

  fftr((double *)Xr, (double *)Xi, n_fft);
  fftr((double *)Hr, (double *)Hi, n_fft);

  // Multiply
  MY_TYPE *Yr = (MY_TYPE *)malloc(sizeof(MY_TYPE) * n_fft);
  MY_TYPE *Yi = (MY_TYPE *)malloc(sizeof(MY_TYPE) * n_fft);
  for (int i = 0; i < n_fft; i++)
  {
    Yr[i] = Xr[i] * Hr[i] - Xi[i] * Hi[i];
    Yi[i] = Xr[i] * Hi[i] + Xi[i] * Hr[i];
  }

  // IFFT
  ifft(Yr, Yi, n_fft);

  // Keep only y_size points
  memcpy(y, Yr, y_size * sizeof(MY_TYPE));

  // Free space
  free(Xr);
  free(Xi);
  free(Hr);
  free(Hi);
  free(Yr);
  free(Yi);

  // TODO use buffer instead of allocating and free
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