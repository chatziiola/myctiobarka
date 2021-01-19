// Prints the land along with the boats, evaluating whether a crash ocurred
// This whole function is designed for a 20 by 80 character window, will 
// malfunction if used in a different context
#include "players.h"

#define landCHAR '#'
#define playerOneCHAR 'b' | COLOR_BLUE
#define playerTwoCHAR 'b' | COLOR_RED
#define crashCHAR 'X' | A_BLINK
#define stdscrCHAR ' ' | A_REVERSE

//  chtype mvinch(int y, int x); 
 

void printSequence(WINDOW * win,int startY, int startX, int length, chtype character)
{
	wmove(win, startY, startX);
	if(length < 0)
	{
		getmaxyx(win, startY, length);
		length -= startX+1; 
	}
	for(int l=0; l < length; l++ )
		waddch(win,character);	
}

void fillSTDSCR()
{
	for(int l=0; l<LINES; l++)
		printSequence(stdscr,l,0,-1,stdscrCHAR);
}

void printLand(WINDOW * win)
{
	int rows, cols;
	getmaxyx(win, rows, cols);
	rows -= 2;
	cols -= 1;
	// printIsLand
	// We must leave a space of at least 2 between two pieces of land
	printSequence(win,6,25,5,landCHAR);
	printSequence(win,7,15,25,landCHAR);
	printSequence(win,8,15,45,landCHAR);
	printSequence(win,9,20,39,landCHAR);
	printSequence(win,10,18,25,landCHAR);
	printSequence(win,11,15,30,landCHAR);
	printSequence(win,12,17,20,landCHAR);
	printSequence(win,13,27,5,landCHAR);
	// printTopLand
	printSequence(win,1,1,75,landCHAR);
	printSequence(win,2,1,70,landCHAR);
	printSequence(win,3,46,10,landCHAR);
	printSequence(win,4,48,5,landCHAR);
	// printBotLand
	printSequence(win,rows-4,39,4,landCHAR);
	printSequence(win,rows-3,35,16,landCHAR);
	printSequence(win,rows-2,7,-1,landCHAR);
	printSequence(win,rows-1,4,-1,landCHAR);
	printSequence(win,rows,2,-1,landCHAR);
	// printRightLand
	printSequence(win,3,cols-5,-1,landCHAR);
	printSequence(win,4,cols-8,-1,landCHAR);
	printSequence(win,5,cols-10,-1,landCHAR);
	printSequence(win,6,cols-12,-1,landCHAR);
	printSequence(win,7,cols-9,-1,landCHAR);
	printSequence(win,8,cols-11,-1,landCHAR);
	printSequence(win,9,cols-10,-1,landCHAR);
	printSequence(win,10,cols-9,-1,landCHAR);
	printSequence(win,11,cols-5,-1,landCHAR);
	printSequence(win,12,cols-7,-1,landCHAR);
	printSequence(win,13,cols-10,-1,landCHAR);
	printSequence(win,14,cols-13,-1,landCHAR);
	printSequence(win,15,cols-17,-1,landCHAR);
	printSequence(win,16,cols-20,-1,landCHAR);
	// printLeftLand
	printSequence(win,3,1,33,landCHAR);
	printSequence(win,4,1,23,landCHAR);
	printSequence(win,4,1,13,landCHAR);
	printSequence(win,5,1,7,landCHAR);
	printSequence(win,6,1,9,landCHAR);
	printSequence(win,7,1,7,landCHAR);
	printSequence(win,8,1,8,landCHAR);
	printSequence(win,9,1,10,landCHAR);
	printSequence(win,10,1,11,landCHAR);
	printSequence(win,11,1,9,landCHAR);
	printSequence(win,12,1,6,landCHAR);
	printSequence(win,13,1,8,landCHAR);
	printSequence(win,14,1,5,landCHAR);
	printSequence(win,15,1,3,landCHAR);
	printSequence(win,16,1,1,landCHAR);
}


// TODO check whether pointers should be used, instinct says so
// 		it will be good practice
// printMap(WINDOW *, PLAYER[])
// In order to make haste, didn't make it easily upgradable for >2 players
//@usage:	
bool printMap(WINDOW * win, PLAYER player[])
{
	printLand(win);
	// First of all I check for a crash between the player
	if( player[0].x == player[1].x )
		if ( player[0].y == player[1].y )
		{
			mvwaddch(win,player[0].y,player[0].x, crashCHAR);
			playersReset(player);
			return false;
		}
	// Now I check whether any of them have crashed with land
	for(int l = 0; l < 2; l++)
		// Check the character where the boat would be printed
		if(	mvinch(player[l].y, player[l].x) == landCHAR )
		{
			mvwaddch(win,player[l].y,player[l].x, crashCHAR);
			playerReset(player[l]);
		}
		else
			printPlayer(win,player[l]);

	// TODO add barrier for when the folks try to win by getting from start to 
	// end in reversed way
	
 	return true;
}

//bool printAnimation(int *xs, int *ys, WINDOW *win /* MORE ARGUMENTS */); 
