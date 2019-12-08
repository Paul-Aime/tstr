#ifndef UTILS_H
#define UTILS_H

template <class T>
T *intervals(T *arr, int arr_size);

template <class T>
void print_array(T *arr, unsigned long size, int n_elt_per_row = 5, int max_elt = 20);

#include "utils.tcc"

#endif