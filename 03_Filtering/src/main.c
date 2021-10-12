#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include "pdm_fir.h"
#include "config.h"

#define DECIMATION_FACTOR 64
#define PDM_WORD_SIZE 16

typedef enum boolean {
    FALSE, TRUE
} bool;

uint32_t filter_chunk(struct pdm_fir_filter *filter, uint16_t *pcm_buffer, uint16_t *pdm_buffer, uint32_t pdm_size,
                      uint32_t decimation_factor, uint8_t linear_gain) {
    uint32_t decimation_factor_words = decimation_factor / PDM_WORD_SIZE;
    uint32_t pcm_to_write = pdm_size / decimation_factor_words;
    for (size_t i = 0; i < pcm_to_write; i++) { // filtering ONE PDM Buffer
        for (size_t j = 0; j < decimation_factor_words; j++) {
            pdm_fir_flt_put(filter, pdm_buffer[i * decimation_factor_words + j]);
        }
        /*
         * on enregistre le sample produit par le filtre
         * on va modifier son amplitude tels que x € [0, 0XFFF] avec une moyenne à Ox7FF
         * ces paramettre correponde a ceux du DAC
         */
        int32_t pcm_tmp = (int32_t) pdm_fir_flt_get(filter, PDM_WORD_SIZE) * linear_gain;
        if (pcm_tmp > 0x7FFF) pcm_tmp = 0x7FFF;
        if (pcm_tmp < -0x7FFF) pcm_tmp = -0x7FFF;
        pcm_buffer[i] = (uint16_t) pcm_tmp & 0xFFFF;
    }
    return pcm_to_write;
}


void print_usage(FILE *stream, char *program_name, bool help_mode) {
    fprintf(stream, "USAGE: %s [-h] [-f FACTOR] <Input File> <Outpufile>\n", program_name);
    if (help_mode == TRUE) {
        fprintf(stdout, "C Program to filter PDM Signals to PCM format\n\n");
        fprintf(stdout, "Positional arguments:\n");
        fprintf(stdout, "    <Input File>        PDM Signal file\n");
        fprintf(stdout, "    <Output File>       PCM output file\n");
        fprintf(stdout, "\nOptional arguments:\n");
        fprintf(stdout, "    -h                  Show this help message\n");
        fprintf(stdout, "    -f FACTOR           Decimation Factor of the filter\n");
    }
}


int main(int argc, char *argv[]) {
    // Argument handling
    char *input_file = NULL;
    char *output_file = NULL;
    fprintf(stdout, "PDM Filter v%s\n", PROJECT_VER);
    uint16_t decimation_factor = 64;
    int opt;
    char *endptr = NULL;
    while ((opt = getopt(argc, argv, "hf:")) != -1) {
        switch (opt) {
            case 'h':
                print_usage(stdout, argv[0], TRUE);
                exit(EXIT_SUCCESS);
            case 'f':
                decimation_factor = strtol(optarg, &endptr, 10);
                break;
            default:
                print_usage(stderr, argv[0], FALSE);
                exit(EXIT_FAILURE);
        }
    }
    if (optind >= argc) {
        print_usage(stderr, argv[0], FALSE);
        exit(EXIT_FAILURE);
    }
    input_file = argv[optind];
    output_file = argv[optind + 1];

    // Filter Init
    struct pdm_fir_filter pdm_filter;
    pdm_fir_flt_init(&pdm_filter);

    // Reading Input
    fprintf(stdout, "Reading PDM file: \"%s\"\n", input_file);
    FILE *fp = fopen(input_file, "rb");

    if (fp == NULL) {
        fprintf(stderr, "\"%s\" Not Found !\n", input_file);
        exit(EXIT_FAILURE);
    }

    //Reading Input file
    fseek(fp, 0L, SEEK_END);
    size_t file_size = ftell(fp);
    size_t pdm_length = file_size / sizeof(uint16_t);
    rewind(fp);
    uint16_t *pdm_signal;
    pdm_signal = malloc(file_size);
    if (pdm_signal == NULL) {
        fprintf(stderr, "MALLOC FAILED!\n");
        return -1;
    }
    fread(pdm_signal, file_size, 1, fp);
    fclose(fp);

    //filtering PDM
    uint32_t pdm_index = 0;
    uint16_t pdm_buffer[128];
    uint16_t pcm_buffer[128 / (DECIMATION_FACTOR / PDM_WORD_SIZE)];

    fprintf(stdout, "Filtering PDM Signal\n");
    size_t pcm_length = pdm_length / (DECIMATION_FACTOR / PDM_WORD_SIZE);
    uint16_t *pcm_signal;
    pcm_signal = malloc(pcm_length * sizeof(uint16_t));
    uint32_t pcm_sound_index = 0;
    if (pcm_signal == NULL) {
        fprintf(stderr, "MALLOC FAILED!\n");
        return -1;
    }

    while (pcm_sound_index < pcm_length) {
        memcpy(pdm_buffer, pdm_signal + pdm_index, 128 * sizeof(uint16_t));
        pdm_index += 128;
        pcm_sound_index += filter_chunk(&pdm_filter, pcm_signal + pcm_sound_index, pdm_buffer, 128, DECIMATION_FACTOR,
                                        1);
    }

    //Writing PCM
    fprintf(stdout, "Writing PCM to \"%s\"\n", output_file);
    fp = fopen(output_file, "wb");
    fwrite(pcm_signal, pcm_length * sizeof(uint16_t), 1, fp);
    fclose(fp);


    free(pcm_signal);
    free(pdm_signal);
    return 0;
}
