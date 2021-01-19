/*This a library that has most of the functions we are using for our program*/


void arrowWind(WINDOW *win,int intensity, int direction){
	wclear(win);                                   //Clears the window from previously printed characters
	printCompass(win);
	start_color();                             			//Enables the use of colors for our arrows
	attron(COLOR_PAIR(intensity));                     //This function switches on the color function
	switch(direction){                                 //According to the value of a, witch is random from 1 to 8,
		case 1:                            //an arrow is printed that shows the direction of the wind
			mvwprintw(win,1,6,"^");    //along with its intensity
			mvwprintw(win,2,6,"|");
			wrefresh(win);
			break;
		case 2:
			mvwprintw(win,1,8,"/");
			mvwprintw(win,2,7,"/");
			wrefresh(win);
			break;
		case 3:
			mvwprintw(win,3,7,"---->");
			wrefresh(win);
			break;
		case 4:
			mvwprintw(win,4,7,"\\");
			mvwprintw(win,5,8,"\\");
			wrefresh(win);
			break;
		case 5:
			mvwprintw(win,4,6,"|");
			mvwprintw(win,5,6,"v");
			wrefresh(win);
			break;
		case 6:
			mvwprintw(win,4,5,"/");
			mvwprintw(win,5,4,"/");
			wrefresh(win);
			break;
		case 7:
			mvwprintw(win,3,1,"<----");
			wrefresh(win);
			break;
		case 8:
			mvwprintw(win,2,5,"\\");
			mvwprintw(win,1,4,"\\");
			wrefresh(win);
			break;
		default:
			break;
		}
	attroff(COLOR_PAIR(intensity));                    //It switches the color attribute off so that characters printed in the
                                                   //future
}

void printCompass(WINDOW * win)
{
    mvwprintw(win,0,2,"NW");                   //Prints the the main and secondary points of the Horizon
	mvwprintw(win,0,6,"N");
	mvwprintw(win,0,9,"NE");
	mvwprintw(win,3,0,"W");
	mvwprintw(win,3,6,"O");
	mvwprintw(win,3,12,"E");
	mvwprintw(win,6,2,"SW");
	mvwprintw(win,6,6,"S");
	mvwprintw(win,6,9,"SE");
}	
