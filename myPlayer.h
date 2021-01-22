void playerReset(PLAYER *player, char index);
void playersReset(PLAYER *players);

void getChar(WINDOW *win, PLAYER *player, int winLength)
{
	chtype ch;
	wclear(win);
	box(win,0,0);
	printInMiddle(win,1,"Please enter your character:",A_NORMAL);
	mvwaddch(win,2,10,'>');
	wrefresh(win);
	ch = wgetch(win);
	wclear(win);
	printInMiddle(win,1,"Is it ok? (Y/N)",A_NORMAL);
	mvwaddch(win,2, winLength/2, ch|A_BOLD);
	wrefresh(win);
	if ( getch() == 'Y' )
		player->ch=ch;
	else
		getChar(win, player, winLength);
}

void getName(WINDOW *win, PLAYER *player, int winLength)
{
	char *name;
	name = (char *)malloc(20);
	wclear(win);
	box(win,0,0);
	printInMiddle(win,1,"Please enter your name:",A_NORMAL);
	mvwaddch(win,2,10,'>');
	wrefresh(win);
	wgetstr(win,name);
	wclear(win);
	printInMiddle(win,1,"Is it ok? (Y/N)",A_NORMAL);
	printInMiddle(win,2,name,A_NORMAL);
	wrefresh(win);
	if ( getch() == 'Y' )
	{
		strcpy(player->name, name);
		getChar(win, player, winLength);
	}
	else
		getName(win, player, winLength);
}


void initPlayers(WINDOW * win,PLAYER *players)
{
	playersReset(players);
	int maxY,maxX;
	getmaxyx(win,maxX,maxX);
	echo();
	wclear(win);
	printTitle(win);
	printInMiddle(win,7,"Now let's start with your characters!!", A_NORMAL);
	printInMiddle(win,8,"Player 1, your turn", A_BOLD);
	wrefresh(win);
	getName(derwin(win,5,maxX-2,10,1), &players[0], maxX);
	printInMiddle(win,8,"Player 2, your turn", A_BOLD);
	wrefresh(win);
	getName(derwin(win,5,maxX-2,10,1), &players[1], maxX);
	noecho();
}

void playerReset(PLAYER *player, char index)
{
	if( index == 0 )
	{
		player->x = 50;
		player->y = 4;
	}
	else
	{
		player->x = 50;
		player->y = 5;
	}
}

void playersReset(PLAYER *players)
{
	// Player 1
	playerReset(&players[0], 0);
	// Player 2
	playerReset(&players[1], 1);
}

void printPlData(WINDOW *win, PLAYER *players)
{

	mvwprintw(win,0,0,"%s: x:%d y:%d", players[0].name,
			players[0].x, players[0].y);
	mvwprintw(win,0,30,"| %s: x:%d y:%d", players[1].name,
			players[1].x, players[1].y);
}


