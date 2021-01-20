#define playerCHAR 'x'

typedef struct _game_player{
	chtype ch;
	char name[50];
	int x,y;
}PLAYER;

void playerInitialize(PLAYER *player, char *name, char index, chtype playerChar);
void playerReset(PLAYER *player, char index);
void playersReset(PLAYER *players);
void printPlayer(WINDOW *win,PLAYER player);

void playerInitialize(PLAYER *player, char *name, char index, chtype playerChar)
{
	strcpy(player->name, name);
	player->ch = playerChar;
	playerReset(player, index);
}

void playerReset(PLAYER *player, char index)
{
	if( index == 0 )
	{
		player->x = 69;
		player->y = 9;
	}
	else
	{
		player->x = 49;
		player->y = 9;
	}

}

void playersReset(PLAYER *players)
{
	// Player 1
	playerReset(&players[0], 0);
	// Player 2
	playerReset(&players[1], 1);
	
}

void printPlayer(WINDOW *win,PLAYER player)
{
	mvwaddch(win, player.y, player.x, player.ch);
}
