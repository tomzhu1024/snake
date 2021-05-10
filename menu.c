#include "headers.h"
#include "Board_GLCD.h"
#include "GLCD_Config.h"
#include "stm32f7xx.h"

extern struct TouchArea menuBTANew;
extern struct TouchArea menuBTALoad;
extern struct TouchArea menuBRAScores;

void renderMenuPage(void) {
	GLCD_SetForegroundColor(GLCD_COLOR_RED);
	GLCD_DrawString(168, 20, "S N A K E");
	
	menuBTANew.startX = 150;
	menuBTANew.startY = 60;
	menuBTANew.sizeX = 180;
	menuBTANew.sizeY = 40;
	GLCD_SetForegroundColor(GLCD_COLOR_WHITE);
	GLCD_DrawRectangle(menuBTANew.startX, menuBTANew.startY, menuBTANew.sizeX, menuBTANew.sizeY);
	GLCD_DrawString(menuBTANew.startX + 26, menuBTANew.startY + 10, "New Game");

	menuBTALoad.startX = 150;
	menuBTALoad.startY = 60;
	menuBTALoad.sizeX = 180;
	menuBTALoad.sizeY = 40;
	GLCD_SetForegroundColor(GLCD_COLOR_WHITE);
	GLCD_DrawRectangle(menuBTANew.startX, menuBTANew.startY, menuBTANew.sizeX, menuBTANew.sizeY);
	GLCD_DrawString(menuBTANew.startX + 26, menuBTANew.startY + 10, "New Game");

	menuBTANew.startX = 150;
	menuBTANew.startY = 60;
	menuBTANew.sizeX = 180;
	menuBTANew.sizeY = 40;
	GLCD_SetForegroundColor(GLCD_COLOR_WHITE);
	GLCD_DrawRectangle(menuBTANew.startX, menuBTANew.startY, menuBTANew.sizeX, menuBTANew.sizeY);
	GLCD_DrawString(menuBTANew.startX + 26, menuBTANew.startY + 10, "New Game");
}

