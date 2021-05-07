extern int STATE;



static void drawGamePause(void){
	// to do: render menu page


}

static void gamePauseLoop(void){
	drawGamePause();
	
	while (1){
		if (STATE != 2) break;
	}
}