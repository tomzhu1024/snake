#include "headers.h"
#include "Board_GLCD.h"
#include "GLCD_Config.h"
#include "stm32f7xx.h"

extern volatile struct TouchArea menuBTANew;
extern volatile struct TouchArea menuBTALoad;
extern volatile struct TouchArea menuBTAScores;

void renderMenuPage(void) {
	GLCD_SetForegroundColor(GLCD_COLOR_RED);
	GLCD_DrawString(168, 30, "S N A K E");
	
	menuBTANew.startX = 130;
	menuBTANew.startY = 85;
	menuBTANew.sizeX = 220;
	menuBTANew.sizeY = 40;
	GLCD_SetForegroundColor(GLCD_COLOR_WHITE);
	GLCD_DrawRectangle(menuBTANew.startX, menuBTANew.startY, menuBTANew.sizeX, menuBTANew.sizeY);
	GLCD_DrawString(menuBTANew.startX + 46, menuBTANew.startY + 10, "New Game");

	menuBTALoad.startX = 130;
	menuBTALoad.startY = 140;
	menuBTALoad.sizeX = 220;
	menuBTALoad.sizeY = 40;
	GLCD_SetForegroundColor(GLCD_COLOR_WHITE);
	GLCD_DrawRectangle(menuBTALoad.startX, menuBTALoad.startY, menuBTALoad.sizeX, menuBTALoad.sizeY);
	GLCD_DrawString(menuBTALoad.startX + 38, menuBTALoad.startY + 10, "Load Game");

	menuBTAScores.startX = 130;
	menuBTAScores.startY = 195;
	menuBTAScores.sizeX = 220;
	menuBTAScores.sizeY = 40;
	GLCD_SetForegroundColor(GLCD_COLOR_WHITE);
	GLCD_DrawRectangle(menuBTAScores.startX, menuBTAScores.startY, menuBTAScores.sizeX, menuBTAScores.sizeY);
	GLCD_DrawString(menuBTAScores.startX + 6, menuBTAScores.startY + 10, "History Score");
}

