#include "headers.h"

#define TICK_PER_RENDER 4
#define BLOCK_SIZE 10
#define BLOCK_NUM_X 24
#define BLOCK_NUM_Y 24
#define BLOCK_PAD 5
#define BOX_X 15
#define BOX_Y 10

#include "stm32f7xx_hal.h"
#include "Board_GLCD.h"
#include "GLCD_Config.h"
#include "Board_Touch.h"
#include <stdlib.h>


int STATE = 3;
struct Snake* currentSnake;
struct Food* currentFood; 
int DIRECTION = 3;

struct TouchArea
{
	unsigned int enabled;
	unsigned int startX;
	unsigned int startY;
	unsigned int sizeX;
	unsigned int sizeY;
};

static struct TouchArea snakeBTAUp;
static struct TouchArea snakeBTADown;
static struct TouchArea snakeBTALeft;
static struct TouchArea snakeBTARight;

static unsigned int renderTickCount = 0;
static unsigned int tmpX = 12;
static unsigned int tmpY = 12;

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

void drawSnakeBox (unsigned int lineColor)
{
	GLCD_SetForegroundColor(lineColor);
	unsigned int edgeLenX = BLOCK_SIZE * BLOCK_NUM_X + 2 * BLOCK_PAD;
	unsigned int edgeLenY = BLOCK_SIZE * BLOCK_NUM_Y + 2 * BLOCK_PAD;
	GLCD_DrawHLine(BOX_X, BOX_Y, edgeLenX);
	GLCD_DrawVLine(BOX_X, BOX_Y, edgeLenY);
	GLCD_DrawHLine(BOX_X, BOX_Y + edgeLenY, edgeLenX);
	GLCD_DrawVLine(BOX_X + edgeLenX, BOX_Y, edgeLenY);
}

void drawSnakeBlock (unsigned int x, unsigned int y, unsigned int fillColor)
{
	if (x >= BLOCK_NUM_X || y >= BLOCK_NUM_Y)
	{
		return;
	}
	GLCD_SetForegroundColor(fillColor);
	unsigned int startX = BOX_X + BLOCK_PAD + x * BLOCK_SIZE;
	unsigned int startY = BOX_Y + BLOCK_PAD + y * BLOCK_SIZE;
	for (unsigned int i = 0; i < BLOCK_SIZE; i++)
	{
		GLCD_DrawHLine(startX, startY + i, BLOCK_SIZE);
	}
}

void drawSnakeButton (unsigned int centerX, unsigned int centerY, unsigned int size, unsigned int spread, unsigned int color)
{
	GLCD_SetForegroundColor(color);
	unsigned int textMarginX = size / 2 - 16;
	unsigned int textMarginY = size / 2 - 12;
	// up
	snakeBTAUp.enabled = 1;
	snakeBTAUp.startX = centerX - size / 2;
	snakeBTAUp.startY = centerY - spread - size / 2;
	snakeBTAUp.sizeX = size;
	snakeBTAUp.sizeY = size;
	GLCD_DrawRectangle(snakeBTAUp.startX, snakeBTAUp.startY, snakeBTAUp.sizeX, snakeBTAUp.sizeY);
	GLCD_DrawString(snakeBTAUp.startX + textMarginX, snakeBTAUp.startY + textMarginY, "UP");
	// down
	snakeBTADown.enabled = 1;
	snakeBTADown.startX = centerX - size / 2;
	snakeBTADown.startY = centerY + spread - size / 2;
	snakeBTADown.sizeX = size;
	snakeBTADown.sizeY = size;
	GLCD_DrawRectangle(snakeBTADown.startX, snakeBTADown.startY, snakeBTADown.sizeX, snakeBTADown.sizeY);
	GLCD_DrawString(snakeBTADown.startX + textMarginX, snakeBTADown.startY + textMarginY, "DN");
	// left
	snakeBTALeft.enabled = 1;
	snakeBTALeft.startX = centerX - spread - size / 2;
	snakeBTALeft.startY = centerY - size / 2;
	snakeBTALeft.sizeX = size;
	snakeBTALeft.sizeY = size;
	GLCD_DrawRectangle(snakeBTALeft.startX, snakeBTALeft.startY, snakeBTALeft.sizeX, snakeBTALeft.sizeY);
	GLCD_DrawString(snakeBTALeft.startX + textMarginX, snakeBTALeft.startY + textMarginY, "LT");
	// right
	snakeBTARight.enabled = 1;
	snakeBTARight.startX = centerX + spread - size / 2;
	snakeBTARight.startY = centerY - size / 2;
	snakeBTARight.sizeX = size;
	snakeBTARight.sizeY = size;
	GLCD_DrawRectangle(snakeBTARight.startX, snakeBTARight.startY, snakeBTARight.sizeX, snakeBTARight.sizeY);
	GLCD_DrawString(snakeBTARight.startX + textMarginX, snakeBTARight.startY + textMarginY, "RT");
}

int checkButtonPressed (int x, int y, struct TouchArea * ta)
{
	return (ta->enabled == 1) && (ta->startX <= (unsigned)x) && ((unsigned)x <= ta->startX + ta->sizeX) &&
		(ta->startY <= (unsigned)y) && ((unsigned)y <= ta->startY + ta->sizeY);
}

void drawGame(void){
	
	drawSnakeBlock(currentFood->coor.x, currentFood->coor.y,GLCD_COLOR_RED);
	drawSnakeBlock(currentSnake->boxToClear.x, currentSnake->boxToClear.y,GLCD_COLOR_BLACK);
	for (int i = 0; i < currentSnake->length; i++) {
		drawSnakeBlock(currentSnake->coor[i].x, currentSnake->coor[i].y,GLCD_COLOR_WHITE);
	}
	
}


void tickHandler (void)
{
	if (renderTickCount == 0)
	{		
		changeDir(currentSnake);
		move(currentSnake);
		
		
		if (checkDead(currentSnake)){
			
			TIM7->DIER &= ~(1 << 0);
			STATE = 3;
			currentSnake = 0;
			currentFood = 0;
		}
		
		
		checkFood(currentSnake,currentFood);
		if (currentFood->eaten){
			currentFood = getNewFood(currentSnake);
		}
		
		drawGame();
		
		
	}
	if (++renderTickCount == TICK_PER_RENDER - 1)
	{
		renderTickCount = 0;
	}
}

void touchHandler (int x, int y)
{
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
}

void TIM7_IRQHandler (void)
{
	// reset flag
	TIM7->SR = ~((unsigned) 1 << 0);
	tickHandler();
}
	
int main (void)
{
	if (currentSnake == 0){
		currentSnake = getNewSnake();
		currentFood = getNewFood(currentSnake);
	}
	
	
	
	SystemClock_Config();
	
	// initiate display and touch
	GLCD_Initialize();
	GLCD_SetBackgroundColor(GLCD_COLOR_BLACK);
	GLCD_SetFont(&GLCD_Font_16x24);
	GLCD_ClearScreen();
	Touch_Initialize();

	// activate TIM7
	RCC->APB1ENR |= (1 << 5);
	// config TIM7 to 20Hz
	TIM7->PSC = 83 - 1;
	TIM7->ARR = 65060 - 1;
	// start counting
	TIM7->CR1 |= (1 << 0);
	// enable TIM7 update interrupt
	TIM7->DIER |= (1 << 0);
	// unmask TIM7 update interrupt
	NVIC_EnableIRQ(TIM7_IRQn);
	
	drawSnakeBox(GLCD_COLOR_YELLOW);
	//drawSnakeBlock(23, 23, GLCD_COLOR_YELLOW);
	drawSnakeButton(370, 136, 50, 60, GLCD_COLOR_RED);
		
	
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
