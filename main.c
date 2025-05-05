#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <complex.h>
#include <sndfile.h>
#include "dsp.h"
#include "xfft_v9_1_bitacc_cmodel.h"

void rotate(cplx out[], cplx in[], int rot) {
    int n = 2048;
    for (int i=0; i<n ;i++) {
        unsigned int j = (i+rot)%n;
        out[j] = in[i];
    }
}

int load_wav_data(const char *filename, cplx sig_ref[], int nsamps) {
    SF_INFO sfinfo;
    SNDFILE *file = sf_open(filename, SFM_READ, &sfinfo);
    
    if (!file) {
        printf("Error opening WAV file: %s\n", sf_strerror(NULL));
        return -1;
    }

    if (sfinfo.channels != 1) {
        printf("Only mono WAV files are supported\n");
        sf_close(file);
        return -1;
    }

    float buffer[nsamps];
    int frames = sf_readf_float(file, buffer, nsamps);
    if (frames < nsamps) {
        printf("Warning: Not enough samples in WAV file\n");
    }

    for (int i = 0; i < frames; i++) {
        sig_ref[i] = buffer[i] + 0.0 * I; // Convert to complex numbers
    }

    sf_close(file);
    return 0;
}

int main(void) {
    int true_delay_a = 15;
    int true_delay_b = 30;
    double amp = 0.5;
    int samp_freq = 1e6;

    double samp_per = 1.0/samp_freq;
    int nsamps = 2048;

    cplx sig_ref[nsamps];
    cplx siga[nsamps];
    cplx sigb[nsamps];

    cplx fft_a_real[nsamps];
    cplx fft_b_real[nsamps];

    //Create from code
    for(int i=0;i<nsamps;i++){
        double v = amp * sin((2.0 * PI * samp_per * i));
        sig_ref[i] = v + 0.0*I;
    }
    rotate(siga,sig_ref,true_delay_a);
    rotate(sigb,sig_ref,true_delay_b);

    //Load from file
    /*
    FILE *file = fopen("data.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    for(int i=0;i<nsamps;i++){
        fscanf(file, "%f,%f,%f\n", &sig_ref[i], &siga[i], &sigb[i]);
    }
    */
    
    fft(sig_ref,nsamps);
    fft(siga,nsamps);
    fft(sigb,nsamps);


    double tau_a = gcc_phat_sec_stage(sig_ref, siga, 500, 2048);
    double tau_b = gcc_phat_sec_stage(sig_ref, sigb, 500, 2048);
    double angle_a = gcc_to_angle(tau_a, samp_freq, 0.3);
    double angle_b = gcc_to_angle(tau_b, samp_freq, 0.3);

    double tau_a_eckart = gcc_eckart_sec_stage(sig_ref, siga, 500, 2048);
    double tau_b_eckart= gcc_eckart_sec_stage(sig_ref, sigb, 500, 2048);
    double angle_a_eckart = gcc_to_angle(tau_a_eckart, samp_freq, 0.3);
    double angle_b_eckart = gcc_to_angle(tau_b_eckart, samp_freq, 0.3);

    printf("Real Tau axis 1: %d\n",true_delay_a);
    printf("Real Tau axis 2: %d\n",true_delay_b);
    printf("Calculated Tau axis 1: %f\n",tau_a);
    printf("Calculated Tau axis 2: %f\n",tau_b);
    printf("Angle axis 1: %f\n", angle_a);
    printf("Angle axis 2: %f\n", angle_b);

    printf("\n");

    printf("Calculated Tau axis 1 for Eckart filter: %f\n",tau_a_eckart);
    printf("Calculated Tau axis 2 for Eckart filter: %f\n",tau_b_eckart);
    printf("Angle axis 1 for Eckart filter: %f\n", angle_a_eckart);
    printf("Angle axis 2 for Eckart filter: %f\n", angle_b_eckart);

    return 0;
}