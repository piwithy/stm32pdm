//
// Created by jezegoup on 10/14/21.
//
#include "file_handling.h"


//extern char USBHPath[4];   /* USBH logical drive path */
//extern FATFS USBHFatFS;    /* File system object for USBH logical drive */
//extern FIL USBHFile;       /* File object for USBH */

FILINFO USBHfno;
FRESULT fresult;  // result
UINT br, bw;  // File read/write count

/**** capacity related *****/
FATFS *pUSBHFatFS;
DWORD fre_clust;
uint32_t total, free_space;


/** Mount USB Disk
 * Mount detected USB DIsk
 *
 * */
FRESULT mount_usb(void) {
    fresult = f_mount(&USBHFatFS, USBHPath, 1);
    return fresult;
}

/** UnMount USB Disk
 * UnMount mounted USB DIsk
 *
 * */
FRESULT unmount_usb(void) {
    fresult = f_mount(NULL, USBHPath, 1);
    return fresult;
}

FRESULT write_text(const char *file_name, const char *text) {
    fresult = f_open(&USBHFile, file_name, FA_CREATE_ALWAYS | FA_WRITE);
    if (fresult != FR_OK) return fresult;
    f_write(&USBHFile, (char *) text, strlen(text), &bw);
    if (fresult != FR_OK) return fresult;
    f_close(&USBHFile);
    return fresult;
}

FRESULT init_wav(const char *file_name, uint32_t fs) {
    uint32_t header[] = {
            // Declaration fichier
            ('F' << 24) + ('F' << 16) + ('I' << 8) + ('R' << 0), // 'RIFF'
            36, // file size + 44 (header) - 8
            0x45564157, // WAVE
            0x20746D66, // 'fmt '
            0x10,
            0x00010001, // PCM Entier 1 canal
            fs, // fs 32kHz
            2 * fs, // 32kHz * 2 octet par bloc ech
            0x00100002, // 2 octet par block d'ech et 16 bit par ech
            0x61746164, //'data'
            0
    };
    fresult = f_open(&USBHFile, file_name, FA_CREATE_ALWAYS | FA_WRITE);
    if (fresult != FR_OK) return fresult;
    fresult = f_write(&USBHFile, (char *) header, 44, &bw);
    return fresult;
}

FRESULT write_wav(const uint16_t *pcm_data, uint32_t pcm_size) {
    // Writing DATA
    fresult = f_write(&USBHFile, (char *) pcm_data, sizeof(uint16_t) * pcm_size, &bw);
    return fresult;
}

FRESULT finish_wav(const char* file_name, uint32_t pcm_size) {
    fresult = f_close(&USBHFile);
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
    return f_close(&USBHFile);

}

/*
FRESULT update_wav(const char *file_name, const uint16_t *pcm_data, uint32_t pcm_size) {
    fresult = f_open(&USBHFile, file_name, FA_OPEN_ALWAYS | FA_WRITE | FA_READ);
    if (fresult != FR_OK) return fresult;
    uint32_t header[11];
    // reading existing Header
    fresult = f_read(&USBHFile, (uint8_t *) header, 44, &br);
    if (fresult != FR_OK) return fresult;
    uint32_t data_size_byte = header[10];
    // Editing Header
    header[1] += pcm_size * sizeof(uint16_t);
    header[10] += pcm_size * sizeof(uint16_t);
    // Writing new Header
    f_rewind(&USBHFile);
    fresult = f_write(&USBHFile, (char *) header, 44, &bw);
    if (fresult != FR_OK) return fresult;
    //Writing new Data
    fresult = f_lseek(&USBHFile, data_size_byte + 44);
    if (fresult != FR_OK) return fresult;
    fresult = f_write(&USBHFile, (char *) pcm_data, pcm_size * sizeof(uint16_t), &bw);
    if (fresult != FR_OK) return fresult;

    //Closing File
    fresult = f_close(&USBHFile);
    return fresult;
}*/