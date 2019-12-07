#include <cstdlib>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cstdio>

int conv(double *x, unsigned long x_size, double *h, unsigned long h_size, double *y, unsigned long y_size);
double *conv2(double *x, long x_size, double *h, long h_size);
void print_vec(double *vec, long vec_size);

int main(int argc, char const *argv[])
{
    double x[3] = {3, 7, 5};
    double h[5] = {1, 6, 1, 2, 5};

    double *y = (double *)malloc(sizeof(double) * (3 + 5 - 1));

    conv(x, 3, h, 5, y, 3 + 5 - 1);
    print_vec(y, 3 + 5 - 1);

    return 0;
}

void print_vec(double *vec, long vec_size)
{
    if (vec_size > 0)
    {
        std::cout << "[";
    }

    for (int i = 0; i < vec_size - 1; i++)
    {
        std::cout << vec[i] << ", ";
    }

    std::cout << vec[vec_size - 1] << "]";
}

int conv(double *x, unsigned long x_size, double *h, unsigned long h_size, double *y, unsigned long y_size)
{

    // Check for size
    if (y_size != x_size + h_size - 1)
    {
        return 1;
    }

    // Declare variables for current conv limits
    int kmin, kmax;

    // Do the actual convolution
    for (int n = 0; n < x_size + h_size - 1; n++)
    {
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