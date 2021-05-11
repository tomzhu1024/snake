#include "headers.h"
#include "Board_GLCD.h"
#include "GLCD_Config.h"
#include "stm32f7xx.h"
#include <stdlib.h>



#define BLOCK_SIZE 10
#define BLOCK_NUM_X 24
#define BLOCK_NUM_Y 24
#define BLOCK_PAD 5
#define BOX_X 15
#define BOX_Y 10

extern int STATE;
extern int uuid;
extern int currentGameId;
extern struct Snake* currentSnake;
extern struct Food* currentFood;
extern struct TouchArea snakeBTAUp;
extern struct TouchArea snakeBTADown;
extern struct TouchArea snakeBTALeft;
extern struct TouchArea snakeBTARight;





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
	snakeBTAUp.startX = centerX - size / 2;
	snakeBTAUp.startY = centerY - spread - size / 2;
	snakeBTAUp.sizeX = size;
	snakeBTAUp.sizeY = size;
	GLCD_DrawRectangle(snakeBTAUp.startX, snakeBTAUp.startY, snakeBTAUp.sizeX, snakeBTAUp.sizeY);
	GLCD_DrawString(snakeBTAUp.startX + textMarginX, snakeBTAUp.startY + textMarginY, "UP");
	// down
	snakeBTADown.startX = centerX - size / 2;
	snakeBTADown.startY = centerY + spread - size / 2;
	snakeBTADown.sizeX = size;
	snakeBTADown.sizeY = size;
	GLCD_DrawRectangle(snakeBTADown.startX, snakeBTADown.startY, snakeBTADown.sizeX, snakeBTADown.sizeY);
	GLCD_DrawString(snakeBTADown.startX + textMarginX, snakeBTADown.startY + textMarginY, "DN");
	// left
	snakeBTALeft.startX = centerX - spread - size / 2;
	snakeBTALeft.startY = centerY - size / 2;
	snakeBTALeft.sizeX = size;
	snakeBTALeft.sizeY = size;
	GLCD_DrawRectangle(snakeBTALeft.startX, snakeBTALeft.startY, snakeBTALeft.sizeX, snakeBTALeft.sizeY);
	GLCD_DrawString(snakeBTALeft.startX + textMarginX, snakeBTALeft.startY + textMarginY, "LT");
	// right
	snakeBTARight.startX = centerX + spread - size / 2;
	snakeBTARight.startY = centerY - size / 2;
	snakeBTARight.sizeX = size;
	snakeBTARight.sizeY = size;
	GLCD_DrawRectangle(snakeBTARight.startX, snakeBTARight.startY, snakeBTARight.sizeX, snakeBTARight.sizeY);
	GLCD_DrawString(snakeBTARight.startX + textMarginX, snakeBTARight.startY + textMarginY, "RT");
}






void gameStartPageInitialize(void){
	while (!(RNG->SR & (1 << 0)));
	srand(RNG->DR);
	//No record is loaded, start new game
	if (currentSnake == 0){
		currentGameId = uuid;
		uuid++;
		currentSnake = getNewSnake();
		currentFood = getNewFood(currentSnake);
	}
	drawSnakeBox(GLCD_COLOR_YELLOW);
	drawSnakeButton(370, 136, 45, 45, GLCD_COLOR_RED);
	startTimer();
}

void drawSnakeAndFood(void){
	drawSnakeBlock(currentFood->coor.x, currentFood->coor.y,GLCD_COLOR_RED);
	drawSnakeBlock(currentSnake->boxToClear.x, currentSnake->boxToClear.y,GLCD_COLOR_BLACK);
	for (int i = 0; i < currentSnake->length-1; i++) {
		drawSnakeBlock(currentSnake->coor[i].x, currentSnake->coor[i].y,GLCD_COLOR_WHITE);
	}
}


void gameRender(void){	
	changeDir(currentSnake);
	move(currentSnake);
	if (checkDead(currentSnake)){
		stopTimer();
		updateHistoryScore();
		STATE = GAME_END;
		renderPage();
		currentSnake = NULL;
		currentFood = NULL;
		currentGameId = 0;
		return;  // avoid new food generation after death
	}
	checkFood(currentSnake,currentFood);
	if (currentFood->eaten){
		currentFood = getNewFood(currentSnake);
	}
	drawSnakeAndFood();
}