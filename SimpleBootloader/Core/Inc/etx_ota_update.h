/*
 * etx_ota_update.c
 *
 *  Created on: 26-Jul-2021
 *  Author: EmbeTronicX
 *  Edited By Max Chaplin
 */

#ifndef INC_ETX_OTA_UPDATE_H_
#define INC_ETX_OTA_UPDATE_H_

#include "main.h"
#include "ili9341.h"

#define ETX_APP_FLASH_ADDR    0x08020000 //Application's Flash Address
#define ETX_OTA_DATA_MAX_SIZE ( 1024 )   //Maximum data Size

/*
 * Exception codes for SD Card
 */
typedef enum
{
  ETX_SD_EX_OK       = 0,    // Updated firmware successfully
  ETX_SD_EX_NO_SD    = 1,    // No SD card Found
  ETX_SD_EX_FU_ERR   = 2,    // Failure during Firmware update
  ETX_SD_EX_ERR      = 3,    // Other Failure
}ETX_SD_EX_;

int GetFilenames(char file_names[][13], const char *path);

ETX_SD_EX_ check_update_firmware_SD_card(char* file_name);

#endif /* INC_ETX_OTA_UPDATE_H_ */
