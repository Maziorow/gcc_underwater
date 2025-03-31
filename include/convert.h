#ifndef CONVERT_H
#define CONVERT_H

#include "fft.h"

double fixed_to_double(int16_t value);
double raw_single_ch_to_voltage(uint32_t raw);
cplx raw_to_complex(int32_t raw_data);

#endif 

