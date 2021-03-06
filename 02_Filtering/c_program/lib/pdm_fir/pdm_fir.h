#ifndef PDM_FIR_H
#define PDM_FIR_H
/**
 * @file pdm_fir.h
 * @brief PDM FIR Header File.
 * This library Implements a Fir Filter to convert a PDM Signal to a PCM Sound
 *
 */


#include "pdm_fir_.h"

/**
 * @brief Struct Containing the filter internal Data
 */
typedef struct {
    /** Sample Buffer*/
    uint16_t buffer[PDM_FTL_TAPS];
    /** Circular buffer index*/
    int next_tap;
} pdm_fir_filter_t;

/**
 * @brief Struct Containing the Config of the Filter
 */
typedef struct {
    /** FIR Filter */
    pdm_fir_filter_t *fir_filter;
    /** Factor between PDM_fs and PCM_fs  (multiple of 16)*/
    uint16_t decimation_factor;
    /** Offset to apply to the PCM Signal BEFORE amplification */
    int16_t input_offset;
    /** Offset to apply to the PCM Signal AFTER amplification */
    int16_t output_offset;
    /** Amplification Factor (linear) */
    uint16_t linear_gain;
    /** Number of bits used for the output PCM samples */
    uint8_t bit_scale;
} pdm_fir_filter_config_t;

/**
 * @brief Initialize filter
 *
 * @param f pdm filter
 * @return void
 */
void pdm_fir_flt_init(pdm_fir_filter_t *f);

/**
 * @brief Initialize config & filter
 * @param f filter config
 * @param decimation_factor Decimation factor of the filter
 * @param input_offset offset to be applied on the sample before amplification
 * @param output_offset offset to be applied on the sample after amplification
 * @param linear_gain amplification coefficient
 * @param bit_scale number of output bits (output right aligned)
 */
void pdm_fir_flt_config_init(pdm_fir_filter_config_t *f, uint16_t decimation_factor, int16_t input_offset,
                             int16_t output_offset, uint16_t linear_gain, uint8_t bit_scale);

/**
 * @brief deInitialize config & filter
 * @param f filter config
 */
void pdm_fir_flt_config_deInit(pdm_fir_filter_config_t *f);

/**
 * @brief Put 16 bits of input PDM signal (MSB first)
 *
 * @param f pdm filter
 * @param bits PDM input word
 *
 * @return void
 */
void pdm_fir_flt_put(pdm_fir_filter_t *f, uint16_t bits);

/**
 * @brief Retrieve output value.
 *
 * May be called at any rate since it does not change the filter state.
 * The output ranges from -(2**(out_bits-1)) to +(2**(out_bits-1)). Those values correspond to
 * all 0 or all 1 input signal. Note that the output value may still exceed this range so caller
 * should truncate return value on its own if necessary.
 *
 * @param f pdm filter
 * @param out_bits number of bits of the sample
 * @return void
 */
int pdm_fir_flt_get(const pdm_fir_filter_t *f, int out_bits);


/**
 * @brief Filter a chunk of PDM Data
 *

 *
 * @param f fir filter Config
 * @param pcm_output PCM output Buffer
 * @param pdm_input PDM Input Buffer
 * @param pdm_size Size of the PDM Buffer (in 16 bit word)
 * @return Number of PCM Samples written in the buffer
 */
uint32_t
pdm_fir_flt_chunk(pdm_fir_filter_config_t *f, uint16_t *pcm_output, const uint16_t *pdm_input, uint32_t pdm_size);


#endif // PDM_FIR_H