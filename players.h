typedef struct _game_player{
	bool hasCrashed;
	int x,y;
	chtype ch;
}PLAYER;

void playersReset(PLAYER players[])
{
	for(int l=0; l<2; l++)
		players[l].hasCrashed = false;
	// Complete the function
}

void playerReset(PLAYER player)
{
	// Complete the function
}

void printPlayer(WINDOW *win,PLAYER player)
{
	mvwaddch(win, player.y, player.x, player.ch);
}
