#include "headers.h"
#include <stdlib.h>

extern int currentGameId;
extern int uuid;
extern struct Snake* currentSnake;
extern struct Food* currentFood;
extern struct GameRecord* gameRecords;


void renderGamePausePage(void){
	// to do: render  page

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
