#include "headers.h"


extern struct Snake* currentSnake;
extern int* historyScores;


void renderGameEndPage(void){
	// to do: render  page

}

void updateHistoryScore(void){
	int score = currentSnake->length-3;
	if (score > historyScores[0]){
		historyScores[2] = historyScores[1];
		historyScores[1] = historyScores[0];
		historyScores[0] = score;
	}
	else if (score > historyScores[1]){
		historyScores[2] = historyScores[1];
		historyScores[1] = score;
	}
	else if (score > historyScores[2]){
		historyScores[2] = score;
	}
}
