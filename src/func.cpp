#include <iostream>
#include <cstring> // memcpy
#include <cstdlib> // malloc, calloc, realloc, free, exit
#include <cstdio>  // fputs, stderr

#include "utils.h"
#include "func.h"
#include "somefunc.h"
#include "RtAudio.h"

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

int tconv(MY_TYPE *x, unsigned long x_size, MY_TYPE *h, unsigned long h_size, MY_TYPE *y, unsigned long y_size)
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

int fconv(MY_TYPE *x, unsigned long x_size, MY_TYPE *y, unsigned long y_size, struct data_struct *pdata)
{
  // Check for size
  if (y_size != x_size + pdata->ir_size - 1)
  {
    return 1;
  }

  memcpy(pdata->Xr, (double *)x, x_size * sizeof(double));

  // FFT
  const int n_fft = (const int)pdata->fft_size;
  fftr(pdata->Xr, pdata->Xi, n_fft);

  // Multiply
  for (int i = 0; i < n_fft; i++)
  {
    pdata->Yr[i] = pdata->Xr[i] * pdata->Hr[i] - pdata->Xi[i] * pdata->Hi[i];
    pdata->Yi[i] = pdata->Xr[i] * pdata->Hi[i] + pdata->Xi[i] * pdata->Hr[i];
  }

  // IFFT
  ifft(pdata->Yr, pdata->Yi, n_fft);

  // Keep only y_size points
  memcpy(y, (MY_TYPE *)pdata->Yr, y_size * sizeof(MY_TYPE));

  return 0;
}

int reverb_t(void *output_buffer, void *input_buffer, unsigned int n_buffer_frames,
              double streamTime, RtAudioStreamStatus status, void *data)
{
  // Check for over/under run
  // if (status)
  // {
  //   std::cout << "Stream over/underflow detected." << std::endl;
  // }

  // Cast
  struct data_struct *pdata = (struct data_struct *)data;
  MY_TYPE *out_buffer = (MY_TYPE *)output_buffer;
  MY_TYPE *in_buffer = (MY_TYPE *)input_buffer;

  // Short variables
  unsigned long L = (unsigned long)n_buffer_frames;
  unsigned long M = (unsigned long)pdata->ir_size;

  // Compute current buffer convolution
  tconv(in_buffer, L, pdata->ir_buffer, M, pdata->curr_conv_buffer, L + M - 1);

  // Achieve overlap-add (also filling the output audio buffer)
  overlap_add(pdata, out_buffer, n_buffer_frames);

  // Fill stats
  fill_stats(pdata);

  return 0;
}

int reverb_f(void *output_buffer, void *input_buffer, unsigned int n_buffer_frames,
              double streamTime, RtAudioStreamStatus status, void *data)
{
  // Check for over/under run
  // if (status)
  // {
  //   std::cout << "Stream over/underflow detected." << std::endl;
  // }

  // Cast
  struct data_struct *pdata = (struct data_struct *)data;
  MY_TYPE *out_buffer = (MY_TYPE *)output_buffer;
  MY_TYPE *in_buffer = (MY_TYPE *)input_buffer;

  // Short variables
  unsigned long L = (unsigned long)n_buffer_frames;
  unsigned long M = (unsigned long)pdata->ir_size;

  // Compute current buffer convolution
  fconv(in_buffer, L, pdata->curr_conv_buffer, L + M - 1, pdata);

  // Achieve overlap-add (also filling the output audio buffer)
  overlap_add(pdata, out_buffer, n_buffer_frames);

  // Fill stats
  fill_stats(pdata);

  return 0;
}

/**
 * Achieve overlap-add.
 * 
 * This add the previous overlapping part to `pdata->curr_conv_buffer`, 
 * fill `out_buffer` with the output for the current window,
 * and register the next overlapping part to `pdata->prev_conv_buffer`.
 * 
 * pdata->curr_conv_buffer must already be filled with the result of the
 * convolution between current input audio buffer and the impulse
 * response.
 * pdata->prev_conv_buffer must already be filled with the overlapping
 * result of the convolution between previous output buffer and the
 * impulse response.
 * 
 * @param pdata Pointer to a `data_struct` strcuture, defined in mydef.h
 * @param out_buffer Pointer to the output audio buffer to be filled
 * with the result
 * @param n_buffer_frames Size of the output audio buffer
 */
int overlap_add(struct data_struct *pdata, MY_TYPE *out_buffer, unsigned int n_buffer_frames)
{

  // Short variables // TODO remove unsigned long, only unsigned int for size variables
  unsigned long L = (unsigned long)n_buffer_frames;
  unsigned long M = (unsigned long)pdata->ir_size;

  // Separate cases to be more efficient
  if (M - 1 > L)
  {
    for (int i = 0; i < L; i++)
    {
      // Add the first L points of the previous overlapping part to pdata->curr_conv_buffer
      pdata->curr_conv_buffer[i] += pdata->prev_conv_buffer[i];

      // Fill the output audio buffer
      out_buffer[i] = pdata->curr_conv_buffer[i];
    }
    for (int i = L; i < M - 1; i++)
    {
      // Add the remaining points of the previous overlapping part to pdata->curr_conv_buffer
      pdata->curr_conv_buffer[i] += pdata->prev_conv_buffer[i];

      // Fill first points of the overlapping part with next buffer
      pdata->prev_conv_buffer[i - L] = pdata->curr_conv_buffer[i];
    }
    for (int i = M - 1; i < L + M - 1; i++)
    {
      // Fill remaining points of the overlapping part with next buffer
      pdata->prev_conv_buffer[i - L] = pdata->curr_conv_buffer[i];
    }
  }
  else // case M-1 <= L
  {
    for (int i = 0; i < M - 1; i++)
    {
      // Add the M-1 points of the previous overlapping part to pdata->curr_conv_buffer
      pdata->curr_conv_buffer[i] += pdata->prev_conv_buffer[i];

      // Fill the output audio buffer
      out_buffer[i] = pdata->curr_conv_buffer[i];
    }
    for (int i = M - 1; i < L; i++) // nothing if M-1 == L
    {
      // Fill the remaining points of the output audio buffer
      out_buffer[i] = pdata->curr_conv_buffer[i];
    }
    for (int i = L; i < L + M - 1; i++)
    {
      // Fill the M-1 points of the overlapping part with next buffer
      pdata->prev_conv_buffer[i - L] = pdata->curr_conv_buffer[i];
    }
  }
  
  return 0;
}

/**
 * Not optimized version of overlap add.
 * 
 * See `overlap_add`.
 */
int overlap_add2(struct data_struct *pdata, MY_TYPE *out_buffer, unsigned int n_buffer_frames)
{

  // Short variables // TODO remove unsigned long, only unsigned int for size variables
  unsigned long L = (unsigned long)n_buffer_frames;
  unsigned long M = (unsigned long)pdata->ir_size;

  // Add the previous overlapping part to pdata->curr_conv_buffer
  for (int i = 0; i < M; i++)
  {
    pdata->curr_conv_buffer[i] += pdata->prev_conv_buffer[i];
  }

  // Fill overlap with next buffer
  for (int i = L; i < L + M; i++)
  {
    pdata->prev_conv_buffer[i - L] = pdata->curr_conv_buffer[i];
  }

  // Fill output buffer
  for (int i = 0; i < L; i++)
  {
    out_buffer[i] = pdata->curr_conv_buffer[i];
  }

  return 0;
}