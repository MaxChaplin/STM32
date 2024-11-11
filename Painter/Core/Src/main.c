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
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "fsmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "ili9341.h"
#include "touchscreen_button.h"
#include "text_field.h"
#include "XPT2046_touch.h"

#include <stdio.h>
#include <stdbool.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define CANVAS_BOTTOM 215

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

const uint16_t color_line = COLOR_WHITE;
const uint16_t color_text = COLOR_WHITE;
const uint16_t color_bg = COLOR_BLACK;

const uint16_t brush_colors[] = {COLOR_RED, COLOR_ORANGE, COLOR_YELLOW,
								 COLOR_GREEN, COLOR_OLIVE, COLOR_CYAN,
								 COLOR_BLUE, COLOR_NAVY, COLOR_PURPLE,
								 COLOR_MAROON, COLOR_WHITE, COLOR_BLACK};

const uint16_t num_colors = sizeof(brush_colors) / sizeof(uint16_t);
uint16_t color_i = 0;

const uint16_t brush_radii[] = {1, 2, 3, 4, 6, 8, 10, 12};

const uint16_t num_radii = sizeof(brush_radii) / sizeof(uint16_t);
uint16_t rad_i = 0;

Button button_color_prev = {
	.x = 80, .y = 270,
	.width = 40, .height = 40,
	.font = &Font20,
	.is_pressed = false,
	.color_text = color_bg, .color_bg = color_text,
	.color_text_pressed = color_text, .color_bg_pressed = color_bg,
	.text = "<"
};

Button button_color_next = {
	.x = 130, .y = 270,
	.width = 40, .height = 40,
	.font = &Font20,
	.is_pressed = false,
	.color_text = color_bg, .color_bg = color_text,
	.color_text_pressed = color_text, .color_bg_pressed = color_bg,
	.text = ">"
};

Button button_size_dec = {
	.x = 80, .y = 220,
	.width = 40, .height = 40,
	.font = &Font20,
	.is_pressed = false,
	.color_text = color_bg, .color_bg = color_text,
	.color_text_pressed = color_text, .color_bg_pressed = color_bg,
	.text = "-"
};

Button button_size_inc = {
	.x = 130, .y = 220,
	.width = 40, .height = 40,
	.font = &Font20,
	.is_pressed = false,
	.color_text = color_bg, .color_bg = color_text,
	.color_text_pressed = color_text, .color_bg_pressed = color_bg,
	.text = "+"
};

Button button_clear = {
	.x = 180, .y = 220,
	.width = 50, .height = 40,
	.font = &Font16,
	.is_pressed = false,
	.color_text = color_bg, .color_bg = color_text,
	.color_text_pressed = color_text, .color_bg_pressed = color_bg,
	.text = "CLR"
};

TextField text_color = {
	.x = 10, .y = 275,
	.width = 60, .height = 30,
	.font = &Font16,
	.color_text = color_text, .color_bg = color_bg, .color_frame = color_bg,
	.text = "COLOR",
	.horizontal_alignment = ALIGN_END,
	.vertical_alignment = ALIGN_CENTER,
};

TextField text_size = {
	.x = 10, .y = 225,
	.width = 60, .height = 30,
	.font = &Font16,
	.color_text = color_text, .color_bg = color_bg, .color_frame = color_bg,
	.text = "SIZE",
	.horizontal_alignment = ALIGN_END,
	.vertical_alignment = ALIGN_CENTER,
};

bool debounce_flag = false;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

void DrawSample();
void HandleSize(int change);

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
  MX_FSMC_Init();
  MX_USART1_UART_Init();
  MX_SPI2_Init();
  MX_SPI1_Init();
  MX_TIM6_Init();
  MX_TIM7_Init();
  /* USER CODE BEGIN 2 */

  printf("\nPainter initialized\r\n\n");

    // Init LCD
	lcdBacklightOn();
	lcdInit();
	lcdSetOrientation(LCD_ORIENTATION_PORTRAIT_MIRROR);
	lcdFillRGB(color_bg);
	lcdSetTextFont(&Font16);
	lcdSetTextColor(color_text, color_bg);

  Button_Draw(&button_color_next);
  Button_Draw(&button_color_prev);
  Button_Draw(&button_size_dec);
  Button_Draw(&button_size_inc);
  Button_Draw(&button_clear);
  TextField_Draw(&text_color);
  TextField_Draw(&text_size);
  lcdDrawRect(180, 270, 50, 40, color_line);
  DrawSample();

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
  RCC_OscInitStruct.PLL.PLLQ = 4;
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

void DrawSample()
{
	lcdFillCircle(205, 290, brush_radii[num_radii - 1], color_bg);
	lcdFillCircle(205, 290, brush_radii[rad_i], brush_colors[color_i]);
}

void HandleSize(int change)
{
	if (-change > rad_i) /* Beginning boundary */
	{
		rad_i = 0;
	}
	else if (rad_i + change >= num_radii) /* End boundary */
	{
		rad_i = num_radii - 1;
	}

	else
	{
		rad_i += change;
	}
}

void  HAL_GPIO_EXTI_Callback ( uint16_t GPIO_Pin)
{
	if (GPIO_Pin == T_PEN_Pin)
	{
		if (XPT2046_TouchPressed())
		{
			HAL_TIM_Base_Start_IT(&htim6); /* Start debouncing timer */

			/* Handle button press */
			uint16_t x = 0, y = 0;
			if (!XPT2046_TouchGetCoordinates(&y, &x))
			{
				return;
			}
			y = lcdGetHeight() - y; /* The display is flipped */

			if (y < CANVAS_BOTTOM - brush_radii[rad_i]
				&& y > brush_radii[rad_i])
			{
				/* Draw on the canvas */
				lcdFillCircle(x, y, brush_radii[rad_i], brush_colors[color_i]);
				return;
			}

			if (debounce_flag)
			{
				return;
			}
			debounce_flag = true;

			if (Button_IsPressed(&button_color_next, x, y))
			{
				Button_SetState(&button_color_next, true);
				Button_Draw(&button_color_next);

				color_i = (color_i + 1) % num_colors;
				DrawSample();
			}
			else if (Button_IsPressed(&button_color_prev, x, y))
			{
				Button_SetState(&button_color_prev, true);
				Button_Draw(&button_color_prev);

				color_i = (color_i - 1) % num_colors;
				DrawSample();
			}
			else if (Button_IsPressed(&button_size_inc, x, y))
			{
				Button_SetState(&button_size_inc, true);
				Button_Draw(&button_size_inc);

				HandleSize(1);
				DrawSample();
			}
			else if (Button_IsPressed(&button_size_dec, x, y))
			{
				Button_SetState(&button_size_dec, true);
				Button_Draw(&button_size_dec);

				HandleSize(-1);
				DrawSample();
			}
			else if (Button_IsPressed(&button_clear, x, y))
			{
				Button_SetState(&button_clear, true);
				Button_Draw(&button_clear);

				lcdFillRect(0, 0, lcdGetWidth(), CANVAS_BOTTOM, color_bg);
			}
		}
		else
		{
			/* Handle button release */
			Button_SetState(&button_color_next, false);
			Button_Draw(&button_color_next);
			Button_SetState(&button_color_prev, false);
			Button_Draw(&button_color_prev);
			Button_SetState(&button_size_inc, false);
			Button_Draw(&button_size_inc);
			Button_SetState(&button_size_dec, false);
			Button_Draw(&button_size_dec);
			Button_SetState(&button_clear, false);
			Button_Draw(&button_clear);
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
