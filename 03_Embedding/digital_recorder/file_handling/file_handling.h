//
// Created by jezegoup on 10/14/21.
//

#ifndef DEMONSTRATEUR_PLAYBACK_FILE_HANDLING_H
#define DEMONSTRATEUR_PLAYBACK_FILE_HANDLING_H

#include "fatfs.h"

/** Mount USB Disk
 * Mount detected USB DIsk
 *
 * */
FRESULT mount_usb(void);

/** UnMount USB Disk
 * UnMount mounted USB DIsk
 *
 * */
FRESULT unmount_usb(void);

FRESULT init_wav(const char *file_name, uint32_t fs);

FRESULT write_wav(const uint16_t *pcm_data, uint32_t pcm_size);

FRESULT finish_wav(const char *file_name, uint32_t pcm_size);


#endif //DEMONSTRATEUR_PLAYBACK_FILE_HANDLING_H