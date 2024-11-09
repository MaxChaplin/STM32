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
#include "i2c.h"
#include "usart.h"
#include "gpio.h"
#include "fsmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ili9341.h"
#include "mpu6050.h"
#include "wire2.h"
#include "wire3.h"
#include "wireframe_render_ili9341.h"

#include <stdio.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

void CalculateAngles(double* angle_dX, double* angle_dY, MPU6050_t* MPU6050);
void UpdateText(double angle_dX, double angle_dY, MPU6050_t * MPU6050);
void UpdateFrame(double angle_dX, double angle_dY, MPU6050_t* MPU6050);

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

DisplaySettings ds = {.center_x = 120, .center_y = 200, .fov = 1.8};

/* Geometry */
Vec3 cube_pos = {0, 0, 0.7};
const double size = 0.45;
const double atom_size = 0.02;

/* Movement constants */
const double smooth_factor = 0.4;
const double sensitivity = 0.025;
const double angle_limit = 270;

/* Colors */
uint16_t bg_color = COLOR_BLACK;
uint16_t text_color = COLOR_WHITE;
uint16_t cube_color = COLOR_GREEN;
uint16_t octahedron_color = COLOR_YELLOW;
uint16_t atom_color = COLOR_RED;

Wire3 *cube, *octahedron;
const uint8_t num_atoms = 14;
Bubble atoms[14];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

void GyroWire3(Wire3 * wire3, double angle_x, double angle_y, Vec3 pivot);

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
	ds.res_x = lcdGetWidth();
	ds.res_y = lcdGetHeight();
	cube = Wire3_MakeCube(size);
	octahedron = Wire3_MakeOctahedron(size);

	/* Set initial positions */
	Wire3_Move(cube, &cube_pos);
	Wire3_Move(octahedron, &cube_pos);


	uint8_t i = 0;

	for (i = 0; i < cube->num_vertices; ++i)
	{
		atoms[i].pos = cube->vertices[i];
		atoms[i].r = atom_size;
	}

	for (i = 0; i < octahedron->num_vertices; ++i)
	{
		atoms[cube->num_vertices + i].pos = octahedron->vertices[i];
		atoms[cube->num_vertices + i].r = atom_size;
	}

	double angle_dX = 0, angle_dY = 0;

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  MPU6050_t MPU6050;
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_FSMC_Init();
  MX_USART1_UART_Init();
  MX_I2C1_Init();
  MX_I2C2_Init();
  /* USER CODE BEGIN 2 */
  printf("%8lu: UART is Ok!\r\n", HAL_GetTick() );

    /* Init LCD */
    lcdBacklightOn();
    lcdInit();
    lcdSetOrientation(LCD_ORIENTATION_PORTRAIT_MIRROR);
    lcdSetTextFont(&Font16);
    lcdSetTextColor(text_color, bg_color);
    lcdFillRGB(bg_color);

    unsigned short Text_Y = 0;
    const unsigned short text_disp = 5;

    /* Static Drawing */
    lcdDrawHLine(1, 239, 70, COLOR_WHITE);

	Text_Y = 2;
	lcdSetCursor(text_disp, Text_Y);
	lcdPrintf("AngleX:");

	Text_Y += lcdGetTextFont()->Height;
	lcdSetCursor(text_disp, Text_Y);
	lcdPrintf("AngleY:");

	Text_Y += lcdGetTextFont()->Height;
	lcdSetCursor(text_disp, Text_Y);
	lcdPrintf("dAngleX:");

	Text_Y += lcdGetTextFont()->Height;
	lcdSetCursor(text_disp, Text_Y);
	lcdPrintf("dAngleY:");

    /* Init MPU6050 */
    while (MPU6050_Init(&hi2c1) == 1);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	/* Read data */
	MPU6050_Read_All(&hi2c1, &MPU6050);
	HAL_Delay(1);

	CalculateAngles(&angle_dX, &angle_dY, &MPU6050);
	UpdateText(angle_dX, angle_dY, &MPU6050);
	UpdateFrame(angle_dX, angle_dY, &MPU6050);

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

void CalculateAngles(double* angle_dX, double* angle_dY, MPU6050_t* MPU6050)
{
	static double angle_dX_prev = 0;
	static double angle_dY_prev = 0;
	static double angle_X_prev = 0;
	static double angle_Y_prev = 0;

	/* Calculate angles */
	*angle_dX = MPU6050->KalmanAngleX - angle_X_prev;
	*angle_dY = MPU6050->KalmanAngleY - angle_Y_prev;

	/* Handle angle overflow */
	if (*angle_dX < -angle_limit)
	{
		*angle_dX += 360;
	}

	if (*angle_dX > angle_limit)
	{
		*angle_dX -= 360;
	}

	if (*angle_dY < -angle_limit)
	{
		*angle_dY += 360;
	}

	if (*angle_dY > angle_limit)
	{
		*angle_dY -= 360;
	}

	/* Smooth out the angles */
	*angle_dX = *angle_dX * (1 - smooth_factor) + angle_dX_prev * smooth_factor;
	*angle_dY = *angle_dY * (1 - smooth_factor) + angle_dY_prev * smooth_factor;

	angle_X_prev = MPU6050->KalmanAngleX;
	angle_Y_prev = MPU6050->KalmanAngleY;
	angle_dX_prev = *angle_dX;
	angle_dY_prev = *angle_dY;
}

void UpdateText(double angle_dX, double angle_dY, MPU6050_t * MPU6050)
{
    static unsigned short text_disp = 5;
    unsigned short digit_disp = text_disp + lcdGetTextFont()->Width * 10;

	/* Draw text */
	uint16_t Text_Y = 2;
	lcdSetCursor(digit_disp, Text_Y);
	lcdPrintf("% 6.1f", MPU6050->KalmanAngleX);

	Text_Y += lcdGetTextFont()->Height;
	lcdSetCursor(digit_disp, Text_Y);
	lcdPrintf("% 6.1f", MPU6050->KalmanAngleY);

	Text_Y += lcdGetTextFont()->Height;
	lcdSetCursor(digit_disp, Text_Y);
	lcdPrintf("% 6.1f", angle_dX);

	Text_Y += lcdGetTextFont()->Height;
	lcdSetCursor(digit_disp, Text_Y);
	lcdPrintf("% 6.1f", angle_dY);

	printf("AngleX = %.1f\r\nAngleY = %.1f\r\ndAngleX = %.1f\r\ndAngleX =%.1f\r\n",
			MPU6050->KalmanAngleX,
			MPU6050->KalmanAngleY,
			angle_dX,
			angle_dY);
	printf("\x1b[1F\x1b[1F\x1b[1F\x1b[1F");
}

void UpdateFrame(double angle_dX, double angle_dY, MPU6050_t * MPU6050)
{
	uint8_t i = 0;

	/* Delete old graphics */
	for (i = 0; i < num_atoms; ++i)
	{
		RenderBubble(atoms + i, bg_color, &ds);
	}
	RenderWire3(cube, bg_color, &ds);
	RenderWire3(octahedron, bg_color, &ds);

	/* Rotate solids */
	GyroWire3(cube, angle_dX, angle_dY, cube_pos);
	GyroWire3(octahedron, angle_dX, angle_dY, cube_pos);

	/* Update atom positions */
	for (i = 0; i < cube->num_vertices; ++i)
	{
		atoms[i].pos = cube->vertices[i];
	}

	for (i = 0; i < octahedron->num_vertices; ++i)
	{
		atoms[cube->num_vertices + i].pos = octahedron->vertices[i];
	}

	/* Render */
	for (i = 0; i < num_atoms; ++i)
	{
		RenderBubble(atoms + i, atom_color, &ds);
	}

	RenderWire3(cube, cube_color, &ds);
	RenderWire3(octahedron, octahedron_color, &ds);
}

void GyroWire3(Wire3 * wire3, double angle_x, double angle_y, Vec3 pivot)
{
	Trans3 rot_mat_x = Trans3_RotX(angle_x * sensitivity, &pivot);
	Wire3_Transform(wire3, &rot_mat_x);

	Trans3 rot_mat_y = Trans3_RotY(-angle_y * sensitivity, &pivot);
	Wire3_Transform(wire3, &rot_mat_y);
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
