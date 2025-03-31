#ifndef DSP_H
#define DSP_H

#include "fft.h"
#include <stdint.h>

void shift(double in[], int32_t n);
double gcc_phat_sec_stage(cplx siga_fft[], cplx sigb_fft[], int margin, int32_t n);
double gcc_eckart_sec_stage(cplx siga_fft[], cplx sigb_fft[], int margin, int32_t n);
double gcc_to_angle(double delay, int32_t sample_freq, double sensor_distance);

#endif