#define MENU 0
#define GAME_START 1
#define GAME_END 2
#define GAME_PAUSE 3
#define HISTORY_SCORE 4
#define GAME_RECORD 5

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3





struct TouchArea
{
	unsigned int enabled;
	unsigned int startX;
	unsigned int startY;
	unsigned int sizeX;
	unsigned int sizeY;
};



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

struct GameRecord {
	int recordId;
	struct Snake * snake;
	struct Food * food;
	struct GameRecord* nextRecord;

};

// main 
void renderPage(void);


//menu
void renderMenuPage(void);

// history score
void renderHistoryScorePage(void);

// game record
void renderGameRecordPage(void);


// game start

struct Snake* getNewSnake(void);

struct Food* getNewFood(struct Snake* snake);

void growBuffer(struct Snake* snake);

void checkFood(struct Snake* snake, struct Food* food);

void move(struct Snake* snake);

int checkDead(struct Snake* snake);

void changeDir(struct Snake* snake);

void drawSnakeBox (unsigned int);

void drawSnakeButton (unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);

void drawSnakeBlock(unsigned int, unsigned int, unsigned int);

void drawSnakeAndFood(void);

void gameStartPageInitialize(void);

void gameRender(void);

// game end
void renderGameEndPage(void);
void updateHistoryScore(void);

//game pause
void renderGamePausePage(void);

void gameSave(void);



