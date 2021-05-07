struct Coordinate {
	int x;
	int y;
};

struct Snake {
	int length;
	int buffer;
	int direction;
	struct Coordinate* coor;
	struct Coordinate boxToClear;
};

struct Food {
	struct Coordinate coor;
	int eaten;
};

struct Snake* getNewSnake(void);

struct Food* getNewFood(struct Snake* snake);

void growBuffer(struct Snake* snake);

void checkFood(struct Snake* snake, struct Food* food);

void move(struct Snake* snake);

int checkDead(struct Snake* snake);

void changeDir(struct Snake* snake);

void drawSnakeBlock(unsigned int, unsigned int, unsigned int);

