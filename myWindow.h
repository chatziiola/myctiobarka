#define landCHAR '#'
#define crashCHAR 'X' | A_BLINK
#define stdscrCHAR ' ' | A_REVERSE

//  chtype mvinch(int y, int x); 

void initialization();
void printSequence(WINDOW * win,int startY, int startX, int length, chtype character);
void fillSTDSCR();
void printInMiddle(WINDOW *win, int row, char *string, chtype attrs);
bool printMap(WINDOW * win, PLAYER *player);
//void printCompass(WINDOW * win)
//void arrowWind(WINDOW *win,int intensity, int direction){
void showMenu(char * answer, char **menuOptions, WINDOW *mainMenuWin);
void showCredits(WINDOW *menuWin);
void showExit(WINDOW *menuWin);

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
	// Also take into consideration the menu colors
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_BLUE, COLOR_BLACK);
}

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
	init_pair(1, COLOR_GREEN, COLOR_BLACK);
	wattron(win, COLOR_PAIR(1));
	// printIsLand
	// We must leave a space of at least 2 between two pieces of land
	printSequence(win,6,25,5,landCHAR);
	printSequence(win,7,15,25,landCHAR);
	printSequence(win,8,15,45,landCHAR);
	printSequence(win,9,20,39,landCHAR);
	// Barrier so that the boats can't go the other way
	printSequence(win,9,59,10,landCHAR | A_INVIS);
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
	wattroff(win, COLOR_PAIR(1));
	printSequence(win,rows-1,1,-1,ACS_HLINE);
}

bool printMap(WINDOW * win, PLAYER *player)
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
			playerReset(&player[l],l);
		}
		else
			printPlayer(win,player[l]);

 	return true;
}

void arrowWind(WINDOW *win,int intensity, int direction)
{
	// ACS_DARROW (L,U,R)
	// This function simply prints the arrow we want to describe the wind
	// There are 4 types of arrows based on direction (UP,DOWN,LEFT,RIGHT)
	// and 4 categories based on intensity (lvl1,2,3,4) each of which is 
	// "expressed" by a different color
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_BLUE, COLOR_BLACK);
	init_pair(4, COLOR_RED, COLOR_BLACK);
	intensity++;
	direction++;
	mvwaddch(win,0,0,ACS_VLINE);
	wattron(win, COLOR_PAIR(intensity));
	// Going clockwise
	wprintw(win," WIND: %d  ", intensity);
	switch(direction)
	{
		case 1:
			waddch(win,ACS_UARROW);
			break;
		case 2:
			waddch(win,ACS_RARROW);
			break;
		case 3:
			waddch(win,ACS_DARROW);
			break;
		case 4:
			waddch(win,ACS_LARROW);
			break;
	}
	wattroff(win, COLOR_PAIR(intensity));
}

void showMenu(char * answer, char *menuOptions[], WINDOW *mainMenuWin)
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
	
	keypad(mainMenuWin, true);
	//  We create a menu, with items all of our initial options, 
	//  in the desired form
	mainMenu = new_menu((ITEM **)myItems);
	// Set main window and subwindow for our menu 
	// (simply creating it would not be enough, the two of them must be
	// linked together)
	set_menu_win(mainMenu, mainMenuWin);
	// The derwin() function is the same as subwin(), creating a virtual
	// subwindow in the window supplemented, with the only difference being
	// that the coordinates are relative to our window
	// ( not to stdscr, the standard terminal screen )
	set_menu_sub(mainMenu, derwin(mainMenuWin, menuOPTIONS, 20, (menuHEIGHT-menuOPTIONS-2)/2,(menuWIDTH-20)/2));
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
	//	printInMiddle(stdscr, -1, "F1 to exit", COLOR_PAIR(1)| A_BLINK);
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
	wclear(mainMenuWin);
	wrefresh(mainMenuWin);

	unpost_menu(mainMenu);
	for(int l=0; l < menuOPTIONS; l++)
		free_item(myItems[l]);
	free_menu(mainMenu);
	strcpy(answer, item_name(playerChoice));
}

void showCredits(WINDOW *menuWin)
{
		box(menuWin,0,0);
		printInMiddle(menuWin,2, gameTITLE, A_BOLD);
		printInMiddle(menuWin,5, "Created by:", A_NORMAL);
		printInMiddle(menuWin,6, "Lamprinos Chatziioannou", A_NORMAL);
		printInMiddle(menuWin,7, "Georgios Panagiotidis", A_NORMAL);
		printInMiddle(menuWin,-2, "Press any key to exit.", A_NORMAL);
		wrefresh(menuWin);
		getch();
}

void showExit(WINDOW *menuWin)
{
		box(menuWin,0,0);
		printInMiddle(menuWin,2, gameTITLE, A_BOLD);
		printInMiddle(menuWin,6, "Thanks for playing our game", A_BOLD);
		printInMiddle(menuWin,8, "Created by:", A_NORMAL);
		printInMiddle(menuWin,9, "Lamprinos Chatziioannou", A_NORMAL);
		printInMiddle(menuWin,10, "Georgios Panagiotidis", A_NORMAL);
		printInMiddle(menuWin,-2, "Press any key to exit.", A_NORMAL);
		wrefresh(menuWin);
		getch();
}

void showControls(WINDOW *menuWin)
{
		box(menuWin,0,0);
		printInMiddle(menuWin,2, gameTITLE, A_BOLD);
		printInMiddle(menuWin,6, "Player 1:", A_BOLD);
		printInMiddle(menuWin,7, "Moves with the arrow keys, 3 keystrokes per turn", A_NORMAL);
		printInMiddle(menuWin,10, "Player 2:", A_BOLD);
		printInMiddle(menuWin,11, "Moves with WASD, also 3 keystrokes per turn", A_NORMAL);
		printInMiddle(menuWin,-2, "Press any key to exit.", A_NORMAL);
		wrefresh(menuWin);
		getch();
}

