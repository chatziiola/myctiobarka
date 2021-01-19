/*This a library that has most of the functions we are using for our program*/


void arrowWind(WINDOW *win,int a, int b){
	clear();                                   //Clears the window from previously printed characters
	mvwprintw(win,0,2,"NW");                   //Prints the the main and secondary points of the Horizon
	mvwprintw(win,0,6,"N");
	mvwprintw(win,0,9,"NE");
	mvwprintw(win,3,0,"W");
	mvwprintw(win,3,6,"O");
	mvwprintw(win,3,12,"E");
	mvwprintw(win,6,2,"SW");
	mvwprintw(win,6,6,"S");
	mvwprintw(win,6,9,"SE");
	start_color();                             //Enables the use of colors for our arrows
	int i=0;                                   //The color of the arrow is determined by the intensity of the wind
	if (b<2) i=2;		                   //The level 1 intensity is designated by a green arrow
	else if ((b>2)&&(b<4)) i=6;                //The level 2 intensity is designated by a cyan arrow
	else if ((b>4)&&(b<6)) i=4;                //The level 3 intensity is designated by a blue arrow
	else if ((b>6)&&(b<8)) i=5;                //The level 4 intensity is designated by a magenta arrow
	else if (b>8) i=1;                         //The level 5 intensity is designated by a red arrow
	attron(COLOR_PAIR(i));                     //This function switches on the color function
	switch(a){                                 //According to the value of a, witch is random from 1 to 8,
		case 1:                            //an arrow is printed that shows the direction of the wind
			mvwprintw(win,1,6,"^");    //along with its intensity
			mvwprintw(win,2,6,"|");
			wrefresh();
			break;
		case 2:
			mvwprintw(win,1,8,"/");
			mvwprintw(win,2,7,"/");
			wrefresh();
			break;
		case 3:
			mvwprintw(win,3,7,"---->");
			wrefresh();
			break;
		case 4:
			mvwprintw(win,4,7,"\\");
			mvwprintw(win,5,8,"\\");
			wrefresh();
			break;
		case 5:
			mvwprintw(win,4,6,"|");
			mvwprintw(win,5,6,"v");
			wrefresh();
			break;
		case 6:
			mvwprintw(win,4,5,"/");
			mvwprintw(win,5,4,"/");
			wrefresh();
			break;
		case 7:
			mvwprintw(win,3,1,"<----");
			wrefresh();
			break;
		case 8:
			mvwprintw(win,2,5,"\\");
			mvwprintw(win,1,4,"\\");
			wrefresh();
			break;
		default:
			break;
		}
	attroff(COLOR_PAIR(i));                    //It switches the color attribute off so that characters printed in the
                                                   //future
