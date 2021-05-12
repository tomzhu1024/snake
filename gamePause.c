#include "headers.h"
#include "Board_GLCD.h"
#include "GLCD_Config.h"
#include "stm32f7xx.h"
#include <stdio.h>
#include <stdlib.h>

extern int currentGameId;
extern int uuid;
extern struct Snake* currentSnake;
extern struct Food* currentFood;
extern struct GameRecord* gameRecords;

extern struct TouchArea pauseBTAResume;
extern struct TouchArea pauseBTASave;


void renderGamePausePage(void){
	GLCD_SetForegroundColor(GLCD_COLOR_RED);
	GLCD_DrawString(168, 30, "S N A K E");
	
	pauseBTAResume.startX = 130;
	pauseBTAResume.startY = 85;
	pauseBTAResume.sizeX = 220;
	pauseBTAResume.sizeY = 40;
	GLCD_SetForegroundColor(GLCD_COLOR_WHITE);
	GLCD_DrawRectangle(pauseBTAResume.startX, pauseBTAResume.startY, pauseBTAResume.sizeX, pauseBTAResume.sizeY);
	GLCD_DrawString(pauseBTAResume.startX + 61, pauseBTAResume.startY + 10, "Resume");

	pauseBTASave.startX = 130;
	pauseBTASave.startY = 140;
	pauseBTASave.sizeX = 220;
	pauseBTASave.sizeY = 40;
	GLCD_SetForegroundColor(GLCD_COLOR_WHITE);
	GLCD_DrawRectangle(pauseBTASave.startX, pauseBTASave.startY, pauseBTASave.sizeX, pauseBTASave.sizeY);
	GLCD_DrawString(pauseBTASave.startX + 78, pauseBTASave.startY + 10, "Save");
}


void gameSave(void){
	struct GameRecord* p = gameRecords;
	
	if (p==0){
		struct GameRecord* newRecord = (struct GameRecord*) malloc(sizeof(struct GameRecord));
		newRecord->nextRecord = 0;
		newRecord->snake = currentSnake;
		newRecord->food = currentFood;
		newRecord->recordId = uuid;
		uuid++;
		gameRecords = newRecord;
		return;
	}
	
	while (p){
		if ( p->recordId == currentGameId){
			free(p->snake);
			free(p->food);
			p->snake = currentSnake;
			p->food = currentFood;
			currentSnake = 0;
			currentFood = 0;
			return;
		}
		
		if (p->nextRecord == 0){
			struct GameRecord* newRecord = (struct GameRecord*) malloc(sizeof(struct GameRecord));
			newRecord->nextRecord = 0;
			newRecord->snake = currentSnake;
			newRecord->food = currentFood;
			newRecord->recordId = uuid;
			uuid++;
			p->nextRecord = newRecord;
		}
		
		p = p->nextRecord;
	}
}
