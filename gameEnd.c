extern int STATE;



static void drawGameEnd(void){
	// to do: render game end page


}

static void gameEndLoop(void){
	drawGameEnd();
	
	while (1){
		if (STATE != 3) break;
	}
}