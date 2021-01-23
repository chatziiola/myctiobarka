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

#define gameTITLE "SAILING GAME"
#define gameWIDTH 80
#define gameHEIGHT 20

#define menuWIDTH 50
#define menuHEIGHT 18
#define menuOPTIONS 4

#define landCHAR '#'
#define crashCHAR 'X'
#define winCHAR 'Z'

// PLAYER(type)
// 		@desc:
typedef struct _game_player{
	char course[3];
	char name[20];
	int character;
	int x,y;
	int crashes;
	int noOfChoices;
}PLAYER;

void airArrow(WINDOW *win,int intensity, int direction);
void initCurses(WINDOW **gameWin, WINDOW **menuWin);
void initPlayers(WINDOW * win,PLAYER *players);
void setPlayerChar(WINDOW *win, PLAYER *player, int winLength);
int setCourse(WINDOW *plrDataWin, PLAYER *player);
void setPlayerName(WINDOW *win, PLAYER *player, int winLength);
void showChoices(WINDOW *win, PLAYER player);
void showControls(WINDOW *ctrlWin);
void showCredits(WINDOW *credWin);
void showExit(WINDOW *exitWin);
void showIntro(WINDOW *introWin);
void showMenu(WINDOW *menuWin);
void resetPlayerLocation(PLAYER *player, char index);
void playATurn(PLAYER *players, WINDOW *gameWin, WINDOW *menuWin, WINDOW *scrTitleWin, WINDOW *plrDataWin, WINDOW *wndDataWin);
void wprintInMiddle(WINDOW *win, int row, char *string, int attrs);
void printLand(WINDOW * win);
bool printMap(WINDOW * win, PLAYER *player);
void printPlData(WINDOW *win, PLAYER *players);
void wprintTimesChar(WINDOW * win,int startY, int startX, int length, int character);
void printTitle(WINDOW *titleWin);

char *menuOptions[] = {"Play","Controls","Credits","Exit",(char *)NULL};

int main()
{
	// Pointers to windows are ncurses library basic data structure, helping us
	// manipulate the screen to our needs. for more see `man ncurses.h`
	WINDOW *gameWin, *menuWin;
	time_t realRandom;

	// Initialize the ncurses window functionality and check that the terminal
	// is of at least the minimum size, as well as that there is color support
	initCurses(&gameWin, &menuWin);	

	// Uses menuWin as a canvas on which we draw a menu with our menuOptions
	// character array
	showMenu(menuWin);	

	PLAYER players[2];
	
	// Smaller windows so that I can more easily use general functs to display
	// specific information, while enhancing readability at the same time
	WINDOW *scrTitleWin = derwin(gameWin, 3, 30, 0, 25);
	WINDOW *plrDataWin  = derwin(gameWin,1,60,18,2); 
	WINDOW *wndDataWin	= derwin(gameWin,1,10,18,66);

	// printLand(gameWin);
	// wrefresh(gameWin);
	showIntro(menuWin);

	initPlayers(menuWin, players);
	srand((unsigned) time(&realRandom));

	while(true)
		playATurn(players, gameWin, menuWin, scrTitleWin, plrDataWin, wndDataWin);

	// Releasing memory allocated during the creation of our menu, end ncurses
	endwin();
	return 0;
}

// initCurses: 
// 		@desc: simply initializing the windows and color functionality,
// 		as well as, adding some extra nice 'refining' touches
void initCurses(WINDOW **gameWin, WINDOW **menuWin)
{
	// We start the curses mode ( enabling windows functionality )
	initscr();
	// Check whether the terminal has color integration
	if ( ! has_colors() )
	{
		printf("%s\n%s\n",
			   	"Please try a different terminal emulator.", 
				"The current one does not support color.");
		exit(1);
	}

	// Checking whether the terminal is of valid size ( if it is too small the
	// program would malfunction
	// THE COLS/LINES variables are initialized with initscr(), hidden in the
	// ncurses documentation. They reveal the standard screen (stdscr) or term
	// window dimensions
	if ( (COLS < gameWIDTH) ||  (LINES < gameHEIGHT) )
	{
		endwin();
		printf("%s\n%s",
				"The dimensions of your terminal are too small for the game to function properly.",
				"Try launching again after resizing the emulator");
		exit(1);
	}
	// Disable line buffering so that we get each keypress in real time
	// without waiting for a <NL> or <CR> character
	// There was also the raw() option but this one was selected so that
	// the "standard" keybindings would still function properly
	cbreak();
	// We don't want to see the characters as they're typed in:
	// ( user's input )
	noecho();
	// Make the cursor invisible
	curs_set(0);
	// Enable colors and set color pairs
	start_color();
	// Also take into consideration the menu colors
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_BLUE, COLOR_BLACK);
	init_pair(4, COLOR_RED, COLOR_BLACK);
	// Crash Color
	init_pair(5, COLOR_YELLOW, COLOR_BLACK);
	// Player color
	init_pair(6, COLOR_CYAN, COLOR_BLACK);
	*gameWin = newwin(gameHEIGHT,gameWIDTH, (LINES-gameHEIGHT)/2, (COLS-gameWIDTH)/2);
	*menuWin = newwin(menuHEIGHT,menuWIDTH, (LINES-menuHEIGHT)/2, (COLS-menuWIDTH)/2);
	// Enabling us to get keyboard input ( not limited to the actual letters )
	// at the windows passed as argument
	keypad(stdscr, TRUE);
	keypad(*gameWin,true);
}

// wprintTimesChar:
// 		@desc: prints the 'character' 'length' times, starting from
// 		'startY','startX' coordinates of the 'win' window
void wprintTimesChar(WINDOW * win,int startY, int startX, int length, int character)
{
	// Move the cursor to the starting location
	wmove(win, startY, startX);
	// If the user has passed a negative value it means that they want 
	// to fill the rest of the line
	if(length < 0)
	{
		// Get the dimensions of our window, storing, the Y coord at startY and
		// X coord at length. This way we can then subtract startX from length
		// and have, exactly, the number of characters we need to fill the line
		// (without printing on the window's border)
		getmaxyx(win, startY, length);
		length -= startX+1; 
	}
	// Add the characters
	for(int l=0; l < length; l++ )
		waddch(win,character);	
}

// wprintInMiddle:
// 		@desc: prints the 'string' passed as an argument in the middle of the 
// 		given window *win, at line 'row' with the attributes that were passed
void wprintInMiddle(WINDOW *win, int row, char *string, int attrs)
{
	// These two variables will hold the dimensions of our window
	int maxRows, maxCols;
	// getmaxyx function simply gives us the maximum y and x for that window
	getmaxyx(win, maxRows, maxCols);

	// If the value of row is negative then the string will be printed
	// |row| lines from the bottom of the window
	if (row < 0)
		row += maxRows -1;

	wattron(win, attrs);
	// Print string in win after moving to y line and column () so that the
	// string is placed in the middle of the window
	mvwprintw(win, row, (maxCols-strlen(string))/2, string);
	wattroff(win, attrs);
}

// setPlayerChar:
// 		@desc: Uses the window 'win' to interact with the user, printing a
// 		prompt to give the character they (users) want their boat to be
// 		represented by
void setPlayerChar(WINDOW *win, PLAYER *player, int winLength)
{
	// Specific character type for the input collected using the ncurses funcs
	int playerChar;
	int	confirmation;
	// Clear the window, print the prompt and refresh window so that the user
	// sees it
	wclear(win);
	wprintInMiddle(win,1,"Please enter your character:",A_NORMAL);
	wrefresh(win);
	// Get input
	playerChar = wgetch(win);
	// Clear the window, print the new prompt
	wclear(win);
	wprintInMiddle(win,1,"Is it ok? (Y/N)",A_NORMAL);
	mvwaddch(win,2, winLength/2, playerChar|A_BOLD);
	wrefresh(win);
	confirmation = wgetch(win);
	// If the character we received on the confirmation screen is 'Y', as
	// urged or 'Enter'
	if ( (confirmation == 'Y') || (confirmation == 10) )
		player->character = playerChar;
	else
		// Repeat the same process till we have the necessary confirmation from
		// the user
		setPlayerChar(win, player, winLength);
}

// setPlayerName:
// 		@desc: Uses the window 'win' to interact with the user, printing a
// 		prompt and initially setting their name string. When finished (the pro
// 		cess may repeat itself for as many times needed to ensure that the user
// 		passed the name they wanted), will call setPlayerChar() to complete
// 		this part of the player's initialization
void setPlayerName(WINDOW *win, PLAYER *player, int winLength)
{
	char *name;
	int confirmation;
	name = (char *)malloc(20);
	wclear(win);
	wprintInMiddle(win,1,"Please enter your name:",A_NORMAL);
	mvwaddch(win,2,10,'>');
	wrefresh(win);
	echo();
	wgetstr(win,name);
	wclear(win);
	wprintInMiddle(win,1,"Is it ok? (Y/N)",A_NORMAL);
	wprintInMiddle(win,2,name,A_NORMAL);
	noecho();
	wrefresh(win);
	confirmation = wgetch(win);
	if ( ( confirmation == 'Y') || (confirmation == 10))
	{
		strcpy(player->name, name);
		setPlayerChar(win, player, winLength);
	}
	else
		setPlayerName(win, player, winLength);
}

// resetPlayerLocation:
// 		@desc: Changes the 'player' values for x and y according to the index 
// 		passed as the second parameter. If index == 0 then player must be 
// 		'PLAYER 1' and their location is 're'set to 'PLAYER 1' start, while
// 		if index == 1, they must be 'PLAYER 2' 
void resetPlayerLocation(PLAYER *player, char index)
{
	// This way we also allow for future updates -> More PLAYERS
	player->x = 50;
	player->y = 7-index;
}

// initPlayers:
// 		@desc: This function is meant to be used to resiz
void initPlayers(WINDOW * win,PLAYER *players)
{
	resetPlayerLocation(&players[0], 0);
	resetPlayerLocation(&players[1], 1);
	players[0].crashes = 0;
	players[1].crashes = 0;
	int maxY,maxX;
	getmaxyx(win,maxX,maxX);
	wclear(win);
	printTitle(win);
	wprintInMiddle(win,7,"Now let's start with your characters!!", A_NORMAL);
	wprintInMiddle(win,8,"Player 1, your turn", A_BOLD);
	wrefresh(win);
	setPlayerName(derwin(win,5,maxX-2,10,1), &players[0], maxX);
	wprintInMiddle(win,8,"Player 2, your turn", A_BOLD);
	wrefresh(win);
	setPlayerName(derwin(win,5,maxX-2,10,1), &players[1], maxX);
}

// setCourse:
// 		@desc: Get the user's input to fill the "course" array for 'player'
// 		while also allowing for backspace (deletion) functionality and visual
// 		feedback of those choices
int setCourse(WINDOW *plrDataWin, PLAYER *player)
{
	int ch;
	while(true)
	{
		ch = getch();
		switch(ch)
		{
			case KEY_UP:
				if(player->noOfChoices<3)
				{
					player->course[player->noOfChoices] = 'U';
					player->noOfChoices++;
				}
				break;
			case KEY_DOWN:
				if(player->noOfChoices<3)
				{
					player->course[player->noOfChoices] = 'D';
					player->noOfChoices++;
				}
				break;
			case KEY_RIGHT:
				if(player->noOfChoices<3)
				{
					player->course[player->noOfChoices] = 'R';
					player->noOfChoices++;
				}
				break;
			case KEY_LEFT:
				if(player->noOfChoices<3)
				{
					player->course[player->noOfChoices] = 'L';
					player->noOfChoices++;
				}
				break;
			// Backspace
			case 127:
				if(player->noOfChoices>0)
					player->noOfChoices--;
				break;
			case 10:
				return 1;
		}
		// Show the options
		showChoices(plrDataWin, *player);
	}
}

void printPlData(WINDOW *win, PLAYER *players)
{
	mvwprintw(win,0,0,"%s: (%d) x:%d y:%d", players[0].name,
			players[0].crashes, players[0].x, players[0].y);
	mvwprintw(win,0,30,"| %s: (%d) x:%d y:%d", players[1].name,
			players[1].crashes, players[1].x, players[1].y);
}

void playerMove(PLAYER *player, char direction)
{
	if ( (direction == 'U') || (direction == 0) )
		player->y -= 1;
	if ( (direction == 'D') || (direction == 2) )
		player->y += 1;
	if ( (direction == 'R') || (direction == 1) )
		player->x += 1;
	if ( (direction == 'L') || (direction == 3) )
		player->x -= 1;
}

void playATurn(PLAYER *players, WINDOW *gameWin, WINDOW *menuWin, WINDOW *scrTitleWin, WINDOW *plrDataWin, WINDOW *wndDataWin)
{
	// Algorithm
	// Printmap and current status
	// -> map, pldata, wind (calc it as well) && title: Current data, sleep(5);
	// Change title to "Player 1, select your course"
	// only KEY_UP/RIGHT/LEFT/DOWN acceptable for input and backspace deletes 
	// last choice
	// Now the same for Player 2
	// showAnimation:
	// -> Print a different "Frame, lets say 4+intensity" in which every
	// ship mvs one block,  evaluating the condition, ofc before moving
	// -> that last part should use a bool type var so that we know whether any
	// of the players have won -> you can add it to player type
	// evaluate the condition for each player-> if n1 has won
	// call that function again recursive from that point of view
	// Frame for all of the game
	box(gameWin,0,0);
	// Print the land with the players on it
	printMap(gameWin,players);
	// Print the players' coordinates and crashes at the bottom of the screen
	printPlData(plrDataWin,players);
	// Calculate with rand() the "air"
	int airIntensity = rand()%4;
	int airDirection = rand()%4;
	players[0].noOfChoices = 0;
	players[1].noOfChoices = 0;
	// Print it on the screen so that players know beforehand about its 
	// direction and intensity
	airIntensity++;
	airArrow(wndDataWin, airIntensity, airDirection);
	// Clear a box at the "middle top" of the screen so that we can show a
	// title for each phase of the game
	wclear(scrTitleWin);
	box(scrTitleWin,0,0);
	wprintInMiddle(scrTitleWin, 1, "Current View", A_BOLD);
	// All the windows I am currently using are simply means to an end, they're
	// not actual windows, but to make the code more readable and easily 
	// expandable I've decided in splitting the gameWin into different
	// "subwindows", so that only the gameWin needs to be refreshed
	wrefresh(gameWin);
	getch();
	// We wait 5 seconds so that the players know where they are
	// And now we get their input for their motions
	wclear(scrTitleWin);
	box(scrTitleWin,0,0);
	wprintInMiddle(scrTitleWin, 1, "Player 1: Choose your course", A_BOLD | A_BLINK);
	wclear(plrDataWin);
	wrefresh(scrTitleWin);
	wrefresh(plrDataWin);
	setCourse( plrDataWin, &players[0]);
	wprintInMiddle(scrTitleWin, 1, "Player 2: Choose your course", A_BOLD | A_BLINK);
	wclear(plrDataWin);
	wrefresh(scrTitleWin);
	wrefresh(plrDataWin);
	setCourse( plrDataWin, &players[1]);
	bool windRound = true;
	for(int l=0; l<3; l++)
	{
		if ( ((rand()%2)==0) && windRound )
		{
			for(int z=0; z < airIntensity; z++)
			{
				playerMove(&players[0], airDirection);
				playerMove(&players[1], airDirection);
				if ( printMap(gameWin, players) )
					exit(1);
				wclear(scrTitleWin);
				box(scrTitleWin,0,0);
				wprintInMiddle(scrTitleWin, 1, "WIND IS BLOWING", A_BOLD | COLOR_PAIR(1));
				wrefresh(gameWin);
				sleep(1);
			}
			windRound = false;
		}
		if ( l < players[0].noOfChoices )
			playerMove( &players[0], players[0].course[l]);
		if ( l < players[1].noOfChoices )
			playerMove( &players[1], players[1].course[l]);

		if ( printMap(gameWin, players) )
			exit(1);
		wrefresh(gameWin);
		sleep(1);
	}
	// If (unluckily) the wind didn't get its chance in between the player's 
	// motions then it has to "appear" at the end
	if ( windRound )
	{
		for(int z=0; z < airIntensity; z++)
		{
			playerMove(&players[0], airDirection);
			playerMove(&players[1], airDirection);
			if ( printMap(gameWin, players) )
				exit(1);
			wclear(scrTitleWin);
			box(scrTitleWin,0,0);
			wprintInMiddle(scrTitleWin, 1, "WIND IS BLOWING", A_BOLD | COLOR_PAIR(1));
			wrefresh(gameWin);
			sleep(1);
		}
	}
}

void printLand(WINDOW * win)
{
	int rows, cols;
	getmaxyx(win, rows, cols);
	rows -= 2;
	cols -= 1;
	wattron(win, COLOR_PAIR(2));
	// printIsLand
	// We must leave a space of at least 2 between two pieces of land
	wprintTimesChar(win,6,25,5,landCHAR);
	wprintTimesChar(win,7,15,25,landCHAR);
	wprintTimesChar(win,8,15,45,landCHAR);
	wprintTimesChar(win,9,20,39,landCHAR);
	// Barrier so that the boats can't go the other way
	wprintTimesChar(win,9,59,10,landCHAR | A_INVIS);
	wprintTimesChar(win,10,18,25,landCHAR);
	wprintTimesChar(win,11,15,30,landCHAR);
	wprintTimesChar(win,12,17,20,landCHAR);
	wprintTimesChar(win,13,27,5,landCHAR);
	// printTopLand
	wprintTimesChar(win,1,1,75,landCHAR);
	wprintTimesChar(win,2,1,70,landCHAR);
	wprintTimesChar(win,3,46,10,landCHAR);
	wprintTimesChar(win,4,48,5,landCHAR);
	// printBotLand
	wprintTimesChar(win,rows-4,39,4,landCHAR);
	wprintTimesChar(win,rows-3,35,16,landCHAR);
	wprintTimesChar(win,rows-2,7,-1,landCHAR);
	// printRightLand
	wprintTimesChar(win,3,cols-5,-1,landCHAR);
	wprintTimesChar(win,4,cols-8,-1,landCHAR);
	wprintTimesChar(win,5,cols-10,-1,landCHAR);
	wprintTimesChar(win,6,cols-12,-1,landCHAR);
	wprintTimesChar(win,7,cols-9,-1,landCHAR);
	wprintTimesChar(win,8,cols-11,-1,landCHAR);
	wprintTimesChar(win,9,cols-10,-1,landCHAR);
	wprintTimesChar(win,10,cols-9,-1,landCHAR);
	wprintTimesChar(win,11,cols-5,-1,landCHAR);
	wprintTimesChar(win,12,cols-7,-1,landCHAR);
	wprintTimesChar(win,13,cols-10,-1,landCHAR);
	wprintTimesChar(win,14,cols-13,-1,landCHAR);
	wprintTimesChar(win,15,cols-17,-1,landCHAR);
	wprintTimesChar(win,16,cols-20,-1,landCHAR);
	// printLeftLand
	wprintTimesChar(win,3,1,33,landCHAR);
	wprintTimesChar(win,4,1,23,landCHAR);
	wprintTimesChar(win,4,1,13,landCHAR);
	wprintTimesChar(win,5,1,7,landCHAR);
	wprintTimesChar(win,6,1,9,landCHAR);
	wprintTimesChar(win,7,1,7,landCHAR);
	wprintTimesChar(win,8,1,8,landCHAR);
	wprintTimesChar(win,9,1,10,landCHAR);
	wprintTimesChar(win,10,1,11,landCHAR);
	wprintTimesChar(win,11,1,9,landCHAR);
	wprintTimesChar(win,12,1,6,landCHAR);
	wprintTimesChar(win,13,1,8,landCHAR);
	wprintTimesChar(win,14,1,5,landCHAR);
	wprintTimesChar(win,15,1,3,landCHAR);
	wprintTimesChar(win,16,1,1,landCHAR);
	wattroff(win, COLOR_PAIR(2));
	wprintTimesChar(win,rows-1,1,-1,ACS_HLINE);
}

bool printMap(WINDOW * win, PLAYER *player)
{
	// Print the terrain, deleting everything else in the background
	wclear(win);
	printLand(win);
	// Check for a crash between the two players
	if( (player[0].x == player[1].x) && ( player[0].y == player[1].y ))
	{
			attron( COLOR_PAIR(5) );
			mvwaddch(win,player[0].y,player[0].x, crashCHAR);
			attroff( COLOR_PAIR(5) );
			player[0].crashes++;
			player[1].crashes++;
			resetPlayerLocation(&player[0], 0);
			resetPlayerLocation(&player[1], 1);
	}
	// Check, individually, whether any of them have crashed with the land
	for(int l = 0; l < 2; l++)
	{
		// Check the character where the boat would be printed
		if(	(mvwinch(win,player[l].y, player[l].x)&A_CHARTEXT) == landCHAR )
		{
			// If so print a crashCharacter at their location
			attron( COLOR_PAIR(5) );
			mvwaddch(win,player[l].y,player[l].x, crashCHAR);
			attroff( COLOR_PAIR(5) );
			// Move them back to their starting location
			player[l].crashes++;
			resetPlayerLocation(&player[l],l);
		}
		else if( (mvwinch(win,player[l].y, player[l].x)&A_CHARTEXT) == winCHAR )
			// printWinScreen we need to fix that a bit
			return true;
		else
		{
			attron( COLOR_PAIR(6) );
			mvwaddch(win, player[l].y, player[l].x, player[l].character);
			attroff( COLOR_PAIR(6) );
		}
	}
	
 	return false;
}

void printTitle(WINDOW *titleWin)
{
		box(titleWin,0,0);
		wprintInMiddle(titleWin,2, gameTITLE, A_BOLD);
		wprintTimesChar(titleWin, 3, 1, -1, ACS_HLINE );
}

void airArrow(WINDOW *win,int intensity, int direction)
{
	// This function simply prints the arrow we want to describe the wind
	// There are 4 types of arrows based on direction (UP,DOWN,LEFT,RIGHT)
	// and 4 categories based on intensity (lvl1,2,3,4) each of which is 
	// "expressed" by a different color
	mvwaddch(win,0,0,ACS_VLINE);
	wattron(win, COLOR_PAIR(intensity));
	// Going clockwise
	wprintw(win," WIND: %d  ", intensity);
	switch(direction)
	{
		case 0:
			waddch(win,ACS_UARROW);
			break;
		case 1:
			waddch(win,ACS_RARROW);
			break;
		case 2:
			waddch(win,ACS_DARROW);
			break;
		case 3:
			waddch(win,ACS_LARROW);
			break;
	}
	wattroff(win, COLOR_PAIR(intensity));
}

void showChoices(WINDOW *win, PLAYER player)
{
	wclear(win);

	mvwprintw(win,0,0, "%s: Choices  %d:", player.name, player.noOfChoices);
	if ( player.noOfChoices > 0 )
		wprintw(win, " %c", player.course[0]); 
	if ( player.noOfChoices > 1 )
		wprintw(win, " %c", player.course[1]); 
	if ( player.noOfChoices > 2 )
		wprintw(win, " %c", player.course[2]); 
	wrefresh(win);
}

void showMenu(WINDOW *menuWin)
{
	MENU *mainMenu;
	ITEM *playerChoice,
		 *currentItem,
		 **myItems;
	int ch;
	
	// The number of options in our menu is equal to the number of elements
	// in the menuOptions array, initialized globally
	// Memory for the items we want to create must be reserved:
	// - the pointer myItems simply points to the beginning of our ITEM * array
	// - the (ITEM **) is to turn the pointer returned by calloc into a ITEM**
	// type of pointer
	myItems = (ITEM **)calloc(menuOPTIONS, sizeof(ITEM *));

	// Filling the array with our menu options, described in our menuOptions ar
	// The second argument of the new_item function is the description
	// of that item, displayed right next to the item. Since we want to 
	// keep the interface minimal we decided against having a description.
	// We supply a NULL pointer to have an empty description.
	// The set_item_userptr() function is responsible for the function that
	// runs when the specific item is selected
	
	for(int l = 0; l < menuOPTIONS; l++)
		myItems[l] = new_item( menuOptions[l], NULL);
	
	wclear(menuWin);
	keypad(menuWin, true);
	//  We create a menu, with items all of our initial options, 
	//  in the desired form
	mainMenu = new_menu((ITEM **)myItems);
	// Set main window and subwindow for our menu 
	// (simply creating it would not be enough, the two of them must be
	// linked together)
	set_menu_win(mainMenu, menuWin);
	// The derwin() function is the same as subwin(), creating a virtual
	// subwindow in the window supplemented, with the only difference being
	// that the coordinates are relative to our window
	// ( not to stdscr, the standard terminal screen )
	set_menu_sub(mainMenu, derwin(menuWin, menuOPTIONS, 20, (menuHEIGHT-menuOPTIONS-2)/2,(menuWIDTH-20)/2));
	// Set which character should point at our current option:
	set_menu_mark(mainMenu, " => ");
	// Print a border for our menu and a title:
	// 0,0 gives default characters for the vertical and horizontal lines
	box( menuWin, 0, 0 );
	// Print the gameTITLE in the middle of the 1st line of our menu window 
	printTitle(menuWin);
	// We set the menu options: 
	//  We make our menu visible, and initialize our 'choiceIsMade' boolean
	//  to false
	//	wprintInMiddle(stdscr, -1, "F1 to exit", COLOR_PAIR(1)| A_BLINK);
	refresh();

	post_menu(mainMenu);
	wrefresh(menuWin);
	
	ch = 0;
	while( ch != 10 )
	{
		ch = wgetch(menuWin);
		switch(ch)
		{
			case KEY_DOWN:
				menu_driver(mainMenu, REQ_DOWN_ITEM);
				break;
			case KEY_UP:
				menu_driver(mainMenu, REQ_UP_ITEM);
				break;
			// The value 10 is associated with the enter key
			case 10:
				playerChoice = current_item(mainMenu);
		}
		wrefresh(menuWin);
	}
	wclear(menuWin);
	wrefresh(menuWin);
	unpost_menu(mainMenu);
	for(int l=0; l < menuOPTIONS; l++)
		free_item(myItems[l]);
	free_menu(mainMenu);

	if(strcmp(item_name(playerChoice),"Controls") == 0)
		showControls(menuWin);
	else if(strcmp(item_name(playerChoice),"Credits") == 0)
		showCredits(menuWin);
	else if(strcmp(item_name(playerChoice),"Exit") == 0)
		showExit(menuWin);
}

void showCredits(WINDOW *credWin)
{
		printTitle(credWin);
		wprintInMiddle(credWin,5, "Created by:", A_NORMAL);
		wprintInMiddle(credWin,6, "Lamprinos Chatziioannou", A_NORMAL);
		wprintInMiddle(credWin,7, "Georgios Panagiotidis", A_NORMAL);
		wprintInMiddle(credWin,-2, "Press any key to exit.", A_NORMAL);
		wrefresh(credWin);
		getch();
		showMenu(credWin);
}

void showExit(WINDOW *exitWin)
{
		printTitle(exitWin);
		wprintInMiddle(exitWin,6, "Thanks for playing our game", A_BOLD);
		wprintInMiddle(exitWin,8, "Created by:", A_NORMAL);
		wprintInMiddle(exitWin,9, "Lamprinos Chatziioannou", A_NORMAL);
		wprintInMiddle(exitWin,10, "Georgios Panagiotidis", A_NORMAL);
		wprintInMiddle(exitWin,-2, "Press any key to exit.", A_NORMAL);
		wrefresh(exitWin);
		getch();
		endwin();
		exit(1);
}

void showControls(WINDOW *ctrlWin)
{
		printTitle(ctrlWin);
		wprintInMiddle(ctrlWin,6, "Player 1:", A_BOLD);
		wprintInMiddle(ctrlWin,7, "Moves with the arrow keys, 3 keystrokes per turn", A_NORMAL);
		wprintInMiddle(ctrlWin,10, "Player 2:", A_BOLD);
		wprintInMiddle(ctrlWin,11, "Moves with WASD, also 3 keystrokes per turn", A_NORMAL);
		wprintInMiddle(ctrlWin,-2, "Press any key to exit.", A_NORMAL);
		wrefresh(ctrlWin);
		getch();
		showMenu(ctrlWin);
}

void showIntro(WINDOW *introWin)
{
		printTitle(introWin);
		wprintInMiddle(introWin, 4, "Hello friends and welcome to the", A_NORMAL);
		wrefresh(introWin);
		sleep(1);
		wprintInMiddle(introWin, 5, gameTITLE, A_BOLD | COLOR_PAIR(4) | A_BLINK);
		wrefresh(introWin);
		sleep(2);
		wprintInMiddle(introWin, 7, "In this, multiplayer, game the two of you", A_NORMAL);
		wrefresh(introWin);
		sleep(1.5);
		wprintInMiddle(introWin, 8, "Are trying to beat each other on your way to", A_NORMAL);
		wrefresh(introWin);
		sleep(1.5);
		wprintInMiddle(introWin, 9, "The finish line! Each of you has up to 5", A_NORMAL);
		wrefresh(introWin);
		sleep(1.5);
		wprintInMiddle(introWin, 10, "keystrokes per turn, but...", A_NORMAL);
		wrefresh(introWin);
		sleep(1.5);
		wprintInMiddle(introWin, 11, "WATCH OUT!", A_BOLD);
		wrefresh(introWin);
		sleep(2);
		wprintInMiddle(introWin, 12, "You do not know when wind will strike!", A_NORMAL);
		wrefresh(introWin);
		sleep(4);
		wprintInMiddle(introWin, -1, "Press any key to continue", A_BLINK);
		wrefresh(introWin);
		getch();
		wclear(introWin);
		printTitle(introWin);
		wprintInMiddle(introWin, 4, "However, fear not!", A_NORMAL);
		wrefresh(introWin);
		sleep(2);
		wprintInMiddle(introWin, 6, "You will know everything beforehand!", A_NORMAL);
		wrefresh(introWin);
		sleep(2);
		wprintInMiddle(introWin, 7, "Just make sure to pay attention to ", A_NORMAL);
		wrefresh(introWin);
		sleep(1.5);
		wprintInMiddle(introWin, 8, "the wind arrow and intensity on the bottom", A_NORMAL);
		wrefresh(introWin);
		sleep(1.5);
		wprintInMiddle(introWin, 9, "right corner, giving you both intensity", A_NORMAL);
		wrefresh(introWin);
		sleep(1.5);
		wprintInMiddle(introWin, 10, "AND DIRECTION!", A_BOLD);
		wrefresh(introWin);
		sleep(1.5);
		wprintInMiddle(introWin, 12, "Use them wisely", A_UNDERLINE);
		wrefresh(introWin);
		sleep(1);
		wprintInMiddle(introWin, 13, "You do not know when wind will strike!", A_NORMAL);
		wrefresh(introWin);
		sleep(4);
		wprintInMiddle(introWin, -1, "Press any key to continue", A_BLINK);
		wrefresh(introWin);
		getch();

}
