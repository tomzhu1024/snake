#include "headers.h"
#include "stm32f7xx_hal.h"
#include "Board_GLCD.h"
#include "GLCD_Config.h"
#include "Board_Touch.h"
#include <stdlib.h>



//Global variables
struct TouchArea snakeBTAUp;
struct TouchArea snakeBTADown;
struct TouchArea snakeBTALeft;
struct TouchArea snakeBTARight;
struct TouchArea menuBTANew;
struct TouchArea menuBTALoad;
struct TouchArea menuBRAScores;

int PREV_STATE = MENU;
int STATE = MENU;
int DIRECTION = RIGHT;
int uuid = 1;
int currentGameId = 0;
int historyScores[3] = {0,0,0};

struct Snake* currentSnake = NULL;
struct Food* currentFood = NULL; 
struct GameRecord* gameRecords = NULL;

static unsigned int tickCount = 0;
static unsigned int tickPSC = 4;

extern GLCD_FONT GLCD_Font_16x24;
void SysTick_Handler(void);
static void SystemClock_Config(void);
void TIM7_IRQHandler (void);
void drawSnakeBox (unsigned int);
void drawSnakeBlock (unsigned int, unsigned int, unsigned int);
void drawSnakeButton (unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
int checkButtonPressed (int, int, struct TouchArea *);
void tickHandler (void);
void touchHandler (int, int);



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



int checkButtonPressed (int x, int y, struct TouchArea * ta)
{
	return (ta->startX <= (unsigned)x) && ((unsigned)x <= ta->startX + ta->sizeX) &&
		(ta->startY <= (unsigned)y) && ((unsigned)y <= ta->startY + ta->sizeY);
}

void startTimer (void) {
	// start counting
	TIM7->CR1 |= (1 << 0);
}

void stopTimer (void) {
	// start counting
	TIM7->CR1 &= ~((unsigned)1 << 0);
}

void renderPage(void){
	GLCD_ClearScreen();
	
	switch (STATE){
		case MENU: {
			renderMenuPage();
			break;
		}
		case GAME_START: {
			gameStartPageInitialize();
			break;
		}
		case GAME_END: {
			renderGameEndPage();
			break;
		}
		case GAME_PAUSE: {
			renderGamePausePage();
			break;
		}
		case HISTORY_SCORE: {
			renderHistoryScorePage();
			break;
		}
		case GAME_RECORD : {
			renderGameRecordPage();
			break;
		}
	}
}


void tickHandler (void)
{
	if (tickCount == 0){
		if (STATE == GAME_START){
			gameRender();
		}
	}
	if (++tickCount == tickPSC - 1)
	{
		tickCount = 0;
	}
}



void touchHandler (int x, int y)
{
	switch (STATE){
		case MENU: {
			if (checkButtonPressed(x, y, &menuBTANew) == 1)
			{
				STATE = GAME_START;
				renderPage();
			}
			break;
		}
		case GAME_START :{
			if (checkButtonPressed(x, y, &snakeBTAUp) == 1)
			{
				DIRECTION = 0;
			}
			if (checkButtonPressed(x, y, &snakeBTADown) == 1)
			{
				DIRECTION = 1;
			}
			if (checkButtonPressed(x, y, &snakeBTALeft) == 1)
			{
				DIRECTION = 2;
			}
			if (checkButtonPressed(x, y, &snakeBTARight) == 1)
			{
				DIRECTION = 3;
			}
			break;
		}
	
	
	
	}
	
}

void TIM7_IRQHandler (void)
{
	// reset flag
	TIM7->SR = ~((unsigned) 1 << 0);
	tickHandler();
}
	
int main (void)
{
	
	SystemClock_Config();
	
	// initiate display and touch
	GLCD_Initialize();
	GLCD_SetBackgroundColor(GLCD_COLOR_BLACK);
	GLCD_SetFont(&GLCD_Font_16x24);
	Touch_Initialize();

	// activate TIM7
	RCC->APB1ENR |= (1 << 5);
	// activate RNG
	RCC->AHB2ENR |= (1 << 6);
	// enable RNG
	RNG->CR |= (1 << 2);
	
	// config TIM7 to 20Hz
	TIM7->PSC = 83 - 1;
	TIM7->ARR = 65060 - 1;
	// enable TIM7 update interrupt
	TIM7->DIER |= (1 << 0);
	// unmask TIM7 update interrupt
	NVIC_EnableIRQ(TIM7_IRQn);
	
	// render page for the first time
	renderPage();
	
	// detect touch event
	TOUCH_STATE touchState;
	while (1)
	{
		Touch_GetState(&touchState);
		if (touchState.pressed == 1)
		{
			touchHandler(touchState.x, touchState.y);
		}
	}
}
