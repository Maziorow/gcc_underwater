#ifndef FFT_H
#define FFT_H

#include <complex.h>

extern const double PI; // Declare `PI` as external
typedef double complex cplx;

cplx double_to_cplx(double num);
void fft();             // Declare the `fft` function
void _fft();            // Declare the `_fft` function
void show();            // Declare the `show` function
void ifft();             // Declare the `fft` function
void _ifft();

#endif
