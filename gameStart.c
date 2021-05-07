#include "headers.h"
#include "Board_GLCD.h"
#include "GLCD_Config.h"

extern int STATE;
extern struct Snake* currentSnake;
extern struct Food* currentFood;


static void drawGame(void){
	
	drawSnakeBox(currentFood->coor.x, currentFood->coor.y,GLCD_COLOR_RED);
	
	drawSnakeBox(currentSnake->boxToClear->x, currentSnake->boxToClear->y,GLCD_COLOR_BLACK);
	for (int i = 0; i < currentSnake->length; i++) {
		drawSnakeBox(currentSnake->coor[i].x, currentSnake->coor[i].y,GLCD_COLOR_WHITE);
	}
	
}

static void gameStartLoop(void){
	
	if (currentSnake == 0){
		currentSnake = getNewSnake();
		currentFood = getNewFood(currentSnake);
	}
	
	while (STATE == 1){
		
		changeDir(currentSnake);
		
		move(currentSnake);
		
		if (checkDead(currentSnake)){
			STATE = 3;
			currentSnake = 0;
			currentFood = 0;
			break;
		}
		
		if (currentFood->eaten){
			currentFood = getNewFood(currentSnake);
		}
	}
	drawGame();
}