#include "term_digits.h"

int main(int argc, char const *argv[])
{/* Run the clock. */
	char *datetime;
    time_t current_time;
    int offsets[] = {0, 10, 20, 24, 34, 44, 48, 58};
    int current_digit;
    int y = 3, x = 7;
	int cpair;
    int i, n;

	Segment segments[7];
	Digit digits[10];
	
	initscr();
	noecho();
	timeout(0);
	curs_set(0);
	setup_cpairs();
	create_segments(&segments);
	create_digits(&digits);
	cpair = get_cpair('w');

	while (true)
	{   current_time = time(NULL);
		datetime = ctime(&current_time);
		cpair = get_keys(cpair);

		for (i = 11; i < 19; ++i)
		{   current_digit = datetime[i] - '0';

			if (current_digit == 10)
				draw_colon(y, x, offsets[i-11]);
			else
			{   draw_digit(current_digit, 
				           y, x, offsets[i-11], 
				           cpair, digits, segments);
			}
		}
		refresh();
	}
	cleanup();
	return 0;
}

int get_keys(int cpair)
{/* Get keyboard input.. */
	int key, temp;

	if ((key = getch()) == -1) return cpair;

	if (key == 'q') cleanup();
	else
	{	temp = get_cpair(key);
		return (temp) ? temp : cpair;
	}
}

void setup_cpairs(void)
{/* Setup all needed color pairs. */
	int n = 0;
	int colors[] = {
		COLOR_WHITE,
		COLOR_RED,
		COLOR_GREEN,
		COLOR_BLUE,
		COLOR_CYAN,
		COLOR_YELLOW,
		COLOR_MAGENTA,
	};

	start_color();

	while (n < 8)
	{	init_pair(n+1, colors[n], colors[0]);
		++n;
	}
}

void cleanup(void)
{/* End the program and restore terminal. */
	echo();
	endwin();
	exit(EXIT_SUCCESS);
}

int get_cpair(char clr)
{/* Get a color pair using its one letter abbreviation. */
	char valid_colors[] = {'w', 'r', 'g', 'b', 'c', 'y', 'm'};
	int i;

	for (i = 0; i < 8; ++i)
	{	if (clr == valid_colors[i])
			return COLOR_PAIR(i+1);
	}
	return 0;
}

void create_segments(Segment (*segments)[])
{/* Fill the segments struct. */
	(*segments)[0] = (Segment){VERT,  LEFT,  TOP};
	(*segments)[1] = (Segment){VERT,  RIGHT, TOP};
	(*segments)[2] = (Segment){VERT,  LEFT,  MIDDLE};
	(*segments)[3] = (Segment){VERT,  RIGHT, MIDDLE};
	(*segments)[4] = (Segment){HORIZ, 0,     TOP};
	(*segments)[5] = (Segment){HORIZ, 0,     MIDDLE};
	(*segments)[6] = (Segment){HORIZ, 0,     BOTTOM};
}

void draw_segment(int n, int state,
	              int y, int x, 
	              int offset, int cpair, 
	              Segment segments[])
{/* Draw a single segment. */
	int i, c;
	int block = ' ';
	int shade = ACS_CKBOARD;

	if (state)
	{	attron(A_REVERSE);
		attron(A_BOLD);
		attron(cpair);
		c = block;
	}
	else 
	{	attron(A_DIM);
		c = shade;
	}
	if (segments[n].type == VERT)
	{	x += offset + segments[n].side;
		y += segments[n].height;

		for (i = 0; i < VERT; ++i)
		{	mvaddch(y+i, x, c);
			mvaddch(y+i, x+1, c);
		}
	}
	else
	{	x += offset;
		y += segments[n].height;

		for (i = 0; i < HORIZ; ++i)
			mvaddch(y, x+i, c);
	}
	if (state) 
	{
		attroff(A_REVERSE);
		attroff(A_BOLD);
		attroff(cpair);
	}
	else attroff(A_DIM);
}

void create_digits(Digit (*digits)[])
{/* Fill the digits struct. */
	(*digits)[0] = (Digit){{5, 0, 1, 2, 3, 4, 6}, {0, 1, 1, 1, 1, 1, 1}};
	(*digits)[1] = (Digit){{0, 2, 4, 5, 6, 1, 3}, {0, 0, 0, 0, 0, 1, 1}};
	(*digits)[2] = (Digit){{0, 3, 1, 2, 4, 5, 6}, {0, 0, 1, 1, 1, 1, 1}};
	(*digits)[3] = (Digit){{5, 0, 2, 1, 3, 4, 6}, {1, 0, 0, 1, 1, 1, 1}};
	(*digits)[4] = (Digit){{2, 4, 6, 0, 1, 3, 5}, {0, 0, 0, 1, 1, 1, 1}};
	(*digits)[5] = (Digit){{1, 2, 0, 3, 4, 5, 6}, {0, 0, 1, 1, 1, 1, 1}};
	(*digits)[6] = (Digit){{1, 0, 2, 3, 4, 5, 6}, {0, 1, 1, 1, 1, 1, 1}};
	(*digits)[7] = (Digit){{0, 2, 5, 6, 1, 3, 4}, {0, 0, 0, 0, 1, 1, 1}};
	(*digits)[8] = (Digit){{0, 1, 2, 3, 4, 5, 6}, {1, 1, 1, 1, 1, 1, 1}};
	(*digits)[9] = (Digit){{2, 0, 1, 3, 4, 5, 6}, {0, 1, 1, 1, 1, 1, 1}};
}

void draw_digit(int n, 
	            int y, int x,
	            int offset, 
	            int cpair,
                Digit digits[],
				Segment segments[])
{/* Draw a digit, segment by segment. */
	int i;
	int seg, state; 

	for (i = 0; i < 7; ++i)
	{   seg = digits[n].segments[i];
		state = digits[n].states[i];
		draw_segment(seg, state, y, x, offset, cpair, segments);
	}
}
                
void draw_colon(int y, int x, int offset)
{/* Draw a colon (':'). */
	int i;

	y += 1;
	x += offset;

	attron(A_BOLD);
	attron(A_REVERSE);

	for (i = 0; i < 2; ++i)
		mvaddstr(y + 4*i, x, "  ");

	attroff(A_BOLD);
	attroff(A_REVERSE);
}