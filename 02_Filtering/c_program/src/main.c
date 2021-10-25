#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <getopt.h>

#include "pdm_fir.h"

#include "config.h"

#define PDM_WORD_SIZE 16

typedef enum {
    FALSE, TRUE
} bool;

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

/**@brief Program Main Function
 *
 * @param argc Argument Count
 * @param argv Argument Vector
 * @return Error Code
 */
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
    pdm_fir_filter_config_t filter;
    pdm_fir_flt_config_init(&filter, decimation_factor, 0, 0, 1, PDM_WORD_SIZE);

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
    uint16_t pdm_buffer[128];
    uint16_t pcm_buffer[128 / (decimation_factor / PDM_WORD_SIZE)];

    fprintf(stdout, "Filtering PDM Signal\n");
    size_t pcm_length = pdm_length / (decimation_factor / PDM_WORD_SIZE);
    uint16_t *pcm_signal;
    pcm_signal = malloc(pcm_length * sizeof(uint16_t));
    uint32_t pcm_sound_index = 0;
    if (pcm_signal == NULL) {
        fprintf(stderr, "MALLOC FAILED!\n");
        return -1;
    }

    // Filtering PDM Signal in chunks of 128 * 16 PDM Samples
    for (size_t pdm_index = 0; pdm_index < pdm_length; pdm_index += 128) {
        //Copying Into pdm buffer
        memcpy(pdm_buffer, pdm_signal + pdm_index, 128 * sizeof(uint16_t));

        //filtering
        size_t filtered = pdm_fir_flt_chunk(&filter, pcm_buffer, pdm_buffer, 128);

        //Copying into PCM Signal
        memcpy(pcm_signal + pcm_sound_index, pcm_buffer, sizeof(uint16_t) * filtered);
        pcm_sound_index += filtered;

    }


    //Writing PCM
    fprintf(stdout, "Writing PCM to \"%s\"\n", output_file);
    fp = fopen(output_file, "wb");
    fwrite(pcm_signal, pcm_length * sizeof(uint16_t), 1, fp);
    fclose(fp);

    pdm_fir_flt_config_deInit(&filter);
    free(pcm_signal);
    free(pdm_signal);
    return 0;
}
