#ifndef MYDEF_H
#define MYDEF_H

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

  MY_TYPE *curr_conv_buffer; // L+M-1
  MY_TYPE *prev_conv_buffer; // L+M-1

  double *stats;
  unsigned long stats_size;
  unsigned long statpos;
};

#endif