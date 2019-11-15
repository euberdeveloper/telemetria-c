#include <ncurses.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
//J yellow (1)
//L magenta (2)
//S white (3)
//Z cyan (4)
//T green (5)
//O blue (6)
//I red (7)

#define debug 1

typedef enum {
	NORMAL = 0,
	YELLOW = 1,
	MAGENTA = 2,
	WHITE = 3,
	CYAN = 4,
	GREEN = 5,
	BLUE = 6,
	RED = 7,
} color_couple;

int refresh_screen(int** matrix, int board_size_x, int board_size_y,int display_size_x, int display_size_y, int screen_size_x, int screen_size_y);
int evaluate(int** matrix,int board_size_x, int board_size_y);
int removelines(int** matrix,int board_size_x, int board_size_y);

int main(int argc, char const *argv[]) {
 	attron(A_ALTCHARSET);

	WINDOW* mywin = initscr();
	cbreak();
	noecho();
	curs_set(0);

	int board_size_x = 10;
	int board_size_y = 20;

	int display_size_x = board_size_x * 3 + 6;
	int display_size_y = board_size_y * 2 + 4;

	int screen_size_x,screen_size_y;
	getmaxyx(mywin,screen_size_y,screen_size_x);

	int** matrix = (int**) malloc(sizeof(int*) * board_size_y);
	for (int j = 0; j < board_size_y; ++j){
		matrix[j] = (int*) malloc(sizeof(int) * board_size_x);
		for (int i = 0; i < board_size_x; ++i) {
			matrix[j][i] = 0;
		}
	} 

	
	matrix[5][5] = 11;
	matrix[5][6] = 100;
	matrix[6][5] = 11;
	matrix[6][6] = 100;
	matrix[7][5] = 11;
	matrix[7][6] = 11;

	matrix[19][0] = 2;
	matrix[19][1] = 5;
	matrix[19][2] = 4;
	matrix[19][3] = 4;
	matrix[19][4] = 7;
	matrix[19][7] = 3;
	matrix[19][8] = 3;
	matrix[19][9] = 3;

	

	if (has_colors()) {
		start_color();
		init_pair(NORMAL,COLOR_WHITE,COLOR_BLACK);
		init_pair(YELLOW,COLOR_YELLOW,COLOR_BLACK);
		init_pair(MAGENTA,COLOR_MAGENTA,COLOR_BLACK);
		init_pair(WHITE,COLOR_WHITE,COLOR_BLACK);
		init_pair(CYAN,COLOR_CYAN,COLOR_BLACK);
		init_pair(GREEN,COLOR_GREEN,COLOR_BLACK);
		init_pair(BLUE,COLOR_BLUE,COLOR_BLACK);
		init_pair(RED,COLOR_RED,COLOR_BLACK);

		wbkgd(mywin, COLOR_PAIR(NORMAL));
		attron(A_BOLD);
	}

	int print_x, print_y;
	if (screen_size_x < display_size_x || screen_size_y -8 < display_size_y) {
		mvwprintw(mywin,1,1,"The screen is too small for playing a decent tetris!");
		char x = getch();
		endwin();
		return 0;
	}			

	char intro[] = "Welcolme to Tetris!";
	int len = strlen(intro);
	for (int i = 0; i < len; ++i) {
		mvwprintw(mywin, 3,((screen_size_x - len)/2)+i,"%c",intro[i]);
		refresh();
		if (!debug) usleep(100000);
	}

	if (!debug) usleep(100000);
	attron(COLOR_PAIR(CYAN));
	mvwprintw(mywin, 3,((screen_size_x - len)/2),"%s",intro);
	refresh();

	if (!debug) usleep(300000);
	attron(COLOR_PAIR(WHITE));
	mvwprintw(mywin, 3,((screen_size_x - len)/2),"%s",intro);
	refresh();

	if (!debug) usleep(300000);
	attron(COLOR_PAIR(CYAN));
	mvwprintw(mywin, 3,((screen_size_x - len)/2),"%s",intro);
	refresh();

	if (!debug) usleep(300000);
	attron(COLOR_PAIR(WHITE));
	mvwprintw(mywin, 3,((screen_size_x - len)/2),"%s",intro);
	refresh();

	if (!debug) usleep(100000);
	char ivan[] = "Developed by Ivan Martini";
	len = strlen(ivan);
	for (int i = 0; i < len; ++i) {
		mvwprintw(mywin, 5,((screen_size_x - len)/2)+i,"%c",ivan[i]);
		refresh();
		if (!debug) usleep(100000);
	}

	if (!debug) usleep(100000);
	char copyright[] = "No rights reserved. Don't even know if its legal.";
	len = strlen(copyright);
	for (int i = 0; i < len; ++i) {
		mvwprintw(mywin, 6,((screen_size_x - len)/2)+i,"%c",copyright[i]);
		refresh();
		if (!debug) usleep(100000);
	}

	//PRINTING THE BOARD
	attron(COLOR_PAIR(WHITE));
	mvaddch(8,((screen_size_x - display_size_x)/2),ACS_ULCORNER);
	mvaddch(8,((screen_size_x - display_size_x)/2)+1,ACS_HLINE);
	mvaddch(8,((screen_size_x - display_size_x)/2)+2,ACS_TTEE);
	for (int i = 0; i < display_size_x-6; ++i) {
		mvaddch(8,((screen_size_x - display_size_x)/2)+3+i,ACS_HLINE);
	}
	mvaddch(8,((screen_size_x - display_size_x)/2)+display_size_x-3,ACS_TTEE);
	mvaddch(8,((screen_size_x - display_size_x)/2)+display_size_x-3+1,ACS_HLINE);
	mvaddch(8,((screen_size_x - display_size_x)/2)+display_size_x-3+2,ACS_URCORNER);

	for (int i = 0; i < display_size_y-4; ++i) {
		if (!debug) usleep(100000);
		refresh();
		mvaddch(9+i,((screen_size_x - display_size_x)/2),ACS_VLINE);
		mvaddch(9+i,((screen_size_x - display_size_x)/2)+1,' ');
		mvaddch(9+i,((screen_size_x - display_size_x)/2)+2,ACS_VLINE);
		//mvprintw(9+i,((screen_size_x - display_size_x)/2)-3, "%d", i);
		mvaddch(9+i,((screen_size_x - display_size_x)/2)+display_size_x-3,ACS_VLINE);
		mvaddch(9+i,((screen_size_x - display_size_x)/2)+display_size_x-3+1,' ');
		mvaddch(9+i,((screen_size_x - display_size_x)/2)+display_size_x-3+2,ACS_VLINE);
	}

	if (!debug) usleep(100000);
	refresh();
	mvaddch(display_size_y+8-3,((screen_size_x - display_size_x)/2),ACS_VLINE);
	mvaddch(display_size_y+8-3,((screen_size_x - display_size_x)/2)+1,' ');
	mvaddch(display_size_y+8-3,((screen_size_x - display_size_x)/2)+2,ACS_LLCORNER);
	for (int j = 0; j < display_size_x-6; ++j) {
		mvaddch(display_size_y+8-3,((screen_size_x - display_size_x)/2)+3+j,ACS_HLINE);
	}
	mvaddch(display_size_y+8-3,((screen_size_x - display_size_x)/2)+display_size_x-3,ACS_LRCORNER);
	mvaddch(display_size_y+8-3,((screen_size_x - display_size_x)/2)+display_size_x-3+1,' ');
	mvaddch(display_size_y+8-3,((screen_size_x - display_size_x)/2)+display_size_x-3+2,ACS_VLINE);

	if (!debug) usleep(100000);
	refresh();
	mvaddch(display_size_y+8-2,((screen_size_x - display_size_x)/2),ACS_VLINE);	
	mvaddch(display_size_y+8-2,((screen_size_x - display_size_x)/2)+display_size_x-1,ACS_VLINE);

	if (!debug) usleep(100000);
	refresh();
	mvaddch(display_size_y+8-1,((screen_size_x - display_size_x)/2),ACS_LLCORNER);
	for (int j = 0; j < display_size_x-2; ++j) {
		mvaddch(display_size_y+8-1,((screen_size_x - display_size_x)/2)+1+j,ACS_HLINE);
	}
	mvaddch(display_size_y+8-1,((screen_size_x - display_size_x)/2)+display_size_x-1,ACS_LRCORNER);
	refresh();
	//GAME_LOGIC
	int count = 0;
	int gen = -1;
	while(1){
		timeout(0);
		char mv = getch();
		if (!(mv == ERR))
			if (debug) mvaddch(2,1,mv);
		
		switch(mv) {
			case 'w': case 'W':
			break;

			case 's': case 'S':
			break;

			case 'a': case 'A':
			//left
			
			break;

			case 'd': case 'D':
			//right

			break;
		}
		refresh();

		if (gen == count) {
		}

		if (evaluate(matrix,board_size_x,board_size_y) == 1) {
			gen = count + 1;
		}

		refresh_screen(matrix,board_size_x,board_size_y,display_size_x, display_size_y, screen_size_x,screen_size_y);
		refresh();
		usleep(100*1000);

		if (removelines(matrix,board_size_x,board_size_y)) {
			refresh_screen(matrix,board_size_x,board_size_y,display_size_x, display_size_y, screen_size_x,screen_size_y);
			refresh();
		}

		mvprintw(1,1,"%d",count++);
		usleep(200*1000);
		flushinp();
		usleep(200*1000);
	}

	endwin();
	return 0;
}

int refresh_screen(int** matrix,int board_size_x, int board_size_y,int display_size_x, int display_size_y, int screen_size_x, int screen_size_y) {
	int top_x = ((screen_size_x - display_size_x)/2)+3;		
	int top_y = 9;

	int plot_x = 0;
	int plot_y = 0;

	//mvaddch(top_y,top_x, '3');
	for (int i = 0; i < board_size_y; ++i) {
		for (int j = 0; j < board_size_x; ++j) {
			char ch = ' ';
			int ev = matrix[i][j] > 10 ? matrix[i][j]-10 : matrix[i][j];

			switch (ev){
				case 1:
				//J yellow (1)
				attron(COLOR_PAIR(YELLOW));
				ch = '#';
				break;

				case 2:
				//L magenta (2)
				attron(COLOR_PAIR(MAGENTA));
				ch = '#';
				break;

				case 3:
				//S white (3)
				attron(COLOR_PAIR(WHITE));
				ch = '#';
				break;

				case 4:
				//Z cyan (4)
				attron(COLOR_PAIR(CYAN));
				ch = '#';
				break;

				case 5:
				//T green (5)
				attron(COLOR_PAIR(GREEN));
				ch = '#';
				break;

				case 6:
				//O blue (6)
				attron(COLOR_PAIR(BLUE));
				ch = '#';
				break;

				case 7:
				//I red (7)
				attron(COLOR_PAIR(RED));
				ch = '#';
				break;

				case 90:
				//I red (7)
				attron(COLOR_PAIR(RED));
				ch = 'a';
				break;
			};

			mvaddch(top_y+plot_y,top_x+plot_x,ch);
			mvaddch(top_y+plot_y,top_x+plot_x+1,ch);
			mvaddch(top_y+plot_y,top_x+plot_x+2,ch);
			
			mvaddch(top_y+plot_y+1,top_x+plot_x,ch);
			mvaddch(top_y+plot_y+1,top_x+plot_x+1,ch);
			mvaddch(top_y+plot_y+1,top_x+plot_x+2,ch);
			plot_x += 3;

			attron(COLOR_PAIR(NORMAL));
		}
		plot_x = 0;
		plot_y +=2; 
	}
	return 0;
}

int evaluate(int** matrix,int board_size_x, int board_size_y) {
	//make falling pieces fall
	int still_falling = 1;
	int min_pos[board_size_x];
	for (int i = 0; i < board_size_x; ++i){
		min_pos[i] = -1;
	}

	for (int i = board_size_y-1; i > -1; --i)	{
		for (int j = board_size_x-1; j > -1; --j) {
			if (matrix[i][j] > 10 && i<board_size_y-1) {
				if (min_pos[j] < i && matrix[i][j] != 100) {min_pos[j] = i+1;}
				if (matrix[i+1][j] == 0) matrix[i+1][j] = matrix[i][j];
				matrix[i][j] = 0;
			}
		}
	}

	for (int i = 0; i < board_size_x; ++i) {
		if (debug) mvprintw(3+i,3,"%d",min_pos[i]);
	}

	int x;
	for (int i = 0; i < board_size_x; ++i){
		if (min_pos[i] == board_size_y - 1)
			still_falling = 0;
		else 
			if (matrix[min_pos[i]+1][i] != 0 && matrix[min_pos[i]+1][i] != 100 )
				still_falling = 0;
	}

	if (still_falling == 0) {
		for (int i = board_size_y-1; i > -1; --i)	{
			for (int j = board_size_x-1; j > -1; --j) {
				if (matrix[i][j] == 100) {
					matrix[i][j] = 0;
				} else if (matrix[i][j] > 10) {
					matrix[i][j] -=10;
				}
			}
		}
		return 1;
	}
	return 0;
}

int removelines(int** matrix,int board_size_x, int board_size_y) {
	for (int i = board_size_y-1; i > -1; --i) {
		int delete = 1;
		for (int j = 0; j < board_size_x; ++j) {
			if (matrix[i][j] == 0) {
				delete = 0;
			}
		}

		if (delete == 1) {
			for (int j = i; j > 0; --j) {
				for (int k = 0; k < board_size_x; ++k) {
					matrix[j][k] = matrix[j-1][k]; 
				}
			}
		}
	}
}