static const int UP = 0;
static const int DOWN = 1;
static const int LEFT = 2;
static const int RIGHT = 3;

#define  MAX_LENGTH 100
#define  INITIAL_LENGTH = 3
#define  SIZE 10





struct Coordinate {
	int x;
	int y;
};

struct Snake {
	int length;
	int buffer;
	int direction;
	struct Coordinate* coor;
};

struct Food {
	struct Coordinate coor;
	int eaten;
};


struct Snake* GameInit(int initial_len) {
	
	struct Coordinate* start_coor = (struct Coordinate *) malloc(sizeof(struct Coordinate)*initial_len);
	start_coor[0].x = 100;
	start_coor[0].y = 100;
	start_coor[1].x = 90;
	start_coor[1].y = 100;
	start_coor[2].x = 80;
	start_coor[2].y = 100;


	struct Snake* snake_p = (struct Snake *) malloc(sizeof(struct Snake));
	snake_p->direction = RIGHT;
	snake_p->length = 3;
	snake_p->coor = start_coor;
	/*snake_p->coor[0].x = 100;
	snake_p->coor[0].y = 100;
	snake_p->coor[1].x = 90;
	snake_p->coor[1].y = 100;
	snake_p->coor[2].x = 80;
	snake_p->coor[2].y = 100;*/

	return snake_p;

}
struct Food* CreateFood(struct Snake* snake) {
	struct Food* food = (struct Food *) malloc(sizeof(struct Food));
	int proper;
	while (1) {

		proper = 1;
		food->coor.x = rand() % 64 * 10;
		food->coor.y = rand() % 64 * 10;

		for (int i = 0; i< snake->length;i++){
			if (snake->coor[i].x == food->coor.x && snake->coor[i].y == food->coor.y) {
				proper = 0;
				break;
			}
		}
		if (proper) break;
	}
	return food;
}
static void GrowBuffer(struct Snake* snake) {
	int new_buffer_size = snake->buffer * 2;
	struct Coordinate* new_coor = (struct Coordinate *) malloc(sizeof(struct Coordinate)*new_buffer_size);
	for (int i = 0; i < snake->length; i++) {
		new_coor[i].x = snake->coor[i].x;
		new_coor[i].y = snake->coor[i].y;
	}
	//to do: release memory
	//delete snake->coor;
	snake->coor = new_coor;
	snake->buffer = new_buffer_size;
}

static void CheckFood(struct Snake* snake, struct Food* food) {
	if (snake->coor[0].x == food->coor.x && snake->coor[0].y == food->coor.y) {
		food->eaten = 1;
		snake->length++;
		if (snake->length > snake->buffer) {
			GrowBuffer(snake);
		}
	}
}


static void Draw(struct Snake* snake, struct Food* food) {
	
	
	/*
	to do
	
	
	*/
	
	/*
	system("cls");
	cleardevice();
	cout << "?????:" << snake->length << endl;
	cout << "????:" << snake->direction << endl;

	for (int i = 0; i < snake->length; i++) {
		cout << "?" << i + 1 << "?????:" << snake->coor[i].x << "," << snake->coor[i].y << endl;
		setfillcolor(RGB(255, 255, 255));
		fillrectangle(snake->coor[i].x, snake->coor[i].y, snake->coor[i].x+SIZE, snake->coor[i].y+SIZE);
	}
	setfillcolor(RGB(0, 255, 255));
	fillrectangle(food->coor.x, food->coor.y, food->coor.x + SIZE, food->coor.y + SIZE);
	*/
}

static void Move(struct Snake* snake) {
	for (int i = snake->length  - 1; i > 0; i--) {
		snake->coor[i].x = snake->coor[i - 1].x;
		snake->coor[i].y = snake->coor[i-1].y;
	}
	switch (snake->direction) {
		case 0 : {
			snake->coor[0].y -= 10;
			break;
		}
		case 1 : {
			snake->coor[0].y += 10;
			break;
		}
		case 2 : {
			snake->coor[0].x -= 10;
			break;
		} 
		case 3 : {
			snake->coor[0].x += 10;
			break;
		} 
	}

}

static int CheckDead(struct Snake* snake) {
	if (snake->coor[0].x < 0 || snake->coor[0].x > 630 || snake->coor[0].y < 0 || snake->coor[0].y > 630) {
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

static void ChangeDir(struct Snake* snake) {

	int dir = UP;
	
	//to do: get user input
	//dir = _getch();
	//cout << "Change direction to:" << dir << endl;
	switch (dir) {
	case 0: if (snake->direction != DOWN) snake->direction = UP;
		break;
	case 1: if (snake->direction != UP) snake->direction = DOWN;
		break;
	case 2: if (snake->direction != RIGHT) snake->direction = LEFT;
		break;
	case 3: if (snake->direction != LEFT) snake->direction = RIGHT;
		break;
	}

}

int main() {
	int max_len = 3;
	struct Snake* snake_p = GameInit(max_len);
	struct Food* food = CreateFood(snake_p);
	int game_over = 0;
	
	//to do: main game loop
	
	
	/*
	while (1) {
		while (!_kbhit()) {
			if (food->eaten) {
				delete food;
				food = CreateFood(snake_p);
			}
			Move(snake_p);
			Draw(snake_p,food);
			CheckFood(snake_p, food);
			if (CheckDead(snake_p)){
				game_over = 1;
				break;
			}
			Sleep(30);
		}
		if (game_over) {
			cout << '\a';
			Sleep(100);
			break;
		}
		ChangeDir(snake_p);
	}
	cout << "Game Over!";
	*/
	return 0;
}