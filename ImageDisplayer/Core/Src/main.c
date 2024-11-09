/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "fatfs.h"
#include "i2c.h"
#include "libjpeg.h"
#include "sdio.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "fsmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ili9341.h"
#include "XPT2046_touch.h"
#include "jpeg_view.h"
#include "File_Handling.h"
#include "w25qxx.h"
#include "pic07.h"

#include "touchscreen_button.h"
#include "text_field.h"

#include <stdio.h>
#include <stdbool.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
//unsigned long testCircles(uint8_t radius, uint16_t color);


/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint16_t part[44032];

FATFS myFatFS;
FIL myFile;
UINT myBytes;

UINT iw =640, ih=480;

const uint16_t color_line = COLOR_WHITE;
const uint16_t color_text = COLOR_WHITE;
const uint16_t color_bg = COLOR_BLACK;

Button button_prev = {
	.x = 10, .y = 270,
	.width = 40, .height = 40,
	.font = &Font16,
	.is_pressed = false,
	.color_text = color_bg, .color_bg = color_text,
	.color_text_pressed = color_text, .color_bg_pressed = color_bg,
	.text = "<-"
};

Button button_next = {
	.x = 60, .y = 270,
	.width = 40, .height = 40,
	.font = &Font16,
	.is_pressed = false,
	.color_text = color_bg, .color_bg = color_text,
	.color_text_pressed = color_text, .color_bg_pressed = color_bg,
	.text = "->"
};

TextField text_filename = {
	.x = 110, .y = 270,
	.width = 50, .height = 30,
	.font = &Font16,
	.color_text = color_text, .color_bg = color_bg, .color_frame = color_bg,
	.text = "",
	.horizontal_alignment = ALIGN_BEGIN,
	.vertical_alignment = ALIGN_CENTER,
};

bool debounce_flag = false;

uint16_t img_index = 0;
uint16_t num_images = 0;
char image_filenames[16][13];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

void SetImage(uint16_t img_index);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

int _write(int file, char *ptr, int len)
{
	HAL_UART_Transmit(&huart1, (uint8_t *) ptr, len, HAL_MAX_DELAY);
	return len;
}

////-----------------------------------------
// void showPic(uint8_t pic_nr)
// {
//     uint32_t index=0;
//     uint32_t laki_count = 1024*150/2;
//       while (index<= laki_count)
//     {
//         LCD_DataWrite(laki[index]);
//         index++;
//     }
// }
// //-----------------------------------------
// void readPicFromFlash(uint8_t pic_nr){
//
// #define rest_pic (150*1024 - 131072)
// #define rest0  rest_pic / 2
// #define  CCM_ADDRESS   0x10000000UL
//
//     uint32_t i, blk_nr;
//     uint16_t * CCM_ADD  = (uint16_t *) CCM_ADDRESS;
//
//      blk_nr = pic_nr*3;
//
//     W25qxx_ReadBlock((uint8_t*)CCM_ADD, blk_nr+0, 0,w25qxx.BlockSize);
//     W25qxx_ReadBlock((uint8_t*)&part[0], blk_nr+1, 0,w25qxx.BlockSize);
//     W25qxx_ReadBlock((uint8_t*)&part[32768], blk_nr+2, 0, rest_pic);
//
//     for(i=0;i<32768;i++)
//     {
//         LCD_DataWrite(CCM_ADD[i]);
//     }
//
//     for(i=0;i<44032;i++)
//     {
//         LCD_DataWrite(part[i]);
//     }
// }
// // ---------------------------------------------------
// void savePicToFlash(uint8_t pic_nr){
//     uint8_t block;
//     uint32_t index=0;
//     uint32_t laki_size =  sizeof(laki);
//     block = pic_nr * 3;
//
//     W25qxx_EraseBlock(block);
//     W25qxx_EraseBlock(block+1);
//     W25qxx_EraseBlock(block+2);
//
//     W25qxx_WriteBlock((uint8_t*)&laki[index], block, 0,w25qxx.BlockSize);index+=65536/2;
//     W25qxx_WriteBlock((uint8_t*)&laki[index], block+1, 0,w25qxx.BlockSize);index+=65536/2;
//     W25qxx_WriteBlock((uint8_t*)&laki[index], block+2, 0,laki_size-(65536));index+=65536/2;
// }
  //-----------------------------------------


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_FSMC_Init();
  MX_USART1_UART_Init();
  MX_SPI2_Init();
  MX_I2C1_Init();
  MX_I2C2_Init();
  MX_SPI1_Init();
  MX_SDIO_SD_Init();
  MX_FATFS_Init();
  MX_LIBJPEG_Init();
  MX_TIM6_Init();
  /* USER CODE BEGIN 2 */

  printf("\x1b[2J\x1b[;H");
  printf("*-------------------------*\r\n");
  printf("* UART is OK  *\r\n");
  printf("*-------------------------*\r\n");


// Init LCD
   lcdBacklightOn();
   lcdInit();
   lcdSetOrientation(LCD_ORIENTATION_PORTRAIT_MIRROR);
   lcdFillRGB(color_bg);


    Mount_SD("/");
//    Scan_SD("/");
//    Check_SD_Space();
    num_files = GetFilenames(file_names, "/img");

   // Create_File("FILE1.TXT");
   // Create_File("FILE2.TXT");
    Unmount_SD("/");


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

//    showPic(5);
//
//            lcdPrintf("INIT OF SPI FLASH W25Q16 :");
//            if (W25qxx_Init()){
//                lcdPrintf("OK\n");
//                lcdPrintf("CHIP ID :%d\n",w25qxx.ID);
//                lcdPrintf("SECTOR COUNT :%d\n",w25qxx.SectorCount);
//                lcdPrintf("SECTOR SIZE :%d\n",w25qxx.SectorSize);
//                lcdPrintf("BLOCK COUNT :%d\n",w25qxx.BlockCount);
//                lcdPrintf("BLOCK SIZE :%d\n",w25qxx.BlockSize);
//                lcdPrintf("PAGE COUNT :%d\n",w25qxx.PageCount);
//                lcdPrintf("PAGE SIZE :%d\n",w25qxx.PageSize);
//
//                savePicToFlash(1);
//                readPicFromFlash(1);
//            }
//            else {
//                lcdPrintf("ERROR\n");
//                while(1);;
//            }

  while (1)
  {
//
//	  jpeg_screen_view("", "11.jpg", 0, 0, &iw, &ih);
//	  HAL_Delay(2000);
//	  jpeg_screen_view("0:/", "12.jpg", 0, 0, &iw, &ih);
//	  	  HAL_Delay(2000);
//	  jpeg_screen_view("", "13.jpg", 0, 0, &iw, &ih);
//	  HAL_Delay(2000);

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void SetImage(uint16_t img_index)
{
	char buf[18];
	sprintf(buf, "img/%s", image_filenames[img_index]);
	jpeg_screen_view("", buf, 0, 0, &iw, &ih);
}

void  HAL_GPIO_EXTI_Callback ( uint16_t GPIO_Pin)
{
	if (GPIO_Pin == T_PEN_Pin)
	{
		if (XPT2046_TouchPressed())
		{
			if (debounce_flag)
			{
				return;
			}
			debounce_flag = true;
			HAL_TIM_Base_Start_IT(&htim6); /* Start debouncing timer */

			/* Handle button press */
			uint16_t x = 0, y = 0;
			if (!XPT2046_TouchGetCoordinates(&y, &x))
			{
				return;
			}

			y = lcdGetHeight() - y; /* The display is flipped */

			if (Button_IsPressed(&button_next, x, y))
			{
				Button_SetState(&button_prev, true);
				Button_Draw(&button_prev);

				img_index = (img_index + 1) % num_images;
				SetImage(img_index);
			}
			else if (Button_IsPressed(&button_prev, x, y))
			{
				Button_SetState(&button_prev, true);
				Button_Draw(&button_prev);

				img_index = (img_index - 1) % num_images;
				SetImage(img_index);
			}
		}
		else
		{
			/* Handle button release */
			Button_SetState(&button_next, false);
			Button_Draw(&button_next);
			Button_SetState(&button_prev, false);
			Button_Draw(&button_prev);
			return;
		}
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim == &htim6)
	{
		HAL_TIM_Base_Stop_IT(&htim6);
		debounce_flag = false;
	}
}


/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
