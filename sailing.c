/*
 * 'Sailing-game' by Chatziioannou Lamprinos("chatziiol@ece.auth.gr")
 * and George Panagiotidis("geopanthe@ece.auth.gr")
 */

#include <unistd.h>
#include <ncurses.h>
#include <menu.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// gameTITLE
#define gameTITLE "SAILING GAME"

// gameWIDTH, gameHEIGHT
//		@usage: these two values are the dimensions of our game window
//				in which the game will take place
#define gameWIDTH 80
#define gameHEIGHT 20

// menuWIDTH, menuHEIGHT
//		@usage: these two values are the dimensions of our menu window, 
//				which the users will be able to access at launch and when
//				ESC is pressed
#define menuWIDTH 50
#define menuHEIGHT 18
#define menuOPTIONS 4

// landCHAR, crashCHAR, stdscrCHAR
// 		@usage: obvious
#define landCHAR '#'
#define crashCHAR 'X'
// This last one wasn't discarded for presumably further development but won't 
// be used for now
#define stdscrCHAR ' ' | A_REVERSE

typedef struct _game_player{
	chtype ch;
	char name[50];
	int x,y;
}PLAYER;

// Print functions
void printSequence(WINDOW * win,int startY, int startX, int length, chtype character);
void printInMiddle(WINDOW *win, int row, char *string, chtype attrs);
bool printMap(WINDOW * win, PLAYER *player);
void printTitle(WINDOW *titleWin);

// Show functions
void showMenu(WINDOW *menuWin);
void showCredits(WINDOW *credWin);
void showExit(WINDOW *exitWin);
void showControls(WINDOW *ctrlWin);
void showIntro(WINDOW *introWin);

#include "myPlayer.h"
#include "myWindow.h"

int main()
{
	int ch;
	WINDOW *gameWin, *menuWin;
	time_t realRandom;

	initCurses(&gameWin, &menuWin);	
	showMenu(menuWin);	


	// Here the actual game begins
	PLAYER players[2];
	
	// Smaller windows so that I can more easily use general functs to display
	// specific information, look at layout inside the game
	WINDOW *scrTitleWin = derwin(gameWin, 3, 30, 0, 25);
	// It can then be further split to 30-30 to fill our needs for displaying
	// coords
	WINDOW *plrDataWin  = derwin(gameWin,1,60,18,2); 
	WINDOW *wndDataWin	= derwin(gameWin,1,10,18,66);

	printLand(gameWin);
	wrefresh(gameWin);
	showIntro(menuWin);
	initPlayers(menuWin, players);
	srand((unsigned) time(&realRandom));
	while(true)
	{
		// Calculate direction and intensity of the wind -> Print it
		// Player 1 -> Enter 3 keystrokes
		// Player 2 -> Enter 3 keystrokes
		box(gameWin,0,0);
		wclear(scrTitleWin);
		box(scrTitleWin,0,0);
		printInMiddle(scrTitleWin, 1, "You fooked", A_BOLD);
		arrowWind(wndDataWin,rand()%4, rand()%4);
		printMap(menuWin, players);
		printPlData(plrDataWin, players);
//		titleSelect(scrTitleWin, "Player 1 selects");
//		titleSelect(scrTitleWin, "Animation(1/5)");
//		titleSelect(scrTitleWin, "You fooked");
//		titleSelect(scrTitleWin, "Player 2 selects");
		wrefresh(gameWin);
		ch = getch();
		// Escape character
		if ( ch == 27)
			break;
	}
	// Releasing memory allocated during the creation of our menu
	endwin();
	return 0;
}
