#include "headers.h"
#include "Board_GLCD.h"
#include "GLCD_Config.h"

extern int STATE;
extern struct Snake* currentSnake;
extern struct Food* currentFood;




static void gameStartUpdate(void){
	
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
		//drawGame();

	}
}