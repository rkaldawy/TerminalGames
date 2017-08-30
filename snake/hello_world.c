#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

#define SCREEN_WIDTH 70
#define SCREEN_HEIGHT 30
#define Y_START 2
#define X_START 10

int len = 4;

typedef struct DOT {
	int x_pos, y_pos;
}DOT;

void drawBoard();
void drawDot(DOT* dot, int flag);
void generateFood(DOT* dot, DOT* food);
void addToEnd(DOT* dot, int last);
void delay(int milliseconds, int* last);

int main()
{

	DOT segs [100];
	DOT* head = segs;
	DOT food;

	head->x_pos = 40;
	head->y_pos = 10;
	segs[1].x_pos = 40;
	segs[1].y_pos = 11;
	segs[2].x_pos = 40;
	segs[2].y_pos = 12;
	segs[3].x_pos = 40;
	segs[3].y_pos = 13;
	segs[4].x_pos = 40;
	segs[4].y_pos = 14;

	initscr();			/* Start curses mode 		  */
	cbreak();
	keypad(stdscr, TRUE);
	noecho();
	nodelay(stdscr, TRUE);

	drawBoard();
	drawDot(segs, 1);
	generateFood(head, &food);


	refresh();			/* Print it on to the real screen */

	int ch, last;
	last = KEY_UP;

	while(1){
		ch = getch();
		if (ch == ERR){ ch = last;}

		switch(ch)
		{
			case KEY_LEFT :
				if(head->x_pos > X_START + 1 && last != KEY_RIGHT)
				{
				drawDot(head, 0);
				for(int i = len; i >= 1; i--) { segs[i] = segs[i-1]; }
				--head->x_pos;
				drawDot(head, 1);
				last = ch;
				}
				break;
			case KEY_RIGHT :
				if(head->x_pos < X_START + SCREEN_WIDTH && last != KEY_LEFT)
				{
				drawDot(head, 0);
				for(int i = len; i >= 1; i--) { segs[i] = segs[i-1]; }
				++head->x_pos;
				drawDot(head, 1);
				last = ch;
				}
				break;
			case KEY_UP :
				if(head->y_pos > Y_START + 1 && last != KEY_DOWN)
				{
				drawDot(head, 0);
				for(int i = len; i >= 1; i--) { segs[i] = segs[i-1]; }
				--head->y_pos;
				drawDot(head, 1);
				last = ch;
				}
				break;
		  case KEY_DOWN :
				if(head->y_pos < Y_START + SCREEN_HEIGHT && last != KEY_UP)
				{
				drawDot(head, 0);
				for(int i = len; i >= 1; i--) { segs[i] = segs[i-1]; }
				++head->y_pos;
				drawDot(head, 1);
				last = ch;
				}
				break;
			}

			if (food.x_pos == head->x_pos && food.y_pos == head->y_pos){
				generateFood(head, &food);
				addToEnd(head, last);
				drawDot(head, 1);
			}
			delay(250, &last);

	}
	getch();			/* Wait for user input */
	endwin();			/* End curses mode		  */

	return 0;
}

void addToEnd(DOT* dot, int last){
	if (len == 1){
		switch(last)
		{
				case(KEY_LEFT):
					dot[len+1].x_pos = dot[len].x_pos + 1;
					dot[len+1].y_pos = dot[len].y_pos;
				case(KEY_RIGHT):
					dot[len+1].x_pos = dot[len].x_pos - 1;
					dot[len+1].y_pos = dot[len].y_pos;
				case(KEY_UP):
					dot[len+1].x_pos = dot[len].x_pos;
					dot[len+1].y_pos = dot[len].y_pos + 1;
				case(KEY_DOWN):
					dot[len+1].x_pos = dot[len].x_pos ;
					dot[len+1].y_pos = dot[len].y_pos - 1;
		}
	} else {
		if(dot[len].x_pos == dot[len-1].x_pos + 1) {
			dot[len+1].x_pos = dot[len].x_pos + 1;
			dot[len+1].y_pos = dot[len].y_pos;
		} else if (dot[len].x_pos == dot[len-1].x_pos - 1){
			dot[len+1].x_pos = dot[len].x_pos - 1;
			dot[len+1].y_pos = dot[len].y_pos;
		} else if (dot[len].y_pos == dot[len-1].y_pos + 1) {
			dot[len+1].x_pos = dot[len].x_pos;
			dot[len+1].y_pos = dot[len].y_pos + 1;
		} else {
			dot[len+1].x_pos = dot[len].x_pos;
			dot[len+1].y_pos = dot[len].y_pos - 1;
		}
	}
	len++;
}

void generateFood(DOT* dot, DOT* food){

	int x_pos, y_pos, check;
	do {
		check = 0;
		x_pos = rand() % SCREEN_WIDTH + X_START;
		y_pos = rand() % SCREEN_HEIGHT + Y_START;
		//x_pos = 20;
		//y_pos = 20;
		for (int i = 0; i <= len; i++){
			if (x_pos == dot[i].x_pos && y_pos == dot[i].y_pos){check = 1; break;}
		}
	} while (check);
	mvaddch(y_pos, x_pos, '@');
	food->x_pos = x_pos;
	food->y_pos = y_pos;
	refresh;
}

void drawDot(DOT* dot, int flag){
	if (flag){
		for(int i = 0; i <= len; i++) {
			mvaddch(dot[i].y_pos, dot[i].x_pos, 'O');
		}
	} else {
		for(int i = 0; i <= len; i++) {
			mvaddch(dot[i].y_pos, dot[i].x_pos, ' ');
		}
	}
	refresh;
}

void drawBoard(){
	mvaddch(Y_START, X_START, '+');
	mvaddch(Y_START+SCREEN_HEIGHT+1, X_START, '+');
	mvaddch(Y_START, X_START+SCREEN_WIDTH+1, '+');
	mvaddch(Y_START+SCREEN_HEIGHT+1, X_START+SCREEN_WIDTH+1, '+');
	mvhline(Y_START, X_START+1, '-', SCREEN_WIDTH);
	mvhline(Y_START+SCREEN_HEIGHT+1, X_START+1, '-', SCREEN_WIDTH);
	mvvline(Y_START+1, X_START, '|', SCREEN_HEIGHT);
	mvvline(Y_START+1, X_START+SCREEN_WIDTH+1, '|', SCREEN_HEIGHT);
}

void delay(int milliseconds, int* last)
{
    long pause;
    clock_t now,then;

    pause = milliseconds*(CLOCKS_PER_SEC/1000);
    now = then = clock();
    while( (now-then) < pause ) {
        now = clock();
				int cg = getch();
				if(cg != ERR){*last = cg; }
			}
}
