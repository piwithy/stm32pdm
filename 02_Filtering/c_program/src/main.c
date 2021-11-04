/**
 * @file main.c
 * @author Pierre-Yves JEZEGOU <pierre-yves.jezegou@ensta-bretagne.org>
 *
 * @brief Main file of the program for filtering PDM Data on PC
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include <getopt.h>

#include "pdm_fir.h"

#include "config.h"

/** @brief Size in Bits of a PDM Word */
#define WORD_SIZE 16

/**
 * @brief Function Used to print the usage Message
 *
 * @param stream Stream Where to print the message
 * @param program_name Program address (argv[0])
 * @param help_mode If it is A Help Message or juste the Usage (true: HELP)
 */
void print_usage(FILE *stream, char *program_name, bool help_mode);

/**@brief Program Main Function
 *
 * @param argc Argument Count
 * @param argv Argument Vector
 * @return Error Code
 */
int main(int argc, char *argv[]) {
    char *input_file_name = NULL, *output_file_name = NULL;
    uint8_t decimation_factor = 64;
    size_t pdm_fs = 3072000;
    bool verbose = false;
    int opt;
    char *endptr = NULL;
    while ((opt = getopt(argc, argv, "hf:s:")) != -1) {
        switch (opt) {
            case 'h':
                print_usage(stdout, argv[0], true);
                exit(EXIT_SUCCESS);
            case 'f':
                decimation_factor = strtol(optarg, &endptr, 10);
                break;
            case 's':
                pdm_fs = strtol(optarg, &endptr, 10);
                break;
            default:
                print_usage(stderr, argv[0], false);
                exit(EXIT_FAILURE);
        }
    }
    size_t pcm_fs = pdm_fs / decimation_factor;
    if (argc - optind < 2) {
        fprintf(stderr, "Missing Input / Output file Names\n");
        print_usage(stderr, argv[0], false);
        exit(EXIT_FAILURE);
    }
    input_file_name = malloc(strlen(argv[optind]));
    strcpy(input_file_name, argv[optind]);
    printf("Importing PDM from \"%s\"\n", input_file_name);
    optind++;

    output_file_name = malloc(strlen(argv[optind]));
    strcpy(output_file_name, argv[optind]);
    printf("Exporting PCM to \"%s\"\n", output_file_name);

    size_t pdm_buffer_size = (pcm_fs / 1000) * (decimation_factor / WORD_SIZE);
    size_t pcm_buffer_size = pcm_fs / 1000;


    printf("Opening : \"%s\"\n", input_file_name);
    FILE *input_fp = fopen(input_file_name, "rb");
    if (input_fp == NULL) {
        perror(input_file_name);
        exit(EXIT_FAILURE);
    }
    printf("Reading Input File\n");
    fseek(input_fp, 0L, SEEK_END);
    size_t pdm_size_bytes = ftell(input_fp);
    rewind(input_fp);
    size_t pdm_size_words = pdm_size_bytes / (WORD_SIZE / 8);
    printf("\"%s\" is %lu bytes long (%lu PDM Word | %lu PDM Samples)\n", input_file_name, pdm_size_bytes,
           pdm_size_words, WORD_SIZE * pdm_size_words);


    uint16_t *pdm_sound = malloc(pdm_size_bytes);
    fread(pdm_sound, pdm_size_bytes, 1, input_fp);
    fclose(input_fp);
    printf("\"%s\" closed !\n", input_file_name);


    size_t pcm_sample_count = pdm_size_words / (decimation_factor / WORD_SIZE);
    pcm_sample_count = pcm_sample_count - (pcm_sample_count % pcm_buffer_size);
    uint16_t *pcm_sound = malloc(sizeof(uint16_t) * pcm_sample_count);

    printf("Initializing PDM Filter\n");

    pdm_fir_filter_config_t filter_config;
    pdm_fir_flt_config_init(&filter_config, decimation_factor, 0, 0, 1, 16);

    uint16_t pdm_buffer[pdm_buffer_size];
    uint16_t pcm_buffer[pcm_buffer_size];

    size_t pcm_sound_index = 0;
    size_t pdm_sound_index = 0;
    printf("Filtering PDM Signal");
    while (pcm_sound_index < pcm_sample_count) {
        memcpy(pdm_buffer, pdm_sound + pdm_sound_index, pdm_buffer_size * sizeof(uint16_t));
        pdm_sound_index += pdm_sound_index;

        size_t filtered = pdm_fir_flt_chunk(&filter_config, pcm_buffer, pdm_buffer, pdm_buffer_size);


        memcpy(pcm_sound + pcm_sound_index, pcm_buffer, filtered * sizeof(uint16_t));
        pcm_sound_index += filtered;
    }
    printf("Filtered %ld Samples sampled at %.2f kHz (%.2f kHz / %d)\n", pcm_sound_index, (double) pcm_fs / 1000.,
           (double) pdm_fs / 1000., decimation_factor);


    printf("Opening \"%s\"\n", output_file_name);
    FILE *output_fp = fopen(output_file_name, "wb");
    if (output_fp == NULL) {
        perror(output_file_name);
    }

    printf("Writing PCM Samples to \"%s\"\n", output_file_name);
    fwrite(pcm_sound, pcm_sample_count * sizeof(uint16_t), 1, output_fp);
    printf("Wrote %lu bytes to \"%s\"\n", pcm_sample_count * sizeof(uint16_t), output_file_name);
    fclose(output_fp);
    printf("\"%s\" closed !\n", output_file_name);


    free(input_file_name);
    free(output_file_name);
    free(pdm_sound);
    free(pcm_sound);
    return 0;
}


void print_usage(FILE *stream, char *program_name, bool help_mode) {
    fprintf(stream, "USAGE: %s [-h] [-f FACTOR] [-s SAMPLING] <Input File> <Outpufile>\n", program_name);
    if (help_mode == true) {
        fprintf(stream, "C Program to filter PDM Signals to PCM format\n\n");
        fprintf(stream, "Positional arguments:\n");
        fprintf(stream, "    <Input File>        PDM Signal file\n");
        fprintf(stream, "    <Output File>       PCM output file\n");
        fprintf(stream, "\nOptional arguments:\n");
        fprintf(stream, "    -h                  Show this help message\n");
        fprintf(stream, "    -f FACTOR           Decimation Factor of the filter\n");
        fprintf(stream, "    -s SAMPLING         Sampling Frequency of the PDM Signal\n");
    }
}