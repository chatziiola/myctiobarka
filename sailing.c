/*
 * 'Sailing-game' by Panagiotidis Giorgos, Chatziioannou Lamprinos
 */

/* 
 * NCURSES:  
 * STDIO:
 * MENU:
 * STDLIB: use of the exit() function TODO check whether it is necessary
 */
#include <stdio.h>
#include <menu.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

// gameTITLE
// - not much to say for this one -
#define gameTITLE "Sailing-game"
#define CTRLD 4

// frameDURATION
//		@usage:
// 		@details:
#define frameDURATION 3000

// gameWIDTH, gameHEIGHT
//		@usage:
// 		@details:
#define gameWIDTH 80
#define gameHEIGHT 20

// menuWIDTH, menuHEIGHT
//		@usage:
// 		@details:
#define menuWIDTH 50
#define menuHEIGHT 18

char *menuOptions[] = {"Play","Controls","Credits","Exit", (char *)NULL};

/* 
 * printInMiddle:
 * @params:
 * takes as arguments a window in which the string will be printed
 *  an integer row, the line in which we want the screen to be printed
 * (if row<0, then the string will be printed |row| lines from the bottom 
 * of the window), the string we want to print, and attributes attrs which in 
 * most cases will be the color
 */
void showMenu(char * answer);
void initialization();
void printInMiddle(WINDOW *win, int y, char *string, chtype color);

int main()
{
	char choice[10];

	// Ncurses general settings 
	initialization();	
	// Show menu and save the user's choice in the choice char arr
	showMenu(choice);	

	if(strcmp(choice,"Exit") == 0)
	{
		endwin();
		printf("%s\n","Thanks for playing our game");
		return 1;
	}
	
	// Releasing memory allocated during the creation of our menu
	endwin();
	return 1;
}

void printInMiddle(WINDOW *win, int row, char *string, chtype attrs)
{
	// These two variables will hold the dimensions of our window
	int maxRows, maxCols;
	// getmaxyx function simply gives us the maximum y and x for that window
	getmaxyx(win, maxRows, maxCols);

	// If the value of row is negative then the string will be printed
	// |row| lines from the bottom of the window
	if (row < 0)
		row = maxRows -1 + row;

	wattron(win, attrs);
	// Print string in win after moving to y line and column () so that the
	// string is placed in the middle of the window
	mvwprintw(win, row, (maxCols-strlen(string))/2, string);
	wattroff(win, attrs);
}	


void showMenu(char * answer)
{
	WINDOW *mainMenuWin;
	MENU *mainMenu;
	ITEM *playerChoice,
		 *currentItem,
		 **myItems;
	int ch;
	
	// The number of options in our menu is equal to the number of elements
	// in the menuOptions array, initialized globally
	int noOfoptions = sizeof(menuOptions) / sizeof(menuOptions[0]);
	// Memory for the items we want to create must be reserved:
	// - the pointer myItems simply points to the beginning of our ITEM * array
	// - the (ITEM **) is to turn the pointer returned by calloc into a ITEM**
	// type of pointer
	myItems = (ITEM **)calloc(noOfoptions, sizeof(ITEM *));

	// Filling the array with our menu options, described in our menuOptions ar
	// The second argument of the new_item function is the description
	// of that item, displayed right next to the item. Since we want to 
	// keep the interface minimal we decided against having a description.
	// We supply a NULL pointer to have an empty description.
	// The set_item_userptr() function is responsible for the function that
	// runs when the specific item is selected
	
	for(int l = 0; l < noOfoptions; l++)
		myItems[l] = new_item( menuOptions[l], NULL);
	
	//  We create a menu, with items all of our initial options, 
	//  in the desired form
	mainMenu = new_menu((ITEM **)myItems);
	// Create the window for our menu
	// it creates a window of menuHEIGHT by menuWIDTH dimensions and places
	// it at the latter 2 args
  	mainMenuWin = newwin(menuHEIGHT, menuWIDTH, (LINES-menuHEIGHT)/2, (COLS-menuWIDTH)/2);
	// enable the usage of keyboard for our window
	keypad(mainMenuWin, TRUE);
	// Set main window and subwindow for our menu 
	// (simply creating it would not be enough, the two of them must be
	// linked together)
	set_menu_win(mainMenu, mainMenuWin);
	// The derwin() function is the same as subwin(), creating a virtual
	// subwindow in the window supplemented, with the only difference being
	// that the coordinates are relative to our window
	// ( not to stdscr, the standard terminal screen )
	set_menu_sub(mainMenu, derwin(mainMenuWin, menuHEIGHT-4, menuWIDTH-4, 3,1));
	// Set which character should point at our current option:
	set_menu_mark(mainMenu, " => ");
	// Print a border for our menu and a title:
	// 0,0 gives default characters for the vertical and horizontal lines
	box( mainMenuWin, 0, 0 );
	// Print the gameTITLE in the middle of the 1st line of our menu window 
	printInMiddle(mainMenuWin, 1, gameTITLE, COLOR_PAIR(1));
	mvwaddch(mainMenuWin, 2, 0, ACS_LTEE);
    mvwhline(mainMenuWin, 2, 1, ACS_HLINE, menuWIDTH-2);
    mvwaddch(mainMenuWin, 2, menuWIDTH-1, ACS_RTEE);
	// We set the menu options: 
	//  We make our menu visible, and initialize our 'choiceIsMade' boolean
	//  to false
	bool choiceIsMade = false;
	printInMiddle(stdscr, -1, "F1 to exit", COLOR_PAIR(1)| A_BLINK);
	refresh();

	post_menu(mainMenu);
	wrefresh(mainMenuWin);
	
	while(!choiceIsMade)
	{
		ch = wgetch(mainMenuWin);
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
				choiceIsMade = true;
		}
		wrefresh(mainMenuWin);
	}
	unpost_menu(mainMenu);
	free_menu(mainMenu);
	strcpy(answer, item_name(playerChoice));
}

void initialization()
{
	// We start the curses mode ( enabling windows functionality )
	initscr();

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
	// There was also the raw() option but this one was selected so that
	// the "standard" keybindings would still function properly
	cbreak();
	// We don't want to see the characters as they're typed in:
	// ( user's input )
	noecho();
	keypad(stdscr, TRUE);
	// Make the cursor invisible
	curs_set(0);
	// Enable colors and set color pairs, see README FOR MORE
	start_color();
	// to make everything more readable
	// Also take into consideration the menu colors
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_BLUE, COLOR_BLACK);
}
