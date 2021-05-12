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
extern int gameRecordLength;

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

 if (gameRecordLength >= 6){
  free(p->snake->coor);
  free(p->snake);
  free(p->food);
  p->snake = currentSnake;
  p->food = currentFood;
  p->recordId = currentGameId;
  currentSnake = NULL;
  currentFood = NULL;
  return;
 }
 
 gameRecordLength++;

 if (p == NULL){
  struct GameRecord* newRecord = (struct GameRecord*) malloc(sizeof(struct GameRecord));
  newRecord->nextRecord = 0;
  newRecord->snake = currentSnake;
  newRecord->food = currentFood;
  newRecord->recordId = uuid;
  uuid++;
  gameRecords = newRecord;
  currentSnake = NULL;
  currentFood = NULL;
  currentGameId = 0;
  return;
 }
 
 while (p != NULL){
  if ( p->recordId == currentGameId){
   free(p->snake->coor);
   free(p->snake);
   free(p->food);
   p->snake = currentSnake;
   p->food = currentFood;
   currentSnake = NULL;
   currentFood = NULL;
   currentGameId = 0;
   return;
  }
  
  if (p->nextRecord == NULL){
   struct GameRecord* newRecord = (struct GameRecord*) malloc(sizeof(struct GameRecord));
   newRecord->nextRecord = 0;
   newRecord->snake = currentSnake;
   newRecord->food = currentFood;
   newRecord->recordId = uuid;
   uuid++;
   p->nextRecord = newRecord;
	 return;
  }
  
  p = p->nextRecord;
 }
}

