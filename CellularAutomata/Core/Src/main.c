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
#include "XPT2046_touch.h"

#include "touchscreen_button.h"
#include "text_field.h"

#include <stdio.h>
#include <stdbool.h>
#include <complex.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define RES_X 240
#define RES_Y 320
#define CA_TOP 40
#define CA_BOTTOM 195

#define MARGIN_X 10
#define MARGIN_Y 10

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

const uint16_t color_line = COLOR_WHITE;
const uint16_t color_text = COLOR_WHITE;
const uint16_t color_bg = COLOR_BLACK;

Button button_dec1 = {
	.x = 10, .y = 270,
	.width = 50, .height = 40,
	.font = &Font16,
	.is_pressed = false,
	.color_text = color_bg, .color_bg = color_text,
	.color_text_pressed = color_text, .color_bg_pressed = color_bg,
	.text = "-1"
};

Button button_inc1 = {
	.x = 70, .y = 270,
	.width = 50, .height = 40,
	.font = &Font16,
	.is_pressed = false,
	.color_text = color_bg, .color_bg = color_text,
	.color_text_pressed = color_text, .color_bg_pressed = color_bg,
	.text = "+1"
};

Button button_dec10 = {
	.x = 10, .y = 225,
	.width = 50, .height = 40,
	.font = &Font16,
	.is_pressed = false,
	.color_text = color_bg, .color_bg = color_text,
	.color_text_pressed = color_text, .color_bg_pressed = color_bg,
	.text = "-10"
};

Button button_inc10 = {
	.x = 70, .y = 225,
	.width = 50, .height = 40,
	.font = &Font16,
	.is_pressed = false,
	.color_text = color_bg, .color_bg = color_text,
	.color_text_pressed = color_text, .color_bg_pressed = color_bg,
	.text = "+10"
};

Button button_reset = {
	.x = 140, .y = 245,
	.width = 90, .height = 30,
	.font = &Font16,
	.is_pressed = false,
	.color_text = color_bg, .color_bg = color_text,
	.color_text_pressed = color_text, .color_bg_pressed = color_bg,
	.text = "RESET"
};

Button button_pause = {
	.x = 140, .y = 280,
	.width = 90, .height = 30,
	.font = &Font16,
	.is_pressed = false,
	.color_text = color_bg, .color_bg = color_text,
	.color_text_pressed = color_text, .color_bg_pressed = color_bg,
	.text = "PAUSE"
};

Button button_help = {
	.x = 200, .y = 210,
	.width = 30, .height = 30,
	.font = &Font16,
	.is_pressed = false,
	.color_text = color_bg, .color_bg = color_text,
	.color_text_pressed = color_text, .color_bg_pressed = color_bg,
	.text = "?"
};

TextField text_rule = {
	.x = 140, .y = 210,
	.width = 50, .height = 30,
	.font = &Font20,
	.color_text = color_text, .color_bg = color_bg, .color_frame = color_bg,
	.text = "22",
	.horizontal_alignment = ALIGN_CENTER,
	.vertical_alignment = ALIGN_CENTER,
};

TextField text_help = {
	.x = 0, .y = CA_TOP,
	.width = RES_X, .height = CA_BOTTOM - CA_TOP,
	.font = &Font16,
	.color_text = color_text, .color_bg = color_bg, .color_frame = color_bg,
	.text = "In each row, each\r\npixel's value is \r\ndetermined by its \r\nthree neighbors in\r\nthe preceding row.",
	.horizontal_alignment = ALIGN_CENTER,
	.vertical_alignment = ALIGN_CENTER
};

Button* buttons[] = {&button_inc1,  &button_dec1,
					 &button_inc10, &button_dec10,
					 &button_reset, &button_pause,
					 &button_help};

bool debounce_flag = false;

uint8_t rule = 22;

uint16_t line_no = CA_TOP;
bool array[RES_X];
bool is_paused = false;
bool is_help = false;

uint16_t rainbow[255];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

void MakeRainbow(uint16_t rainbow[]);

bool ApplyRule(unsigned char rule, bool bit1, bool bit2, bool bit3);

void ECA(bool * array, unsigned short size, unsigned char rule);

void ArrayToLine(uint16_t graphic_line[], bool array[]);
void MakeLine(bool array[]);
void ResetArray(bool* array, uint16_t size);
void UpdateTextField(TextField* text_rule, uint8_t rule);
void DrawRuleDiagram(uint8_t rule, bool init);
void UpdateRuleDiagram(uint8_t rule);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

	MakeRainbow(rainbow);

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
  printf("%8lu: UART is Ok!\r\n", HAL_GetTick() );
// Init LCD
   lcdBacklightOn();
   lcdInit();
    lcdSetOrientation(LCD_ORIENTATION_PORTRAIT_MIRROR);
    lcdFillRGB(color_bg);
    lcdSetTextFont(&Font16);
    lcdSetTextColor(color_text, color_bg);
    lcdSetCursor(0, lcdGetHeight() - lcdGetTextFont()->Height - 1);
//-----------------------------------------

  ResetArray(array, RES_X);
  HAL_TIM_Base_Start_IT(&htim7);

  for (uint8_t i = 0; i < 7; ++i)
  {
	  Button_Draw(buttons[i]);
  }

  TextField_Draw(&text_rule);
  DrawRuleDiagram(rule, true);
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
int _write(int file, char *ptr, int len)
{
	HAL_UART_Transmit(&huart1, (uint8_t *) ptr, len, HAL_MAX_DELAY);
	return len;
}

uint16_t RGB(uint16_t red, uint16_t green, uint16_t blue)
{
	return red << 11 | green << 5 | blue;
}

void MakeRainbow(uint16_t rainbow[])
{
	uint8_t i = 0;
	uint16_t red = 31;
	uint16_t green = 0;
	uint16_t blue = 0;

	for(; green < 63; ++green)
	{
		rainbow[i++] = RGB(red, green, blue);
	}

	for(; red > 0; --red)
	{
		rainbow[i++] = RGB(red, green, blue);
	}

	for(; blue < 31; ++blue)
	{
		rainbow[i++] = RGB(red, green, blue);
	}

	for(; green > 0; --green)
	{
		rainbow[i++] = RGB(red, green, blue);
	}

	for(; red < 31; ++red)
	{
		rainbow[i++] = RGB(red, green, blue);
	}

	for(; blue > 0; --blue)
	{
		rainbow[i++] = RGB(red, green, blue);
	}
}

bool ApplyRule(unsigned char rule, bool bit1, bool bit2, bool bit3)
{
    unsigned short neighborhood = bit1 << 2 | bit2 << 1 | bit3;

    return rule & (1 << neighborhood);
}

void ECA(bool* array, unsigned short size, unsigned char rule)
{
    unsigned char buffer[] = {array[size-1], array[0]};
    unsigned char first = array[0];
    for (unsigned short i = 0; i < size - 2; ++i)
    {
        array[i] = ApplyRule(rule, buffer[0], buffer[1], array[i+1]);

        /* Rotate buffer */
        buffer[0] = buffer[1];
        buffer[1] = array[i + 1];
    }

    array[size - 2] = ApplyRule(rule, buffer[0], buffer[1], array[size-1]);
    array[size - 1] = ApplyRule(rule, buffer[1], array[size - 1], first);
}

void ArrayToLine(uint16_t graphic_line[], bool array[])
{
	static uint16_t color_index = 0;
	color_index = (color_index + 1) % 500;

	for (uint16_t i = 0; i < RES_X; ++i)
	{
		if (array[i])
		{
			graphic_line[i] = rainbow[color_index/2];
		}
		else
		{
			graphic_line[i] = color_bg;
		}
	}
}

uint16_t NextLine(uint16_t line_no)
{
	++line_no;
	if (line_no >= CA_BOTTOM)
	{
		line_no = CA_TOP;
	}

	return line_no;
}

void MakeLine(bool array[])
{
	static uint16_t graphic_line[RES_X];

	ArrayToLine(graphic_line, array);
	lcdDrawPixelLine(0, line_no, graphic_line, RES_X);
	ECA(array, RES_X, rule);
	line_no = NextLine(line_no);
	lcdDrawHLine(0, RES_X, line_no, COLOR_WHITE);
}

void ResetArray(bool* array, uint16_t size)
{
	lcdDrawHLine(0, RES_X, line_no, color_bg);
	line_no = CA_TOP;
	for (uint16_t i = 0; i < size; ++i)
	{
		array[i] = 0;
	}

	array[size/2] = 1;
}

void UpdateTextField(TextField* text_rule, uint8_t rule)
{
	static char text[4];
	sprintf(text, "%u", rule);
	TextField_SetText(text_rule, text);
	TextField_Draw(text_rule);
}

void DrawRuleDiagram(uint8_t rule, bool init)
{
	static uint16_t margin_x = 8;
	static uint16_t margin_y = 8;
	static uint16_t size = 8;
	static uint16_t gap = 29; /* = (RES_X - 2*margin_x - 3*size)/7 */
	uint16_t pos = RES_X - margin_x - 3*size;

	if (init)
	{
		for (size_t i = 0; i < 8; ++i)
		{
			if (i & 1)
			{
				lcdFillRect(pos + 2*size, margin_y, size-1, size-1, color_line);
			}
			else
			{
				lcdDrawRect(pos + 2*size, margin_y, size, size, color_line);
			}

			if (i & 2)
			{
				lcdFillRect(pos + size, margin_y, size-1, size-1, color_line);
			}
			else
			{
				lcdDrawRect(pos + size, margin_y, size, size, color_line);
			}

			if (i & 4)
			{
				lcdFillRect(pos, margin_y, size-1, size-1, color_line);
			}
			else
			{
				lcdDrawRect(pos, margin_y, size, size, color_line);
			}

			pos -= gap;
		}
	}

	pos = RES_X - margin_x - 2*size;

	for (size_t i = 0; i < 8; ++i)
	{
		if (rule & (1 << i))
		{
			lcdFillRect(pos, margin_y + size, size-1, size-1, color_line);
		}
		else
		{
			lcdFillRect(pos, margin_y + size - 1, size, size, color_bg);
			lcdDrawRect(pos, margin_y + size - 1, size, size, color_line);
		}

		pos -= gap;
	}
}

void HandlePause(bool is_paused)
{
	if (is_paused)
	{
		Button_SetText(&button_pause, "UNPAUSE");
	}
	else
	{
		Button_SetText(&button_pause, "PAUSE");
	}
}

void RemoveHelp()
{
	text_help.color_text = color_bg;
	TextField_Draw(&text_help);
	text_help.color_text = color_text;
	is_help = false;
	is_paused = false;
}

//void DrawHelp()
//{
//	if (help)
//	{
//		is_paused = true;
//		TextField_Draw(&text_help);
//	}
//	else
//	{
//		is_paused = false;
//
//		RemoveHelp();
//
//		line_no = CA_TOP;
//	}
//
//	HandlePause(is_paused);
//}




void  HAL_GPIO_EXTI_Callback (uint16_t GPIO_Pin)
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

			if (Button_IsPressed(&button_inc1, x, y))
			{
				Button_SetState(&button_inc1, true);
				Button_Draw(&button_inc1);

				if (rule < 255)
				{
					++rule;
				}
				UpdateTextField(&text_rule, rule);
				DrawRuleDiagram(rule, false);
			}
			else if (Button_IsPressed(&button_dec1, x, y))
			{
				Button_SetState(&button_dec1, true);
				Button_Draw(&button_dec1);

				if (rule > 0)
				{
					--rule;
				}
				UpdateTextField(&text_rule, rule);
				DrawRuleDiagram(rule, false);
			}
			else if (Button_IsPressed(&button_inc10, x, y))
			{
				Button_SetState(&button_inc10, true);
				Button_Draw(&button_inc10);

				if (rule < 245)
				{
					rule += 10;
				}
				UpdateTextField(&text_rule, rule);
				DrawRuleDiagram(rule, false);
			}
			else if (Button_IsPressed(&button_dec10, x, y))
			{
				Button_SetState(&button_dec10, true);
				Button_Draw(&button_dec10);

				if (rule > 10)
				{
					rule -= 10;
				}
				UpdateTextField(&text_rule, rule);
				DrawRuleDiagram(rule, false);
			}
			else if (Button_IsPressed(&button_reset, x, y))
			{
				Button_SetState(&button_reset, true);
				Button_Draw(&button_reset);

				if (is_help)
				{
					RemoveHelp();
				}

				ResetArray(array, RES_X);

				is_paused = false;
				HandlePause(false);
			}
			else if (Button_IsPressed(&button_pause, x, y))
			{
				Button_SetState(&button_pause, true);
				Button_Draw(&button_pause);

				if (is_help)
				{
					RemoveHelp();
				}
				else
				{
					is_paused = !is_paused;
				}

				HandlePause(is_paused);
			}
			else if (Button_IsPressed(&button_help, x, y))
			{
				if (is_help)
				{
					RemoveHelp();
				}
				else
				{
					is_paused = true;
					is_help = true;
					ResetArray(array, RES_X);
					TextField_Draw(&text_help);
				}
				HandlePause(is_paused);
			}
		}
		else
		{
			/* Handle button release */
			Button_SetState(&button_inc1, false);
			Button_Draw(&button_inc1);
			Button_SetState(&button_dec1, false);
			Button_Draw(&button_dec1);
			Button_SetState(&button_inc10, false);
			Button_Draw(&button_inc10);
			Button_SetState(&button_dec10, false);
			Button_Draw(&button_dec10);
			Button_SetState(&button_reset, false);
			Button_Draw(&button_reset);
			Button_SetState(&button_pause, false);
			Button_Draw(&button_pause);
			Button_SetState(&button_help, false);
			Button_Draw(&button_help);
			return;
		}
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim == &htim7 && !is_paused)
	{
		MakeLine(array);
	}
	else if (htim == &htim6)
	{
		HAL_TIM_Base_Stop_IT(&htim6);
		debounce_flag = false;
	}
}

//----------------------------------------------------


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
