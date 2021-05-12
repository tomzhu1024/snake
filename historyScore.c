#include "headers.h"
#include "Board_GLCD.h"
#include "GLCD_Config.h"
#include "stm32f7xx.h"
#include <stdio.h>

extern int historyScores[3];
extern struct TouchArea scoresBTAMenu;


void renderHistoryScorePage(void){
	GLCD_SetForegroundColor(GLCD_COLOR_WHITE);
	char buffer[1024];
	sprintf(buffer, "Score 1: %d", historyScores[0]);
	GLCD_DrawString(150, 90, buffer);
	sprintf(buffer, "Score 2: %d", historyScores[1]);
	GLCD_DrawString(150, 130, buffer);
	sprintf(buffer, "Score 3: %d", historyScores[2]);
	GLCD_DrawString(150, 170, buffer);
	
	scoresBTAMenu.startX = 1;
	scoresBTAMenu.startY = 1;
	scoresBTAMenu.sizeX = 80;
	scoresBTAMenu.sizeY = 30;
	GLCD_SetForegroundColor(GLCD_COLOR_WHITE);
	GLCD_DrawRectangle(scoresBTAMenu.startX, scoresBTAMenu.startY, scoresBTAMenu.sizeX, scoresBTAMenu.sizeY);
	GLCD_DrawString(scoresBTAMenu.startX + 8, scoresBTAMenu.startY + 6, "MENU");
}
