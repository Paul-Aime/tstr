// Template functions implementations.
// See https://stackoverflow.com/a/10632266

#ifndef UTILS_TCC
#define UTILS_TCC

#include <cstdlib>  // malloc, calloc, realloc, free
#include <iostream> // std::cout, std::endl

template <class T>
T *intervals(T *arr, int arr_size)
{
    T *intvls = (T *)malloc(sizeof(T) * (arr_size - 1));

    for (int i = 0; i < arr_size - 1; i++)
    {
        intvls[i] = arr[i + 1] - arr[i];
    }

    return intvls;
}

template <class T>
void print_array(T *arr, unsigned long size, int n_elt_per_row, int max_elt)
{
    int n_elt = (int)std::min(size, (unsigned long)max_elt);
    int n_full_rows = n_elt / n_elt_per_row;
    int n_elt_last_row = n_elt % n_elt_per_row;
    int idx = 0;

    if (!n_elt_last_row) // Ensure to have a last row
    {
        n_full_rows -= 1;
        n_elt_last_row = n_elt_per_row;
    }

    std::cout << "[";

    // Print full rows
    for (int ir = 0; ir < n_full_rows; ir++)
    {
        for (int i = 0; i < n_elt_per_row; i++)
        {
            std::cout << arr[idx++] << ", ";
        }
        std::cout << "\n "; // Alignement
    }

    // Print last row
    for (int i = 0; i < n_elt_last_row - 1; i++)
    {
        std::cout << arr[idx++] << ", ";
    }
    std::cout << arr[idx++] << ", ";

    // Close display
    if (size > max_elt)
    {
        std::cout << "...]" << std::endl;
    }
    else
    {
        std::cout << "\b\b]" << std::endl;
    }
}

template <class T>
T mean(T *arr, unsigned long arr_size)
{
    T mean = 0;
    for (int i = 0; i < arr_size; i++)
    {
        mean += arr[i];
    }

    mean = mean / (T)arr_size;
    return mean;
}

#endif