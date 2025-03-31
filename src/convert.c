#include "fft.h"
#include <stdint.h>

double fixed_to_double(int16_t value){
    if (value & 0x0800){
        value |= 0xF000;
    }
    return (double)value * (500.0 / 2048);
}

double raw_single_ch_to_voltage(uint32_t raw) {
    int16_t cut = raw & 0xFFF;

    if (cut & 0x0800) {
        cut |= 0xF000;
    }
    return (double)cut * (500.0 / 2048);
}

cplx raw_to_complex(int32_t raw_data){
    int16_t real_part = raw_data & 0xFFF;
    int16_t imag_part = (raw_data >> 16) & 0xFFF;

    cplx result = fixed_to_double(real_part) + fixed_to_double(imag_part)*I;
    return result;
}