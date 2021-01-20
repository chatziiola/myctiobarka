/*
 * 'Sailing-game' by Panagiotidis Giorgos, Chatziioannou Lamprinos
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
#define crashCHAR 'X' | A_BLINK
// This last one wasn't discarded for presumably further development but won't 
// be used for now
#define stdscrCHAR ' ' | A_REVERSE

#include "myPlayer.h"
#include "myWindow.h"

char *menuOptions[] = {"Play","Controls","Credits","Exit",(char *)NULL};

int main()
{
	char choice[10];
	int ch;
	WINDOW *gameWin, *menuWin;
	time_t realRandom;

	initialization();	
	gameWin = newwin(gameHEIGHT,gameWIDTH, (LINES-gameHEIGHT)/2, (COLS-gameWIDTH)/2);
	menuWin = newwin(menuHEIGHT,menuWIDTH, (LINES-menuHEIGHT)/2, (COLS-menuWIDTH)/2);

	//fillSTDSCR();
	// Show menu and save the user's choice in the choice char arr
	showMenu(choice,menuOptions,menuWin);	
	if(strcmp(choice,"Controls") == 0)
		showControls(menuWin);
	else if(strcmp(choice,"Credits") == 0)
		showCredits(menuWin);
	else if(strcmp(choice,"Exit") == 0)
		showExit(menuWin);
	else
	{
		// Print initial screen, play a sound "\a"
		// -> with "press key to continue"
		// Ask for names and initialize your players
		// Little help dialogue -> using the menuWin
		// presumably more than 1 page long (-_-) -> will need to take care of
		// it
		// Refresh the game win so that help is not shown
		// Create small window at the top to show ANIMATION/PLAYER !
		// Create small window at the bottom to show current location
		// (TODO number of crashes would be ideal too!)
		srand((unsigned) time(&realRandom));
		while(true)
		{
			// Calculate direction and intensity of the wind -> Print it
			// TODO Where should one save these keystrokes?
			// Player 1 -> Enter 3 keystrokes
			// Player 2 -> Enter 3 keystrokes
			box(gameWin,0,0);
			arrowWind(derwin(gameWin,1,10,18,66), rand()%4, rand()%4);
			printLand(gameWin);
			wrefresh(gameWin);
			ch = getch();
			// Escape character
			if ( ch == 27)
				break;
		}
	}
	
	// Releasing memory allocated during the creation of our menu
	endwin();
	return 0;
}

