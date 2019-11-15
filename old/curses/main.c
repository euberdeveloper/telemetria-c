#include <ncurses.h>
#include <unistd.h>

int main(int argc, char const *argv[]) {
	WINDOW* mywin = initscr();
	cbreak();
	noecho();
	curs_set(0);

	int sizex,sizey;
	getmaxyx(mywin,sizey,sizex);
	int starting_y = 0;
	if (sizey > 10) {
		starting_y = (sizey - 10)/2;
	}

	if (has_colors()) {
		start_color();
		init_pair(1,COLOR_WHITE,COLOR_BLACK);
		init_pair(2,COLOR_RED,COLOR_BLACK);
		init_pair(3,COLOR_CYAN,COLOR_BLACK);
		init_pair(4,COLOR_YELLOW,COLOR_BLACK);
		init_pair(5,COLOR_MAGENTA,COLOR_BLACK);

		wbkgd(mywin, COLOR_PAIR(1));
	}

	int teleLenght = 114;
	char* telemetria[] = {
		",---------.    .-''-.    .---.       .-''-.  ,---.    ,---.    .-''-. ,---------. .-------.   .-./`)    ____     ",
		"\\          \\ .'_ _   \\   | ,_|     .'_ _   \\ |    \\  /    |  .'_ _   \\\\          \\|  _ _   \\  \\ .-.') .'  __ `.  ",
		" `--.  ,---'/ ( ` )   ',-./  )    / ( ` )   '|  ,  \\/  ,  | / ( ` )   '`--.  ,---'| ( ' )  |  / `-' \\/   '  \\  \\ ",
		"    |   \\  . (_ o _)  |\\  '_ '`) . (_ o _)  ||  |\\_   /|  |. (_ o _)  |   |   \\   |(_ o _) /   `-'`\"`|___|  /  | ",
		"    :_ _:  |  (_,_)___| > (_)  ) |  (_,_)___||  _( )_/ |  ||  (_,_)___|   :_ _:   | (_,_).' __ .---.    _.-`   | ",
		"    (_I_)  '  \\   .---.(  .  .-' '  \\   .---.| (_ o _) |  |'  \\   .---.   (_I_)   |  |\\ \\  |  ||   | .'   _    | ",
		"   (_(=)_)  \\  `-'    / `-'`-'|___\\  `-'    /|  (_,_)  |  | \\  `-'    /  (_(=)_)  |  | \\ `'   /|   | |  _( )_  | ",
		"    (_I_)    \\       /   |        \\\\       / |  |      |  |  \\       /    (_I_)   |  |  \\    / |   | \\ (_ o _) / ",
		"    '---'     `'-..-'    `--------` `'-..-'  '--'      '--'   `'-..-'     '---'   ''-'   `'-'  '---'  '.(_,_).'  "
	};

	refresh();
	char selected;
	for (int j = 0; j < sizex+teleLenght; j++) {
		for (int i = 0; i < 9; ++i) {
			int sub = 0;
			for (int k = 0; k < j; ++k) {
				if (k-sub>= 0 && k-sub<teleLenght) {
					selected = telemetria[i][k];					
					mvwprintw(mywin,starting_y+i,sizex-j+k-sub,"%c",selected);			
				}
			}
		}
		refresh();
		usleep(30000);
	}

	char x = getch();
	endwin();
	return 0;
}