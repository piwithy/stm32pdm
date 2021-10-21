#include "pdm_fir.h"

#include <stdlib.h>

/* PDM FIR filter.
 * The source frequency is expected to be 1024kHz so we are receiving 16 bit words at 64kHz rate MSB first.
 * The filter cutoff frequency is 8kHz.  -> customized in pdm_fir.py
 */



/* Initialize filter */
void pdm_fir_flt_init(pdm_fir_filter_t *f) {
    int t;
    f->next_tap = 0;
    for (t = 0; t < PDM_FTL_TAPS; ++t)
        f->buffer[t] = 0x5555;
}

/* Put 16 bits MSB first */
void pdm_fir_flt_put(pdm_fir_filter_t *f, uint16_t bits) {
    f->buffer[f->next_tap] = bits;
    if (++f->next_tap >= PDM_FTL_TAPS)
        f->next_tap = 0;
}

/* Retrieve output value. May be called at any rate since it does not change the filter state.
 * The output ranges from -(2**(out_bits-1)) to +(2**(out_bits-1)). Those values correspond to
 * all 0 or all 1 input signal. Note that the output value may still exceed this range so caller
 * should truncate return value on its own if necessary.
 */
int pdm_fir_flt_get(const pdm_fir_filter_t *f, int out_bits) {
    int t, i = 0, tot = 0;
    for (t = f->next_tap;;) {
        uint16_t v = f->buffer[t];
        tot += byte_coeff[i++][(uint8_t) (v >> 8)];
        tot += byte_coeff[i++][(uint8_t) (v)];
        if (++t >= PDM_FTL_TAPS)
            t = 0;
        if (t == f->next_tap)
            break;
    }
    /* Rescale to output range */
    return tot >> (PDM_FTL_SCALE_BITS - out_bits + 1);
}


void pdm_fir_flt_config_init(pdm_fir_filter_config_t *f, uint16_t decimation_factor, int16_t input_offset,
                             int16_t output_offset,
                             uint16_t linear_gain, uint8_t bit_scale) {
    f->fir_filter = malloc(sizeof(pdm_fir_filter_t));
    pdm_fir_flt_init(f->fir_filter);
    f->decimation_factor = decimation_factor;
    f->input_offset = input_offset;
    f->output_offset = output_offset;
    f->linear_gain = linear_gain;
    f->bit_scale = bit_scale;
}


void pdm_fir_flt_config_deInit(pdm_fir_filter_config_t *f) {
    free(f->fir_filter);
}

uint32_t
pdm_fir_flt_chunk(pdm_fir_filter_config_t *f, uint16_t* pcm_output, const uint16_t *pdm_input, uint32_t pdm_size) {
    uint16_t decimation_words = f->decimation_factor / 16;
    size_t pcm_to_write = pdm_size / decimation_words;
    int32_t lim = (1 << f->bit_scale) - 1;
    for (size_t i = 0; i < pcm_to_write; i++) {
        for (size_t j = 0; j < decimation_words; j++) {
            pdm_fir_flt_put(f->fir_filter, pdm_input[i * decimation_words + j]);
        }
        int32_t received_pcm = pdm_fir_flt_get(f->fir_filter, f->bit_scale);
        received_pcm += f->input_offset; // centering PCM Signal
        received_pcm *= f->linear_gain;
        if (received_pcm > lim) received_pcm = lim;
        if (received_pcm < -lim) received_pcm = -lim;
        received_pcm += f->output_offset;
        pcm_output[i] = (uint16_t) received_pcm & ((1 << (f->bit_scale + 1)) - 1);
    }
    return pcm_to_write;
}

