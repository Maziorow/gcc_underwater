#include "dsp.h"
#include <complex.h>
#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <float.h>
#include <stdlib.h>
#include <stdio.h>

#define DATA_SIZE 2048
#define SOUND_SPEED 1481

void shift(double in[], int32_t n){
    size_t midpoint = (int32_t) floor(n / 2.0);
    for (size_t i = 0; i < midpoint; i++) {
        in[n - midpoint + i] = in[i];
    }
}

double gcc_phat_sec_stage(cplx siga_fft[], cplx sigb_fft[], int margin, int32_t n){
    //R = SIG * REFSIG_CONJ
    cplx* R = (cplx*) malloc(n * sizeof(cplx));
    double* cross_corr = (double*) malloc(n * sizeof(double));

    for(uint32_t i=0; i<n; i++) {
        cplx v = sigb_fft[i] * conj(siga_fft[i]);
        R[i] = v / (cabs(v) + DBL_MIN);
    }

    ifft(R, n);

    for(uint32_t i = 0; i < n; i++){
        cross_corr[i] = creal(R[i]);
    }

    shift(cross_corr, n);

    uint32_t center_i =  ceil(n / 2.0);
    uint32_t new_margin = margin;
    if (((int)(center_i - new_margin)) < 0) new_margin = center_i;
    if ((center_i + new_margin) >= n)new_margin = (n-1) - center_i;

    int32_t start_i = center_i - new_margin;
    int32_t len = 2 * new_margin + 1;
    int32_t max_index = 0;

    double max_value = -DBL_MAX;
    for (int32_t i = start_i; i < start_i + len; i++) {
        if (cross_corr[i] > max_value) {
            max_value = cross_corr[i];
            max_index = i;
        }
    }

    free(R);
    free(cross_corr);

    return (double) (max_index - center_i);
}

double gcc_eckart_sec_stage(cplx siga_fft[], cplx sigb_fft[], int margin, int32_t n){
    //R = SIG * REFSIG_CONJ
    cplx* R = (cplx*) malloc(n * sizeof(cplx));
    double* cross_corr = (double*) malloc(n * sizeof(double));

    for(uint32_t i=0; i<n; i++) {
        cplx v = sigb_fft[i] * conj(siga_fft[i]);
        double Px = creal(siga_fft[i]) * creal(siga_fft[i]) + cimag(siga_fft[i]) * cimag(siga_fft[i]);
        double Py = creal(sigb_fft[i]) * creal(sigb_fft[i]) + cimag(sigb_fft[i]) * cimag(sigb_fft[i]);
        double denominator = Px + Py + DBL_MIN;

        R[i] = v / denominator;
    }

    ifft(R, n);

    for(uint32_t i = 0; i < n; i++){
        cross_corr[i] = creal(R[i]);
    }

    shift(cross_corr, n);

    uint32_t center_i =  ceil(n / 2.0);
    uint32_t new_margin = margin;
    if (((int)(center_i - new_margin)) < 0) new_margin = center_i;
    if ((center_i + new_margin) >= n)new_margin = (n-1) - center_i;

    int32_t start_i = center_i - new_margin;
    int32_t len = 2 * new_margin + 1;
    int32_t max_index = 0;

    double max_value = -DBL_MAX;
    for (int32_t i = start_i; i < start_i + len; i++) {
        if (cross_corr[i] > max_value) {
            max_value = cross_corr[i];
            max_index = i;
        }
    }

    free(R);
    free(cross_corr);

    return (double) (max_index - center_i);
}

double gcc_to_angle(double delay, int32_t sample_freq, double sensor_distance){
    double delay_time = delay / sample_freq;
    return (180.0/PI)*asin((SOUND_SPEED * delay_time) / sensor_distance);
}