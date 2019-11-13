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
		init_pair(1,COLOR_GREEN,COLOR_BLACK);
		init_pair(2,COLOR_RED,COLOR_BLACK);
		init_pair(3,COLOR_CYAN,COLOR_BLACK);
		init_pair(4,COLOR_YELLOW,COLOR_BLACK);
		init_pair(5,COLOR_MAGENTA,COLOR_BLACK);

		wbkgd(mywin, COLOR_PAIR(1));
		attron(A_BOLD);
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

	char* colors[] = {
		",---------.    .-''-.    .---.       .-''-.  ,---.    ,---.    .-''-. ,---------. .-------.   rrrrrr    ____     ",
		"\\          \\ .'c c   \\   | rr|     .'r r   \\ |    \\  /    |  .'c c   \\\\          \\|  c c   \\  r yyyrrr .'  __ `.  ",
		" `--.  ,---'/ c c c   'rrrr  r    / r r r   '|  ,  \\/  ,  | / c c c   '`--.  ,---'| c c c  |  r yyy r/   '  \\  \\ ",
		"    |   \\  . cc y cc  |r  ry rrr . rr y rr  ||  |\\m   /|  |. cc y cc  |   |   \\   |cc y cc /   rrrrrr|___|  /  | ",
		"    :m m:  |  ccccc___| r yyy  r |  rrrrr___||  mm mm/ |  ||  ccccc___|   :r r:   | ccccc.' __ .---.    _.-`   | ",
		"    mmmmm  '  \\   .---.r  r  rrr '  \\   .---.| mm y mm |  |'  \\   .---.   rrrrr   |  |\\ \\  |  ||   | .'   m    | ",
		"   mmyyymm  \\  `-'    / rrrrrr|___\\  `-'    /|  mmmmm  |  | \\  `-'    /  rryyyrr  |  | \\ `'   /|   | |  mm mm  | ",
		"    mmmmm    \\       /   |        \\\\       / |  |      |  |  \\       /    rrrrr   |  |  \\    / |   | \\ mm y mm / ",
		"    '---'     `'-..-'    `--------` `'-..-'  '--'      '--'   `'-..-'     '---'   ''-'   `'-'  '---'  '.mmmmm.'  "

	};

	for (int j = 0; j < sizex+teleLenght; j++) {
		for (int i = 0; i < 9; ++i) {
			int sub = 0;
			for (int k = 0; k < j; ++k) {
				if (k-sub>= 0 && k-sub<teleLenght) {
					switch (colors[i][k]) {
						case 'r':
							attron(COLOR_PAIR(2));
						break;
						case 'c':
							attron(COLOR_PAIR(3));
						break;
						case 'y':
							attron(COLOR_PAIR(4));
						break;
						case 'm':
							attron(COLOR_PAIR(5));
						break;
					}
					mvwprintw(mywin,starting_y+i,sizex-j+k-sub,"%c",telemetria[i][k]);			
					attron(COLOR_PAIR(1));
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