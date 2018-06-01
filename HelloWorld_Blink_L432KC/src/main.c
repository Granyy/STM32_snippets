/******************************************************************************/
/*        @TITRE : main.c       	                                          */
/*      @VERSION : 0                                                          */
/*     @CREATION : fev 23, 2018                                               */
/* @MODIFICATION :                                                            */
/*      @AUTEURS : Leo GRANIER                                                */
/******************************************************************************/

#include "main.h"
#include "stdio.h"
#include <string.h>

#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)

static GPIO_InitTypeDef  GPIO_InitStruct;
static UART_HandleTypeDef serialUART;
static void SystemClock_Config(void);

void error_handler() {
	while(1);
}


PUTCHAR_PROTOTYPE
{
  HAL_UART_Transmit(&serialUART, (uint8_t *)&ch, 1, 0xFFFF);
  return ch;
}


int main(void)
{

	HAL_Init();

    SystemClock_Config();
  
	LED3_GPIO_CLK_ENABLE();
	GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull  = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Pin = LED3_PIN;
	HAL_GPIO_Init(LED3_GPIO_PORT, &GPIO_InitStruct);

	__GPIOA_CLK_ENABLE();
	__USART2_CLK_ENABLE();

	serialUART.Instance          = USART2;
	serialUART.Init.BaudRate     = 9600;
	serialUART.Init.WordLength   = UART_WORDLENGTH_8B;
	serialUART.Init.StopBits     = UART_STOPBITS_1;
	serialUART.Init.Parity       = UART_PARITY_NONE;
	serialUART.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
	serialUART.Init.Mode         = UART_MODE_TX_RX;
	serialUART.Init.OverSampling = UART_OVERSAMPLING_16;
	serialUART.Init.OneBitSampling = UART_ONEBIT_SAMPLING_DISABLED;
	serialUART.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	if(HAL_UART_DeInit(&serialUART) != HAL_OK)
	{
	  error_handler();
	}
	if(HAL_UART_Init(&serialUART) != HAL_OK)
	{
		error_handler();
	}


	int cnt = 0;
	while (1)
	{
	HAL_GPIO_TogglePin(LED3_GPIO_PORT, LED3_PIN);
	printf("Hello world %i\n", cnt++);
	HAL_Delay(1000);
	}
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follows :
  *            System Clock source            = PLL (MSI)
  *            SYSCLK(Hz)                     = 80000000
  *            HCLK(Hz)                       = 80000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 1
  *            MSI Frequency(Hz)              = 4000000
  *            PLL_M                          = 1
  *            PLL_N                          = 40
  *            PLL_R                          = 2
  *            PLL_P                          = 7
  *            PLL_Q                          = 4
  *            Flash Latency(WS)              = 4
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};

  /* MSI is enabled after System reset, activate PLL with MSI as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLP = 7;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;  
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }
}
