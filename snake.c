#include "headers.h"
#include <stdlib.h>



#define  MAX_LENGTH 100
#define  INITIAL_LENGTH  3
#define INITIAL_BUFFER  1000
#define  SIZE 10

extern int DIRECTION;


struct Snake* getNewSnake() {
	struct Coordinate* start_coor = (struct Coordinate *) malloc(sizeof(struct Coordinate)*INITIAL_BUFFER);
	start_coor[0].x = 10;
	start_coor[0].y = 10;
	start_coor[1].x = 9;
	start_coor[1].y = 10;
	start_coor[2].x = 8;
	start_coor[2].y = 10;
	
	struct Coordinate* box_to_clear = (struct Coordinate *) malloc(sizeof(struct Coordinate));
	box_to_clear->x = 8;
	box_to_clear->y = 10;

	struct Snake* snake_p = (struct Snake *) malloc(sizeof(struct Snake));
	snake_p->direction = 3;
	snake_p->buffer = INITIAL_BUFFER;
	snake_p->length = INITIAL_LENGTH;
	snake_p->coor = start_coor;
	snake_p->boxToClear = *box_to_clear;
	return snake_p;
}




struct Food* getNewFood(struct Snake* snake) {
	struct Food* food = (struct Food *) malloc(sizeof(struct Food));
	food->eaten = 0;
	int proper = 0;
	while (!proper) {

		proper = 1;
		food->coor.x = rand() % 20;
		food->coor.y = rand() % 20;

		for (int i = 0; i< snake->length;i++){
			if (snake->coor[i].x == food->coor.x && snake->coor[i].y == food->coor.y) {
				proper = 0;
				break;
			}
		}
	}
	return food;
}



/*
void growBuffer(struct Snake* snake) {
	int new_buffer_size = snake->buffer * 2;
	struct Coordinate* new_coor = (struct Coordinate *) malloc(sizeof(struct Coordinate)*new_buffer_size);
	for (int i = 0; i < snake->length-1; i++) {
		new_coor[i].x = snake->coor[i].x;
		new_coor[i].y = snake->coor[i].y;
	}
	//to do: release memory
	//delete snake->coor;
	snake->coor = new_coor;
	snake->buffer = new_buffer_size;
}
*/



void checkFood(struct Snake* snake, struct Food* food) {
	if (snake->coor[0].x == food->coor.x && snake->coor[0].y == food->coor.y) {
		food->eaten = 1;
		snake->length++;
		
		/*
		if (snake->length > snake->buffer) {
			growBuffer(snake);
		}
		*/
		
	}
}




void move(struct Snake* snake) {
	snake->boxToClear.x = snake->coor[snake->length-1].x;
	snake->boxToClear.y = snake->coor[snake->length-1].y;
	for (int i = snake->length  - 1; i > 0; i--) {
		snake->coor[i].x = snake->coor[i - 1].x;
		snake->coor[i].y = snake->coor[i-1].y;
	}
	switch (snake->direction) {
		case 0 : {
			snake->coor[0].y -= 1;
			break;
		}
		case 1 : {
			snake->coor[0].y += 1;
			break;
		}
		case 2 : {
			snake->coor[0].x -= 1;
			break;
		} 
		case 3 : {
			snake->coor[0].x += 1;
			break;
		} 
	}

}

 int checkDead(struct Snake* snake) {
	if (snake->coor[0].x < 0 || snake->coor[0].x > 23 || snake->coor[0].y < 0 || snake->coor[0].y > 23) {
		return 1;
	}
	for (int i = snake->length - 1; i > 0; i--) {
		if (snake->coor[i].x == snake->coor[0].x && snake->coor[i].y == snake->coor[0].y) {
			// crash happens, quit
			return 1;

		}
	}
	return 0;
}

void changeDir(struct Snake* snake) {
	switch (DIRECTION) {
	case 0: if (snake->direction != 1) snake->direction = 0;
		break;
	case 1: if (snake->direction != 0) snake->direction = 1;
		break;
	case 2: if (snake->direction != 3) snake->direction = 2;
		break;
	case 3: if (snake->direction != 2) snake->direction = 3;
		break;
	}
}
