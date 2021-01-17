/*
 * 'Sailing-game' by Panagiotidis Giorgos, Chatziioannou Lamprinos
 */

/* 
 * NCURSES:  
 * STDIO:
 * MENU:
 * STDLIB: use of the exit() function TODO check whether it is necessary
 */
#include <ncurses.h>
#include <stdio.h>
#include <menu.h>
#include <stdlib.h>


/*
 * Environment Variables
 */
	
// gameTITLE
// - not much to say for this one -
#define gameTITLE "MALAKIA"

// frameDURATION
//		@usage:
// 		@details:
#define frameDURATION 3000

// gameWIDTH, gameHEIGHT
//		@usage:
// 		@details:
#define gameWIDTH 80
#define gameHEIGHT 40

// menuWIDTH, menuHEIGHT
//		@usage:
// 		@details:
#define menuWIDTH 50
#define menuHEIGHT 18
#define menuXCOORD ( COLS - menuWIDTH )/2
#define menuYCOORD ( LINES - menuHEIGHT )/2

// landCHAR, landCOLOR
// seaCHAR, seaCOLOR
// boatCHAR, boatCOLOR
//		@usage:
// 		@details:
#define landCHAR '*'
#define seaCHAR '-'
#define boatCHAR 'b'
// We need to do some research on that
// TODO associate it with COLOR_PAIR()
#define landCOLOR COLOR_BLACK
#define seaCOLOR COLOR_BLACK
#define boatCOLOR COLOR_BLACK


/*
 * Environment Functions
 * (To make the code more readable)
 */
#define arraySIZE(l) (sizeof(l) / sizeof(l[0]))


/*
 * Global variables
 * 		(Even though this isn't the optimal way of doing things)
 */
char *menuOptions[] = {"Play","Difficulty","Controls","Credits","Exit"};

/*
 * Our structures
 */

//typedef struct _random
//{
//}random;

/*
 * Function Declarations
 */

// TODO add description and complete the function
void printInMiddle(WINDOW *win, int startY, int startX, int width, char *string, chtype color);

/*
 * EXTRA FEATURES, consider adding
 *
 * Check whether we should use cbreak or raw
 * Enable the usage of the function keys: F1, F2, F...
 * keypad(stdscr, TRUE);
 *
 * These two lines should be followed by a comment explaining that COLS and LINES variables are set during initscr();
 * Serving as the dimension of the terminal
 * posX = ( COLS - menuWIDTH )/2;
 * posY = ( LINES - menuHEIGHT)/2;
 */

int main()
{
	/*
	 * Initialization
	 */
	
	// Check whether the terminal has color integration
	// -> TRUE: continue with the program
	// -> FALSE: show error
	if ( ! has_colors() )
	{
		printf("%s\n%s\n",
			   	"Please try a different terminal emulator.", 
				"The current one does not support color.");
		exit(1);
	}

	// We start the curses mode ( enabling windows functionality )
	initscr();
	// Checking whether the terminal is of valid size ( if it is too small the
	// program would, otherwise, malfunction
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
	raw();
	// We don't want to see the characters as they're typed in:
	// ( user's input )
	noecho();
	// Make the cursor invisible
	curs_set(0);
	// Enable colors and set color pairs, see README FOR MORE
	start_color();
	// TODO You may want to check the associated values land/sea/boatCOLOR 
	// to make everything more readable
	// Also take into consideration the menu colors
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_BLUE, COLOR_BLACK);
	
	/*
	 * Variables
	 */
	
	// ch .... getch()
	int ch,
		posX,
		posY;

	/*
	 * Menu initialization
	 */	

	// Related to our menu
	WINDOW *menuWin;
	MENU *mainMenu;
	ITEM *currentItem,
		 **myItems;
	
	// The number of options in our menu is equal to the number of elements
	// in the menuOptions array, initialized globally
	int noOfoptions = arraySIZE(menuOptions);
	// Memory for the items we want to create must be reserved:
	// - we reserve memory for noOfoptions + 1 items, since we want to add
	// a NULL pointer at the end to signal the end of our array
	// - the pointer myItems simply points to the beginning of our ITEM * array
	// - TODO the (ITEM **) is to turn the pointer returned by calloc into a ITEM**
	// type of pointer
	myItems = (ITEM **)calloc(noOfoptions + 1, sizeof(ITEM *));
	// Filling the array with our menu options, described in our menuOptions ar
	for ( int l = 0; l < noOfoptions; l++ )
		// The second argument of the new_item function is the description
		// of that item, displayed right next to the item. Since we want to 
		// keep the interface minimal we decided against having a description.
		// We supply a NULL pointer to have an empty description.
		myItems[l] = new_item( menuOptions[l], NULL );
	// We need to have the last item as null so that the menu initialization
	// stops at the end of the array
	myItems[noOfoptions] = (ITEM *)NULL;
	//  We create a menu, with items all of our initial options, 
	//  in the desired form
	mainMenu = new_menu((ITEM **)myItems);

	// Create the window for our menu
	// TODO add a better description
  	menuWin = newwin(menuHEIGHT, menuWIDTH, menuXCOORD, menuYCOORD);
	// Set main window and subwindow for our menu 
	// (simply creating it would not be enough, the two of them must be
	// linked together)
	set_menu_win( mainMenu, menuWin );
	// The derwin() function is the same as subwin(), creating a virtual
	// subwindow in the window supplemented, with the only difference being
	// that the coordinates are relative to our window
	// ( not to stdscr, the standard terminal screen )
	set_menu_sub( mainMenu, derwin( menuWin, 6, 38, 3, 1));
	// TODO once again
 	set_menu_format( mainMenu, 5, 1 );
	// Set which character should point at our current option:
	set_menu_mark( mainMenu, "=>");
	// Print a border for our menu and a title:
	// TODO comment the arguments
	box( menuWin, 0, 0 );
	printInMiddle( menuWin, 1, 0, menuWIDTH,gameTITLE, COLOR_PAIR(1) );
	// We set the menu options: 
	set_menu_fore( mainMenu, COLOR_PAIR(1) | A_REVERSE );
	// set_menu_back, set_menu_grey
	//  We make our menu visible
	post_menu( mainMenu );
	wrefresh( menuWin );
	// TODO menu_scroll.c
	
	// Add this line at the end of the menu interface
	// unpost_menu( mainMenu );
	
	/*
	 * Main structure
	 */
	

	// Releasing memory allocated during the creation of our menu
	free_menu( mainMenu );
	endwin();
	return 1;
}
