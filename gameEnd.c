#include "headers.h"
#include "Board_GLCD.h"
#include "GLCD_Config.h"
#include "stm32f7xx.h"
#include <stdio.h>


extern struct Snake* currentSnake;
extern int* historyScores;
extern struct TouchArea endBTAMenu;


void renderGameEndPage(void){
	GLCD_SetForegroundColor(GLCD_COLOR_RED);
	GLCD_DrawString(176, 70, "GAME END");
	char buffer[1024];
	sprintf(buffer, "Score: %d", currentSnake->length-3);
	GLCD_SetForegroundColor(GLCD_COLOR_WHITE);
	GLCD_DrawString(168, 110, buffer);
	endBTAMenu.startX = 130;
	endBTAMenu.startY = 150;
	endBTAMenu.sizeX = 220;
	endBTAMenu.sizeY = 40;
	GLCD_SetForegroundColor(GLCD_COLOR_WHITE);
	GLCD_DrawRectangle(endBTAMenu.startX, endBTAMenu.startY, endBTAMenu.sizeX, endBTAMenu.sizeY);
	GLCD_DrawString(endBTAMenu.startX + 22, endBTAMenu.startY + 10, "Return Menu");
}

void updateHistoryScore(void){
	int score = currentSnake->length-3;
	if (score > historyScores[0]){
		historyScores[2] = historyScores[1];
		historyScores[1] = historyScores[0];
		historyScores[0] = score;
	}
	else if (score > historyScores[1]){
		historyScores[2] = historyScores[1];
		historyScores[1] = score;
	}
	else if (score > historyScores[2]){
		historyScores[2] = score;
	}
}
