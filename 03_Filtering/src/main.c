#include <stdio.h>
#include <stdlib.h>
#include "pdm_fir.h"

#define DECIMATION_FACTOR 64
#define PDM_WORD_SIZE 16

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Not enough Arguments\n");
        return -1;
    }
    struct pdm_fir_filter pdm_filter;
    pdm_fir_flt_init(&pdm_filter);

    printf("Reading PDM file: \"%s\"\n", argv[1]);
    FILE *fp = fopen(argv[1], "rb");

    if (fp == NULL) {
        fprintf(stderr, "\"%s\" Not Found !", argv[1]);
        return -1;
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
    printf("Filtering PDM Signal\n");
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
    printf("Writing PCM to \"%s\"\n", argv[2]);
    fp = fopen(argv[2], "wb");
    fwrite(pcm_signal, pcm_length * sizeof(uint16_t), 1, fp);
    fclose(fp);


    free(pcm_signal);
    free(pdm_signal);
    return 0;
}
