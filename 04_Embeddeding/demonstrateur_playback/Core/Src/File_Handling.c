/*
 * File_Handling_RTOS.c
 *
 *  Created on: 26-June-2020
 *      Author: Controllerstech.com
 */

#include "File_Handling.h"
#include "stm32f4xx_hal.h"


extern UART_HandleTypeDef huart1;
#define UART &huart1


/* =============================>>>>>>>> NO CHANGES AFTER THIS LINE =====================================>>>>>>> */



extern char USBHPath[4];   /* USBH logical drive path */
extern FATFS USBHFatFS;    /* File system object for USBH logical drive */
extern FIL USBHFile;       /* File object for USBH */

FILINFO USBHfno;
FRESULT fresult;  // result
UINT br, bw;  // File read/write count

/**** capacity related *****/
FATFS *pUSBHFatFS;
DWORD fre_clust;
uint32_t total, free_space;


void Send_Uart(char *string) {
    HAL_UART_Transmit(UART, (uint8_t *) string, strlen(string), HAL_MAX_DELAY);
}


void Mount_USB(void) {
    fresult = f_mount(&USBHFatFS, USBHPath, 1);
    if (fresult != FR_OK) Send_Uart("ERROR!!! in mounting USB ...\n\r\n\r");
    else Send_Uart("USB mounted successfully...\n\r");
}

void Unmount_USB(void) {
    fresult = f_mount(NULL, USBHPath, 1);
    if (fresult == FR_OK) Send_Uart("USB UNMOUNTED successfully...\n\r\n\r\n\r");
    else Send_Uart("ERROR!!! in UNMOUNTING USB \n\r\n\r\n\r");
}

/* Start node to be scanned (***also used as work area***) */
FRESULT Scan_USB(char *pat) {
    DIR dir;
    UINT i;
    char *path = malloc(20 * sizeof(char));
    sprintf(path, "%s", pat);

    fresult = f_opendir(&dir, path);                       /* Open the directory */
    if (fresult == FR_OK) {
        for (;;) {
            fresult = f_readdir(&dir, &USBHfno);                   /* Read a directory item */
            if (fresult != FR_OK || USBHfno.fname[0] == 0) break;  /* Break on error or end of dir */
            if (USBHfno.fattrib & AM_DIR)     /* It is a directory */
            {
                if (!(strcmp("SYSTEM~1", USBHfno.fname))) continue;
                if (!(strcmp("System Volume Information", USBHfno.fname))) continue;
                char *buf = malloc(30 * sizeof(char));
                sprintf(buf, "Dir: %s\r\n\r", USBHfno.fname);
                Send_Uart(buf);
                free(buf);
                i = strlen(path);
                sprintf(&path[i], "/%s", USBHfno.fname);
                fresult = Scan_USB(path);                     /* Enter the directory */
                if (fresult != FR_OK) break;
                path[i] = 0;
            } else {   /* It is a file. */
                char *buf = malloc(30 * sizeof(char));
                sprintf(buf, "File: %s/%s\n\r", path, USBHfno.fname);
                Send_Uart(buf);
                free(buf);
            }
        }
        f_closedir(&dir);
    }
    free(path);
    return fresult;
}

/* Only supports removing files from home directory */
FRESULT Format_USB(void) {
    DIR dir;
    char *path = malloc(20 * sizeof(char));
    sprintf(path, "%s", "/");

    fresult = f_opendir(&dir, path);                       /* Open the directory */
    if (fresult == FR_OK) {
        for (;;) {
            fresult = f_readdir(&dir, &USBHfno);                   /* Read a directory item */
            if (fresult != FR_OK || USBHfno.fname[0] == 0) break;  /* Break on error or end of dir */
            if (USBHfno.fattrib & AM_DIR)     /* It is a directory */
            {
                if (!(strcmp("SYSTEM~1", USBHfno.fname))) continue;
                if (!(strcmp("System Volume Information", USBHfno.fname))) continue;
                fresult = f_unlink(USBHfno.fname);
                if (fresult == FR_DENIED) continue;
            } else {   /* It is a file. */
                fresult = f_unlink(USBHfno.fname);
            }
        }
        f_closedir(&dir);
    }
    free(path);
    return fresult;
}


FRESULT Write_File(char *name, char *data) {

    /**** check whether the file exists or not ****/
    fresult = f_stat(name, &USBHfno);
    if (fresult != FR_OK) {
        char *buf = malloc(100 * sizeof(char));
        sprintf(buf, "ERROR!!! *%s* does not exists\n\r\n\r", name);
        Send_Uart(buf);
        free(buf);
        return fresult;
    } else {
        /* Create a file with read write access and open it */
        fresult = f_open(&USBHFile, name, FA_OPEN_EXISTING | FA_WRITE);
        if (fresult != FR_OK) {
            char *buf = malloc(100 * sizeof(char));
            sprintf(buf, "ERROR!!! No. %d in opening file *%s*\n\r\n\r", fresult, name);
            Send_Uart(buf);
            free(buf);
            return fresult;
        } else {
            char *buf = malloc(100 * sizeof(char));
            sprintf(buf, "Opening file-->  *%s*  To WRITE data in it\n\r", name);
            Send_Uart(buf);
            free(buf);

            fresult = f_write(&USBHFile, data, strlen(data), &bw);
            if (fresult != FR_OK) {
                char *buf = malloc(100 * sizeof(char));
                sprintf(buf, "ERROR!!! No. %d while writing to the FILE *%s*\n\r\n\r", fresult, name);
                Send_Uart(buf);
                free(buf);
            }

            /* Close file */
            fresult = f_close(&USBHFile);
            if (fresult != FR_OK) {
                char *buf = malloc(100 * sizeof(char));
                sprintf(buf, "ERROR!!! No. %d in closing file *%s* after writing it\n\r\n\r", fresult, name);
                Send_Uart(buf);
                free(buf);
            } else {
                char *buf = malloc(100 * sizeof(char));
                sprintf(buf, "File *%s* is WRITTEN and CLOSED successfully\n\r\n\r", name);
                Send_Uart(buf);
                free(buf);
            }
        }
        return fresult;
    }
}

FRESULT write_wav(const char *name, const uint32_t *header, const uint16_t *data, uint32_t data_size) {

    /**** check whether the file exists or not ****/
    fresult = f_stat(name, &USBHfno);
    if (fresult != FR_OK) {
        char *buf = malloc(100 * sizeof(char));
        sprintf(buf, "ERROR!!! *%s* does not exists\n\r\n\r", name);
        Send_Uart(buf);
        free(buf);
        return fresult;
    } else {
        /* Create a file with read write access and open it */
        fresult = f_open(&USBHFile, name, FA_OPEN_EXISTING | FA_WRITE);
        if (fresult != FR_OK) {
            char *buf = malloc(100 * sizeof(char));
            sprintf(buf, "ERROR!!! No. %d in opening file *%s*\n\r\n\r", fresult, name);
            Send_Uart(buf);
            free(buf);
            return fresult;
        } else {
            char *buf = malloc(100 * sizeof(char));
            sprintf(buf, "Opening file-->  *%s*  To WRITE data in it\n\r", name);
            Send_Uart(buf);
            free(buf);

            //Writing file Header
            f_write(&USBHFile, (uint8_t *) header, 44, &bw);
            if (fresult != FR_OK) {
                char *buf = malloc(100 * sizeof(char));
                sprintf(buf, "ERROR!!! No. %d while writing to the FILE *%s*\n\r\n\r", fresult, name);
                Send_Uart(buf);
                free(buf);
            }
            fresult = f_write(&USBHFile, (uint8_t *) data, data_size * sizeof(uint16_t), &bw);
            if (fresult != FR_OK) {
                char *buf = malloc(100 * sizeof(char));
                sprintf(buf, "ERROR!!! No. %d while writing to the FILE *%s*\n\r\n\r", fresult, name);
                Send_Uart(buf);
                free(buf);
            }

            /* Close file */
            fresult = f_close(&USBHFile);
            if (fresult != FR_OK) {
                char *buf = malloc(100 * sizeof(char));
                sprintf(buf, "ERROR!!! No. %d in closing file *%s* after writing it\n\r\n\r", fresult, name);
                Send_Uart(buf);
                free(buf);
            } else {
                char *buf = malloc(100 * sizeof(char));
                sprintf(buf, "File *%s* is WRITTEN and CLOSED successfully\n\r\n\r", name);
                Send_Uart(buf);
                free(buf);
            }
        }
        return fresult;
    }
}

FRESULT Create_File(char *name) {
    fresult = f_stat(name, &USBHfno);
    if (fresult == FR_OK) {
        char *buf = malloc(100 * sizeof(char));
        sprintf(buf, "ERROR!!! *%s* already exists!!!!\n\r use Update_File \n\r\n\r", name);
        Send_Uart(buf);
        free(buf);
        return fresult;
    } else {
        fresult = f_open(&USBHFile, name, FA_CREATE_ALWAYS | FA_READ | FA_WRITE);
        if (fresult != FR_OK) {
            char *buf = malloc(100 * sizeof(char));
            sprintf(buf, "ERROR!!! No. %d in creating file *%s*\n\r\n\r", fresult, name);
            Send_Uart(buf);
            free(buf);
            return fresult;
        } else {
            char *buf = malloc(100 * sizeof(char));
            sprintf(buf, "*%s* created successfully\n\r Now use Write_File to write data\n\r", name);
            Send_Uart(buf);
            free(buf);
        }

        fresult = f_close(&USBHFile);
        if (fresult != FR_OK) {
            char *buf = malloc(100 * sizeof(char));
            sprintf(buf, "ERROR No. %d in closing file *%s*\n\r\n\r", fresult, name);
            Send_Uart(buf);
            free(buf);
        } else {
            char *buf = malloc(100 * sizeof(char));
            sprintf(buf, "File *%s* CLOSED successfully\n\r\n\r", name);
            Send_Uart(buf);
            free(buf);
        }
    }
    return fresult;
}

FRESULT Update_File(char *name, char *data) {
    /**** check whether the file exists or not ****/
    fresult = f_stat(name, &USBHfno);
    if (fresult != FR_OK) {
        char *buf = malloc(100 * sizeof(char));
        sprintf(buf, "ERROR!!! *%s* does not exists\n\r\n\r", name);
        Send_Uart(buf);
        free(buf);
        return fresult;
    } else {
        /* Create a file with read write access and open it */
        fresult = f_open(&USBHFile, name, FA_OPEN_APPEND | FA_WRITE);
        if (fresult != FR_OK) {
            char *buf = malloc(100 * sizeof(char));
            sprintf(buf, "ERROR!!! No. %d in opening file *%s*\n\r\n\r", fresult, name);
            Send_Uart(buf);
            free(buf);
            return fresult;
        }

        char *buf = malloc(100 * sizeof(char));
        sprintf(buf, "Opening file-->  *%s*  To UPDATE data in it\n\r", name);
        Send_Uart(buf);
        free(buf);

        /* Writing text */
        fresult = f_write(&USBHFile, data, strlen(data), &bw);
        if (fresult != FR_OK) {
            char *buf = malloc(100 * sizeof(char));
            sprintf(buf, "ERROR!!! No. %d in writing file *%s*\n\r\n\r", fresult, name);
            Send_Uart(buf);
            free(buf);
        } else {
            char *buf = malloc(100 * sizeof(char));
            sprintf(buf, "*%s* UPDATED successfully\n\r", name);
            Send_Uart(buf);
            free(buf);
        }

        /* Close file */
        fresult = f_close(&USBHFile);
        if (fresult != FR_OK) {
            char *buf = malloc(100 * sizeof(char));
            sprintf(buf, "ERROR!!! No. %d in closing file *%s*\n\r\n\r", fresult, name);
            Send_Uart(buf);
            free(buf);
        } else {
            char *buf = malloc(100 * sizeof(char));
            sprintf(buf, "File *%s* CLOSED successfully\n\r\n\r", name);
            Send_Uart(buf);
            free(buf);
        }
    }
    return fresult;
}

FRESULT Remove_File(char *name) {
    /**** check whether the file exists or not ****/
    fresult = f_stat(name, &USBHfno);
    if (fresult != FR_OK) {
        char *buf = malloc(100 * sizeof(char));
        sprintf(buf, "ERROR!!! *%s* does not exists\n\r\n\r", name);
        Send_Uart(buf);
        free(buf);
        return fresult;
    } else {
        fresult = f_unlink(name);
        if (fresult == FR_OK) {
            char *buf = malloc(100 * sizeof(char));
            sprintf(buf, "*%s* has been removed successfully\n\r\n\r", name);
            Send_Uart(buf);
            free(buf);
        } else {
            char *buf = malloc(100 * sizeof(char));
            sprintf(buf, "ERROR No. %d in removing *%s*\n\r\n\r", fresult, name);
            Send_Uart(buf);
            free(buf);
        }
    }
    return fresult;
}

FRESULT Create_Dir(char *name) {
    fresult = f_mkdir(name);
    if (fresult == FR_OK) {
        char *buf = malloc(100 * sizeof(char));
        sprintf(buf, "*%s* has been created successfully\n\r\n\r", name);
        Send_Uart(buf);
        free(buf);
    } else {
        char *buf = malloc(100 * sizeof(char));
        sprintf(buf, "ERROR No. %d in creating directory *%s*\n\r\n\r", fresult, name);
        Send_Uart(buf);
        free(buf);
    }
    return fresult;
}

void Check_USB_Details(void) {
    /* Check free space */
    f_getfree("", &fre_clust, &pUSBHFatFS);

    total = (uint32_t) ((pUSBHFatFS->n_fatent - 2) * pUSBHFatFS->csize * 0.5);
    char *buf = malloc(30 * sizeof(char));
    sprintf(buf, "USB  Total Size: \t%lu\n\r", total);
    Send_Uart(buf);
    free(buf);
    free_space = (uint32_t) (fre_clust * pUSBHFatFS->csize * 0.5);
    buf = malloc(30 * sizeof(char));
    sprintf(buf, "USB Free Space: \t%lu\n\r", free_space);
    Send_Uart(buf);
    free(buf);
}

