/*
 * etx_ota_update.c
 *
 *  Created on: 26-Jul-2021
 *  Author: EmbeTronicX
 *  Edited By Max Chaplin
 */

#include "stdio.h"
#include "etx_ota_update.h"
#include "main.h"
#include <string.h>
#include <stdbool.h>
#include "fatfs.h"

/* Firmware Size that we have received */
static uint32_t ota_fw_received_size;

FRESULT MountSD()
{
	FATFS FatFs;
    FRESULT res;

	// Mount the SD Card.
	res = f_mount(&FatFs, "", 1);
	if (res != FR_OK)
	{
		printf("No SD Card found: (%i)\r\n", res);
		return res;
	}
	printf("SD Card Mounted\r\n");

	return res;
}


bool IsBin(const char filename[])
{
  char* extension = strrchr(filename, '.');

  if (extension != NULL)
  {
	  return (bool) strcmp(extension, ".bin");
  }

  return false;
}

int GetFilenames(char file_names[][13], const char *path)
{
    FRESULT res;
    DIR dir;
    FILINFO fno;

    int num_files = 0;

    res = MountSD();

    if (res != FR_OK)
    {
    	return 0;
    }

    res = f_opendir(&dir, path); /* Open the directory */

    if (res != FR_OK)
    {
    	printf("Failed to open \"%s\". (%u)\r\n", path, res);
    	return -1;
    }

	num_files = 0;

	while (true)
	{
		res = f_readdir(&dir, &fno);                     /* Read a directory item */
		if (res != FR_OK || fno.fname[0] == 0)
		{
			break;    /* Error or end of dir */
		}
		if (!(fno.fattrib & AM_DIR) && IsBin(fno.fname)) /* Checking that it's a bin file */
		{
			strcpy(file_names[num_files], fno.fname);
			++num_files;
		}
	}

	f_closedir(&dir);
    return num_files;
}

/**
  * @brief Write data to the Slot
  * @param data data to be written
  * @param data_len data length
  * @is_first_block true - if this is first block, false - not first block
  * @retval HAL_StatusTypeDef
  */
static HAL_StatusTypeDef write_data_to_slot(uint8_t *data, uint16_t data_len, bool is_first_block)
{
  HAL_StatusTypeDef ret;
  // open flash to write
	  ret = HAL_FLASH_Unlock();
    if(ret != HAL_OK)
    {
    	return ret;
    }

    //  --------------- Erasing the Slot ---------------
    // No need to erase every time. Erase only the first time.
    if(is_first_block)
    {
      printf("Erasing internal Flash...\r\n");
      //Erase the Flash
      FLASH_EraseInitTypeDef EraseInitStruct;
      uint32_t SectorError;

      EraseInitStruct.TypeErase     = FLASH_TYPEERASE_SECTORS;
      EraseInitStruct.Sector        = FLASH_SECTOR_5;
      EraseInitStruct.NbSectors     = 7;                    //erase sectors
      EraseInitStruct.VoltageRange  = FLASH_VOLTAGE_RANGE_3;

      ret = HAL_FLASHEx_Erase( &EraseInitStruct, &SectorError );
      if(ret != HAL_OK)
      {
    	printf("Flash erase error.\r\n");
        return ret;
      }
    }
   //lcdPrintf("Flash Erase no 1 block\r\n");

    //  --------------- write  data  to slot ---------------
     for(int i = 0; i < data_len; i++ )
    {
		ret = HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE,
							  (ETX_APP_FLASH_ADDR + ota_fw_received_size),
							  data[i]);
		if(ret == HAL_OK)
		{
		// Update the data count for place to next addr.
			ota_fw_received_size += 1;
		}
		else
		{
			printf("Flash write error.\r\n");
			return ret;
		}
    } // end writing data to slot


    if(ret != HAL_OK)
    {
    	printf("Flash erase not OK.\r\n");
    	return ret;
    }

    ret = HAL_FLASH_Lock();
    if(ret != HAL_OK)
    {
    	printf("HAL_FLASH_Lock not OK.\r\n");
    }

  return ret;
}

/**
  * @brief Check the SD for Firmware update
  * @param none
  * @retval ETX_SD_EX_
  */
ETX_SD_EX_ check_update_firmware_SD_card(char* file_name)
{
	ETX_SD_EX_  ret = ETX_SD_EX_ERR;
	FATFS       FatFs;                // Fatfs handle
	FIL         fil;                  // File handle
	FRESULT     fres;                 // Result after operations

	do
	{
		// Mount the SD Card.
		fres = f_mount(&FatFs, "", 1);    //1=mount now
		if (fres != FR_OK)
		{
			printf("No SD Card found: (%i)\r\n", fres);
			ret = ETX_SD_EX_NO_SD;
			break;
		}
		lcdPrintf("SD Card Mounted\r\n");

		fres = f_open(&fil, file_name, FA_WRITE | FA_READ | FA_OPEN_EXISTING);
		if(fres != FR_OK)
		{
			printf("No Firmware found: (%i)\r\n", fres);
			break;
		}

		UINT bytesRead;
		BYTE readBuf[ETX_OTA_DATA_MAX_SIZE];
		UINT fw_size = f_size(&fil);
		UINT size;

		printf("Firmware found.\r\nSize: %d KB\r\n", fw_size >> 10);

		bool is_first_block = true;
		// --------------------------------------- Copy to slot
		ota_fw_received_size = 0;

		for(uint32_t i = 0; i < fw_size; i += size)
		{
			if((fw_size - i) >= ETX_OTA_DATA_MAX_SIZE) // correct size value
			{
				size = ETX_OTA_DATA_MAX_SIZE;
			}
			else
			{
				size = fw_size - i;
			}
			//clear the buffer
			memset(readBuf, 0, ETX_OTA_DATA_MAX_SIZE);

			fres = f_read(&fil, readBuf, size, &bytesRead);
			if((fres != FR_OK) || (size != bytesRead))
			{
				printf("FW Read Error : (%i)\r\n", fres);
				ret = ETX_SD_EX_FU_ERR;
				break;
			}

			/* write the chunk to Slot location */
			HAL_StatusTypeDef ex;
			ex = write_data_to_slot( readBuf, size, is_first_block );
			if(ex != HAL_OK)
			{
				printf("Flash write Error : (%d)\r\n", ex);
				ret = ETX_SD_EX_FU_ERR;
				break;
			}

			is_first_block = false;
		}

		if(ret == ETX_SD_EX_FU_ERR)
		{
			f_close(&fil);
			break;
		}

		//close your file
		f_close(&fil);

		//update the status okay
		ret = ETX_SD_EX_OK;

	} while(false);

	if(ret != ETX_SD_EX_NO_SD)
	{
		/* We're done, so de-mount the drive */
		f_mount(NULL, "", 0);
	}

  return ret;
}
