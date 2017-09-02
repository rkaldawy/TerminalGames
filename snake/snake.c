#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

#define SCREEN_WIDTH 75
#define SCREEN_HEIGHT 22
#define Y_START 0
#define X_START 2

int len = 4;

typedef struct DOT {
	int x_pos, y_pos;
}DOT;

void drawBoard();
void drawDot(DOT* dot, int flag);
void generateFood(DOT* dot, DOT* food);
void addToEnd(DOT* dot, int last);
void delay(int milliseconds, int* last, int orig);
void initSnake(DOT* segs, int len, int start_X, int start_Y);
int checkLoss(DOT* dot);
void animateLoss(DOT* dot);
void mainMenu();
void clearScreen(int startX, int startY, int width, int height);

int main()
{

	DOT segs [100];
	DOT* head = segs;
	DOT food;

	int start_X, start_Y;
	start_X = 40;
	start_Y = 10;

	initSnake(segs, len, start_X, start_Y);

	initscr();			/* Start curses mode 		  */
	cbreak();
	curs_set(0);
	keypad(stdscr, TRUE);
	noecho();

	drawBoard();
	refresh();
	mainMenu();
	clearScreen(30, 6, 15, 20);

	nodelay(stdscr, TRUE);
	drawDot(segs, 1);
	generateFood(head, &food);


	refresh();			/* Print it on to the real screen */

	int ch, last, orig;
	last = KEY_UP;

	while(1){
		ch = getch();
		if (ch == ERR){ ch = last;}
		orig = last;

		switch(ch)
		{
			case KEY_LEFT :
				if(/*head->x_pos > X_START + 1 &&*/ last != KEY_RIGHT)
				{
				drawDot(head, 0);
				for(int i = len; i >= 1; i--) { segs[i] = segs[i-1]; }
				--head->x_pos;
				drawDot(head, 1);
				last = ch;
				}
				break;
			case KEY_RIGHT :
				if(/*head->x_pos < X_START + SCREEN_WIDTH &&*/ last != KEY_LEFT)
				{
				drawDot(head, 0);
				for(int i = len; i >= 1; i--) { segs[i] = segs[i-1]; }
				++head->x_pos;
				drawDot(head, 1);
				last = ch;
				}
				break;
			case KEY_UP :
				if(/*head->y_pos > Y_START + 1 &&*/ last != KEY_DOWN)
				{
				drawDot(head, 0);
				for(int i = len; i >= 1; i--) { segs[i] = segs[i-1]; }
				--head->y_pos;
				drawDot(head, 1);
				last = ch;
				}
				break;
		  case KEY_DOWN :
				if(/*head->y_pos < Y_START + SCREEN_HEIGHT &&*/ last != KEY_UP)
				{
				drawDot(head, 0);
				for(int i = len; i >= 1; i--) { segs[i] = segs[i-1]; }
				++head->y_pos;
				drawDot(head, 1);
				last = ch;
				}
				break;
			}

			if (checkLoss(head)) {refresh(); animateLoss(head); break;}

			if (food.x_pos == head->x_pos && food.y_pos == head->y_pos){
				generateFood(head, &food);
				addToEnd(head, last);
				drawDot(head, 1);
			}
			delay(100, &last, orig);

	}
	endwin();			/* End curses mode		  */

	return 0;
}

void animateLoss(DOT* dot){
	delay(500, NULL, 0);
	for (int i = len; i >= 1; i--){
		mvaddch(dot[i].y_pos, dot[i].x_pos, ' ');
		refresh();
		delay(50, NULL, 0);
	}
	int i = 0;
	while(i < 3)
	{	mvaddch(dot->y_pos, dot->x_pos, ' ');
		refresh();
		delay(100, NULL, 0);
		mvaddch(dot->y_pos, dot->x_pos, 'O');
		refresh();
		delay(100, NULL, 0);
		i++;
	}
}

int checkLoss(DOT* dot){

	int check = 0;
	if(dot->y_pos == Y_START + SCREEN_HEIGHT + 1 || dot->x_pos == X_START || dot->x_pos == X_START + SCREEN_WIDTH + 1 || dot->y_pos == Y_START) {
			 check = 1;
	} for (int i = 1; i <= len; i++){
		if (dot->x_pos == dot[i].x_pos && dot->y_pos == dot[i].y_pos) {
			check = 1;
			break;
		}
	}


	return check;
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
		x_pos = rand() % (SCREEN_WIDTH - 1) + X_START + 1;
		y_pos = rand() % (SCREEN_HEIGHT - 1) + Y_START + 1;
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

void delay(int milliseconds, int* last, int orig)
{
    long pause;
    clock_t now,then;
		int cL, cR, cD, cU;

    pause = milliseconds*(CLOCKS_PER_SEC/1000);
    now = then = clock();
    while( (now-then) < pause ) {
        now = clock();
				if (last != NULL){
					int cg = getch();
					if(cg != ERR){
						cL = orig != KEY_RIGHT && cg == KEY_LEFT;
						cR = orig != KEY_LEFT && cg == KEY_RIGHT;
						cU = orig != KEY_DOWN && cg == KEY_UP;
						cD = orig != KEY_UP && cg == KEY_DOWN;
						if (cL || cR || cU || cD){ *last = cg; }
					}
				}
			}
}

void initSnake(DOT* segs, int len, int start_X, int start_Y){
	segs->x_pos = start_X;
	segs->y_pos = start_Y;
	segs++;
	for (int i = 1; i<= len; i++){
		segs->x_pos = start_X;
		segs->y_pos = start_Y + i;
		segs++;
	}

}

void clearScreen(int startX, int startY, int width, int height){
	 for (int i = startY; i<startY+width; i++){
		 for (int j = startX; j<startX+height; j++){
			 mvaddch(i, j, ' ');
		 }
	 }
	 refresh();

}
 void mainMenu()
 {
	 	mvprintw(6, 30, "SNAKE!");
		mvprintw(8, 30, "by Remy Kaldawy");
		mvprintw(12, 35, "Play");
		mvprintw(14, 35, "Options");
		mvprintw(16, 35, "High Scores");

		int play, pos, key, y_pos;
		play = 1;
		pos = 1;

		while(play){
			y_pos = 12 + (2 * pos);
			mvaddch(y_pos, 30, '>');
			key = getch();
			if (key == KEY_RIGHT)
				play = 0;
			else if (key == KEY_UP && pos > 0) {
				mvaddch(y_pos, 30, ' ');
				pos--;
			} else if (key == KEY_DOWN && pos < 2) {
				mvaddch(y_pos, 30, ' ');
				pos++;
			}
		}

 }
