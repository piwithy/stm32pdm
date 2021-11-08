/**
  ******************************************************************************
  * @file           file_handling.h
  * @brief          Header of the file handling library
  ******************************************************************************
  */
#ifndef DEMONSTRATEUR_PLAYBACK_FILE_HANDLING_H
#define DEMONSTRATEUR_PLAYBACK_FILE_HANDLING_H

#include "fatfs.h"

/** @brief Mount USB Disk
 * @return Status code associated with the operation
 * */
FRESULT mount_usb(void);


/** @brief unMount USB Disk
 * @return Status code associated with the operation
 * */
FRESULT unmount_usb(void);

/**
 * @brief Initialize a new WAV File by creating the File pointer and Writing the file Header
 *
 * @param file_name Path to the file in the USB Stick
 * @param fs Sampling Frequency of the PCM samples
 * @return Status code associated with the operation
 */
FRESULT init_wav(const char *file_name, uint32_t fs);

/**
 * @brief Write new PCM Samples to the opened file
 * @attention A Wav file have to be already opened by calling init_wav() before calling this function
 *
 * @param pcm_data PCM samples to write in the WAV file
 * @param pcm_size Number of samples to write
 * @return Status code associated with the operation
 */
FRESULT write_wav(const uint16_t *pcm_data, uint32_t pcm_size);

/**
 * @brief "Finishes" a WAV file by writing the size of it in the header and closing the file pointer
 * @attention A Wav file have to be already Initialized by calling init_wav() before calling this function
 *
 * @param file_name Path to the file in the USB Stick
 * @param pcm_size Number of PCM Samples in the file
 * @return Status code associated with the operation
 */
FRESULT finish_wav(const char *file_name, uint32_t pcm_size);


#endif //DEMONSTRATEUR_PLAYBACK_FILE_HANDLING_H