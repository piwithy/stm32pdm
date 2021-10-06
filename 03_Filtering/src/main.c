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

void print_usage(FILE *stream, char *program_name, bool help_mode) {
    fprintf(stream, "USAGE: %s [-h] [-f FACTOR] <Input File> <Outpufile>\n", program_name);
    if (help_mode == TRUE) {
        fprintf(stdout, "C Program to filter PDM Signals to PCM format\n\n");
        fprintf(stdout, "Positional arguments:\n");
        fprintf(stdout, "\t<Input File>\t\tPDM Signal file\n");
        fprintf(stdout, "\t<Output File>\t\tPCM output file\n");
        fprintf(stdout, "\nOptional arguments:\n");
        fprintf(stdout, "\t-h\t\t\t\t\tShow this help message\n");
        fprintf(stdout, "\t-f FACTOR\t\t\tDecimation Factor of the filter\n");
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
    fprintf(stdout, "Filtering PDM Signal\n");
    size_t pcm_length = pdm_length / (DECIMATION_FACTOR / PDM_WORD_SIZE);
    uint16_t *pcm_signal;
    pcm_signal = malloc(pcm_length * sizeof(uint16_t));
    if (pcm_signal == NULL) {
        fprintf(stderr, "MALLOC FAILED!\n");
        return -1;
    }

    for (size_t i = 0; i < pcm_length; i++) {
        for (size_t j = 0; j < (DECIMATION_FACTOR / PDM_WORD_SIZE); j++) {
            pdm_fir_flt_put(&pdm_filter, pdm_signal[i * (DECIMATION_FACTOR / PDM_WORD_SIZE) + j]);
        }
        pcm_signal[i] = pdm_fir_flt_get(&pdm_filter, PDM_WORD_SIZE) & (1 >> PDM_WORD_SIZE) - 1;
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
