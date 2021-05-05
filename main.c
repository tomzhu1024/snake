#include "stm32f7xx_hal.h"
#include "Board_GLCD.h"
#include "GLCD_Config.h"
#include "Board_Touch.h"
#include <stdlib.h>

// import fonts
extern GLCD_FONT GLCD_Font_16x24;
// overclock
void SysTick_Handler(void);
static void SystemClock_Config(void);
// interrupt handler
void TIM7_IRQHandler (void);

void SysTick_Handler (void)
{
    HAL_IncTick();
}

static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  HAL_StatusTypeDef ret = HAL_OK;

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 432;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 9;

  ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
  if(ret != HAL_OK)
  {
    while (1);
  }

  /* Activate the OverDrive to reach the 216 MHz Frequency */
  ret = HAL_PWREx_EnableOverDrive();
  if(ret != HAL_OK)
  {
    while (1);
  }
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7);
  if(ret != HAL_OK)
  {
    while (1);
  }
}

void TIM7_IRQHandler (void)
{
	// reset flag
	TIM7->SR = ~((unsigned) 1 << 0);
}
	
int main (void)
{
	SystemClock_Config();
	
	// activate TIM7
	RCC->APB1ENR |= (1 << 5);
	// config TIM7 to 20Hz
	TIM7->PSC = 108 - 1;
	TIM7->ARR = 50000 - 1;
	// enable TIM7 update interrupt
	TIM7->DIER |= (1 << 0);
	// unmask TIM7 update interrupt
	NVIC_EnableIRQ(TIM7_IRQn);
	
	GLCD_Initialize();
	GLCD_SetBackgroundColor(GLCD_COLOR_BLUE);
	GLCD_SetForegroundColor(GLCD_COLOR_WHITE);
	GLCD_SetFont(&GLCD_Font_16x24);
	GLCD_ClearScreen();
	
	return 0;
}
