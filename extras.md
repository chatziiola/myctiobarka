/* 
 * printInMiddle:
 * @params:
 * takes as arguments a window in which the string will be printed
 *  an integer row, the line in which we want the screen to be printed
 * (if row<0, then the string will be printed |row| lines from the bottom 
 * of the window), the string we want to print, and attributes attrs which in 
 * most cases will be the color
 */


// gameWIDTH, gameHEIGHT
//		@usage: these two values are the dimensions of our game window
//				in which the game will take place

// menuWIDTH, menuHEIGHT
//		@usage: these two values are the dimensions of our menu window, 
//				which the users will be able to access at launch and when
//				ESC is pressed
// landCHAR, crashCHAR, stdscrCHAR
/
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
	/ 		@usage: obvious
