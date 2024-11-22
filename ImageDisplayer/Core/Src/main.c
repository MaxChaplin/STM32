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

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
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
	.width = 120, .height = 40,
	.font = &Font16,
	.color_text = color_text, .color_bg = color_bg, .color_frame = color_text,
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
  printf("\nInitializing ImageDisplayer\r\n\n");

// Init LCD
   lcdBacklightOn();
   lcdInit();
   lcdSetOrientation(LCD_ORIENTATION_PORTRAIT_MIRROR);
   lcdFillRGB(color_bg);

   Button_Draw(&button_next);
   Button_Draw(&button_prev);
   TextField_Draw(&text_filename);

    Mount_SD("/");
    num_images = GetFilenames(image_filenames, "/img", "jpg");

    printf("%d images found:\r\n", num_images);

    for (uint16_t i = 0; i < num_images; ++i)
    {
    	printf("%s\r\n", image_filenames[i]);
    }

    printf("\n");

    SetImage(img_index);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  while (1)
  {
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
//	char buf[18];
//	sprintf(buf, "img/%s", image_filenames[img_index]);

	JPGRESULT res = jpeg_screen_view("/img/", image_filenames[img_index], 0, 0, 240, 280, &iw, &ih);

	printf("Loading image: %s\r\n", image_filenames[img_index]);
	TextField_SetText(&text_filename, image_filenames[img_index]);
	TextField_Draw(&text_filename);

	if (res == JPG_MOUNT_ERROR)
	{
		printf("JPG mount error\r\n");
		return;
	}
	else if (res == JPG_OPEN_ERROR)
	{
		printf("JPG open error\r\n");
		return;
	}
	else if (res == JPG_DECODE_ERROR)
	{
		printf("JPG decode error\r\n");
		return;
	}
	else
	{

		printf("Image loaded. Size: %u x %u\r\n", iw, ih);
	}
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
			if (!XPT2046_TouchGetCoordinates(&x, &y))
			{
				return;
			}

			if (Button_IsPressed(&button_next, x, y))
			{
				Button_SetState(&button_next, true);
				Button_Draw(&button_next);

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
