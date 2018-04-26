#include <stdbool.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define LEFT   0
#define RIGHT  6
#define TOP    0
#define MIDDLE 3
#define BOTTOM 6
#define VERT   4
#define HORIZ  8

/* * * * * * * * * *
 *    Segments     *
 *  ************   *
 *       4         *
 *    ████████     *
 *  0 ██    ██ 1   *
 *    ██ 5  ██     *
 *    ████████     *
 *    ██    ██     *
 *  2 ██ 6  ██ 3   *
 *    ████████     *
 *                 *
 * * * * * * * * * */

typedef struct __segment
{	int type;
	int side;
	int height;
} Segment;

typedef struct __digit
{	int segments[7];
	int states[7];
} Digit;

int get_cpair(char clr);
int get_keys(int cpair);
void setup_cpairs(void);
void cleanup(void);
void create_segments(Segment (*segments)[]);
void create_digits(Digit (*digits)[]);
void draw_colon(int y, int x, int offset);
void draw_segment(int n, int state,
	              int y, int x, 
	              int offset,
	              int cpair, 
	              Segment segments[]);
void draw_digit(int n, 
	            int y, int x,
	            int offset,
	            int cpair,
                Digit digits[],
				Segment segments[]);