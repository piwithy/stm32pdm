/**
  ******************************************************************************
  * @file           file_handling.c
  * @brief          Sources of the file handling library
  ******************************************************************************
  */
#include "file_handling.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//extern char USBHPath[4];   /* USBH logical drive path */
//extern FATFS USBHFatFS;    /* File system object for USBH logical drive */
//extern FIL USBHFile;       /* File object for USBH */

extern UART_HandleTypeDef huart1;

/** @brief  Read Counters */
size_t br;
/** @brief  Read Counters */
size_t bw;  // File read/write count




FRESULT mount_usb(void) {
    FRESULT fresult = f_mount(&USBHFatFS, USBHPath, 1);
    if (fresult == FR_OK) {
        char *str_buff = malloc(100 * sizeof(char));
        sprintf(str_buff, "USB Disk Mounted Successfully!\r\n");
        HAL_UART_Transmit(&huart1, (uint8_t *) str_buff, strlen(str_buff), HAL_MAX_DELAY);
        free(str_buff);
    } else {
        char *str_buff = malloc(100 * sizeof(char));
        sprintf(str_buff, "ERROR While Mounting USB Disk!\r\n");
        HAL_UART_Transmit(&huart1, (uint8_t *) str_buff, strlen(str_buff), HAL_MAX_DELAY);
        free(str_buff);
    }
    return fresult;
}


FRESULT unmount_usb(void) {
    FRESULT fresult = f_mount(NULL, USBHPath, 1);
    if (fresult == FR_OK) {
        char *str_buff = malloc(100 * sizeof(char));
        sprintf(str_buff, "USB Disk Unmounted Successfully!\r\n\n");
        HAL_UART_Transmit(&huart1, (uint8_t *) str_buff, strlen(str_buff), HAL_MAX_DELAY);
        free(str_buff);
    } else {
        char *str_buff = malloc(100 * sizeof(char));
        sprintf(str_buff, "ERROR While Unmounting USB Disk!\r\n\n");
        HAL_UART_Transmit(&huart1, (uint8_t *) str_buff, strlen(str_buff), HAL_MAX_DELAY);
        free(str_buff);
    }
    return fresult;
}

FRESULT init_wav(const char *file_name, uint32_t fs) {
    uint32_t header[] = {
            // Declaration fichier
            ('F' << 24) + ('F' << 16) + ('I' << 8) + ('R' << 0), // 'RIFF'
            36, // file size + 44 (header) - 8
            ('E' << 24) + ('V' << 16) + ('A' << 8) + ('W' << 0), // WAVE
            (' ' << 24) + ('t' << 16) + ('m' << 8) + ('f' << 0), // 'fmt '
            0x10,
            (1 << 16) + (1 << 0), // PCM Entier + 1 canal
            fs, // fs 32kHz
            fs * 2, // 32kHz * 2 octet par bloc ech
            (16 << 16) + (2 << 0), // 2 octet par block d'ech et 16 bit par ech
            ('a' << 24) + ('t' << 16) + ('a' << 8) + ('d' << 0), //'data'
            0
    };
    FRESULT fresult = f_open(&USBHFile, file_name, FA_CREATE_ALWAYS | FA_WRITE);
    if (fresult != FR_OK) return fresult;
    fresult = f_write(&USBHFile, (char *) header, 44, &bw);
    if (fresult == FR_OK) {
        char *str_buff = malloc(100 * sizeof(char));
        sprintf(str_buff, "Started Recording to \"%s\"\r\n", file_name);
        HAL_UART_Transmit(&huart1, (uint8_t *) str_buff, strlen(str_buff), HAL_MAX_DELAY);
        free(str_buff);
    }
    fresult = f_close(&USBHFile);
    if (fresult != FR_OK) return fresult;
    fresult = f_open(&USBHFile, file_name, FA_OPEN_APPEND | FA_WRITE);
    return fresult;
}

FRESULT write_wav(const uint16_t *pcm_data, uint32_t pcm_size) {
    // Writing DATA
    FRESULT fresult = f_write(&USBHFile, (char *) pcm_data, sizeof(uint16_t) * pcm_size, &bw);
    if (fresult != FR_OK) return fresult;
    fresult = f_sync(&USBHFile);
    return fresult;
}

FRESULT finish_wav(const char *file_name, uint32_t pcm_size) {
    FRESULT fresult = f_close(&USBHFile);
    if (fresult != FR_OK) return fresult;
    fresult = f_open(&USBHFile, file_name, FA_OPEN_ALWAYS | FA_WRITE | FA_READ);
    if (fresult != FR_OK) return fresult;
    uint32_t header[11];
    // reading existing Header
    fresult = f_read(&USBHFile, (uint8_t *) header, 44, &br);
    if (fresult != FR_OK) return fresult;
    // Editing Header
    header[1] += pcm_size * sizeof(uint16_t);
    header[10] += pcm_size * sizeof(uint16_t);
    // Writing new Header
    f_rewind(&USBHFile);
    fresult = f_write(&USBHFile, (char *) header, 44, &bw);
    if (fresult != FR_OK) return fresult;

    char *str_buff = malloc(100 * sizeof(char));
    sprintf(str_buff, "Finished recording to \"%s\"\r\n", file_name);
    HAL_UART_Transmit(&huart1, (uint8_t *) str_buff, strlen(str_buff), HAL_MAX_DELAY);
    sprintf(str_buff, "\tSampling Frequency : %.2f kHz\r\n", (double) header[6] / 1000.);
    HAL_UART_Transmit(&huart1, (uint8_t *) str_buff, strlen(str_buff), HAL_MAX_DELAY);
    sprintf(str_buff, "\tRecord Length      : %.2f s\r\n", (double) pcm_size / header[6]);
    HAL_UART_Transmit(&huart1, (uint8_t *) str_buff, strlen(str_buff), HAL_MAX_DELAY);
    free(str_buff);
    return f_close(&USBHFile);

}
