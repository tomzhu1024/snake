#include "headers.h"
#include "Board_GLCD.h"
#include "GLCD_Config.h"
#include "stm32f7xx.h"
#include <stdio.h>

extern struct GameRecord* gameRecords;

extern int currentGameId;
extern struct Snake* currentSnake;
extern struct Food* currentFood;


extern volatile struct TouchArea recordBTAMenu;
extern volatile struct TouchArea recordBTA1;
extern volatile struct TouchArea recordBTA2;
extern volatile struct TouchArea recordBTA3;
extern volatile struct TouchArea recordBTA4;
extern volatile struct TouchArea recordBTA5;
extern volatile struct TouchArea recordBTA6;

void renderGameRecordPage(void){
	recordBTAMenu.startX = 1;
	recordBTAMenu.startY = 1;
	recordBTAMenu.sizeX = 80;
	recordBTAMenu.sizeY = 30;
	GLCD_SetForegroundColor(GLCD_COLOR_WHITE);
	GLCD_DrawRectangle(recordBTAMenu.startX, recordBTAMenu.startY, recordBTAMenu.sizeX, recordBTAMenu.sizeY);
	GLCD_DrawString(recordBTAMenu.startX + 8, recordBTAMenu.startY + 6, "MENU");
	
	recordBTA1.startX = 30;
	recordBTA1.startY = 80;
	recordBTA1.sizeX = 50;
	recordBTA1.sizeY = 40;
	GLCD_SetForegroundColor(GLCD_COLOR_WHITE);
	GLCD_DrawRectangle(recordBTA1.startX, recordBTA1.startY, recordBTA1.sizeX, recordBTA1.sizeY);
	GLCD_DrawString(recordBTA1.startX + 17, recordBTA1.startY + 10, "1");
	
	recordBTA2.startX = 30;
	recordBTA2.startY = 140;
	recordBTA2.sizeX = 50;
	recordBTA2.sizeY = 40;
	GLCD_SetForegroundColor(GLCD_COLOR_WHITE);
	GLCD_DrawRectangle(recordBTA2.startX, recordBTA2.startY, recordBTA2.sizeX, recordBTA2.sizeY);
	GLCD_DrawString(recordBTA2.startX + 17, recordBTA2.startY + 10, "2");
	
	recordBTA3.startX = 30;
	recordBTA3.startY = 200;
	recordBTA3.sizeX = 50;
	recordBTA3.sizeY = 40;
	GLCD_SetForegroundColor(GLCD_COLOR_WHITE);
	GLCD_DrawRectangle(recordBTA3.startX, recordBTA3.startY, recordBTA3.sizeX, recordBTA3.sizeY);
	GLCD_DrawString(recordBTA3.startX + 17, recordBTA3.startY + 10, "3");
	
	recordBTA4.startX = 400;
	recordBTA4.startY = 80;
	recordBTA4.sizeX = 50;
	recordBTA4.sizeY = 40;
	GLCD_SetForegroundColor(GLCD_COLOR_WHITE);
	GLCD_DrawRectangle(recordBTA4.startX, recordBTA4.startY, recordBTA4.sizeX, recordBTA4.sizeY);
	GLCD_DrawString(recordBTA4.startX + 17, recordBTA4.startY + 10, "4");
	
	recordBTA5.startX = 400;
	recordBTA5.startY = 140;
	recordBTA5.sizeX = 50;
	recordBTA5.sizeY = 40;
	GLCD_SetForegroundColor(GLCD_COLOR_WHITE);
	GLCD_DrawRectangle(recordBTA5.startX, recordBTA5.startY, recordBTA5.sizeX, recordBTA5.sizeY);
	GLCD_DrawString(recordBTA5.startX + 17, recordBTA5.startY + 10, "5");
	
	recordBTA6.startX = 400;
	recordBTA6.startY = 200;
	recordBTA6.sizeX = 50;
	recordBTA6.sizeY = 40;
	GLCD_SetForegroundColor(GLCD_COLOR_WHITE);
	GLCD_DrawRectangle(recordBTA6.startX, recordBTA6.startY, recordBTA6.sizeX, recordBTA6.sizeY);
	GLCD_DrawString(recordBTA6.startX + 17, recordBTA6.startY + 10, "6");

}

void loadGame(int gamePos){
 struct GameRecord* p = gameRecords;
 while(gamePos>0 && p != NULL){
  p = p->nextRecord;
 }
 if (p != NULL){
  currentSnake = p->snake;
  currentFood = p->food;
  currentGameId = p->recordId;
 }
}
