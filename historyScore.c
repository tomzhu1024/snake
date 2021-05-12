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
	GLCD_DrawString(150, 50, buffer);
	sprintf(buffer, "Score 2: %d", historyScores[0]);
	GLCD_DrawString(150, 90, buffer);
	sprintf(buffer, "Score 3: %d", historyScores[0]);
	GLCD_DrawString(150, 130, buffer);
	
	scoresBTAMenu.startX = 130;
	scoresBTAMenu.startY = 170;
	scoresBTAMenu.sizeX = 220;
	scoresBTAMenu.sizeY = 40;
	GLCD_SetForegroundColor(GLCD_COLOR_WHITE);
	GLCD_DrawRectangle(scoresBTAMenu.startX, scoresBTAMenu.startY, scoresBTAMenu.sizeX, scoresBTAMenu.sizeY);
	GLCD_DrawString(scoresBTAMenu.startX + 22, scoresBTAMenu.startY + 10, "Return Menu");
}
