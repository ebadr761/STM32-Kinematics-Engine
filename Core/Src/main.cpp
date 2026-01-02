/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.cpp
  * @brief          : Main program body (C++ Version)
  * @author			: EBAD UR REHMAN
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "PhysicsEngine.h" // Your new C++ Class
#include <stdio.h>         // Required for sprintf()

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;

// --- INSTANTIATE PHYSICS ENGINE ---
PhysicsEngine myGymTracker;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_USART2_UART_Init(void);
// static void MX_GPIO_Init(void);

/* Custom UART Print Function */
void UART_Print(char *msg) {
  while (*msg) {
    // Wait for Transmit Data Register (TDR) to be empty (TXE bit)
    while (!(USART2->SR & (1 << 7)));

    // Write character to Data Register
    USART2->DR = *msg;

    msg++;
  }
}

extern "C" int main(void)
{
  HAL_Init();
  SystemClock_Config();

  /* Initialize UART (Let HAL handle baud rate math) */
  MX_USART2_UART_Init();

  /* USER CODE BEGIN 2 */
  // --- BARE METAL GPIO SETUP ---

  // 1. Enable Clock for GPIOA
  RCC->AHB1ENR |= (1 << 0);

  // 2. Configure PA5 as Output (01)
  GPIOA->MODER &= ~(3 << 10);
  GPIOA->MODER |= (1 << 10);

  /* USER CODE END 2 */

  /* Infinite loop */
  while (1)
  {
    // --- PHYSICS SIMULATION ---

    // 1. Fake Sensor Data (Simulating a lift)
    // In Phase 3, we will replace this line with: float accel = MPU6050_Read();
    float fake_acceleration = 0.5f; // Constant push of 0.5 m/s^2
    float time_step = 0.1f;         // Assume 0.1s passed

    // 2. Run the Math (C++ Class)
    float current_speed = myGymTracker.update(fake_acceleration, time_step);

    // 3. Format the Message
    char buffer[64]; // Temporary container for text
    sprintf(buffer, "Status: Lifting... V: %.2f m/s\r\n", current_speed);

    // 4. Send to Laptop
    UART_Print(buffer);

    // 5. Toggle Green LED (Visual Heartbeat)
    GPIOA->ODR ^= (1 << 5);

    // 6. Slow Delay (So you can read the text)
    // Increased to 5,000,000 so it doesn't flood the console
    for(volatile int i = 0; i < 5000000; i++);
  }
}

/**
  * @brief System Clock Configuration
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

static void MX_USART2_UART_Init(void)
{
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
}

void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}
