#include<ncurses.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

#define ON 1
#define OFF 0
#define SIZE 7
#define START_X 30
#define START_Y 1
#define X_INCR 10
#define Y_INCR 4

short table[SIZE][SIZE];
int result = FALSE;

void show_stats()
{
	int on_bulbs = 0, off_bulbs = 0;
	int i, j;
	int flag = 0;
	char   buffer[32];
	for(i=0; i<SIZE; i++)
	{
		for(j=0; j < SIZE; j++)
		{
			if(table[i][j] == OFF)
				++off_bulbs;
			else
			{
				++on_bulbs;
				flag = 1;				
			}
		}
	}
	if(flag == 0)
		result = TRUE;
	
	sprintf(buffer, "BULBS ON  : %02d", on_bulbs);
	move(1, 3);
	addstr(buffer);
	
	sprintf(buffer, "BULBS OFF : %02d", off_bulbs);
	move(2, 3);
	addstr(buffer);	
}

void printTable(int rows, int columns)
{
	char   buffer[10];
	int i, j;	
	int x=START_X, y=START_Y;
	
	for(i = 0; i< rows; i++)
	{
		x += X_INCR;
		y = START_Y;
		for(j=0; j<columns; j++)
		{
			y+=Y_INCR;
			move(y, x);
			sprintf(buffer, "%d", table[i][j]);
			addstr(buffer);
		}
	}
	show_stats();
}

void swap_int(int* a, int* b){ int tmp = *a; *a=*b; *b=tmp; }

inline int rand_int_hi_lo(int upper, int lower){
  return((rand() % (upper-lower+1)) + lower);
}

int rand_int(int a,int b)
{
    if (b > a) swap_int(&a,&b);
    return rand_int_hi_lo(a,b);
}


void initTable()
{
	char   buffer[10];
	int i, j, x, y, n;
	for(i=0; i<SIZE; i++)
	{
		for(j=0; j<SIZE; j++)
		{
			table[i][j] = OFF;
		}
	}
	n = rand_int(7, 10);
	n = rand_int(1, 3);
	
	for(i=0; i<n; i++)
	{
		x = rand_int(1,SIZE);
		y = rand_int(1,SIZE);
		if(x < 0 || y < 0 || x >= SIZE || y >= SIZE)
			continue;
		table[x][y] = ON;
	}
}

void inline toggle(int i, int j)
{
	table[i][j] = table[i][j] == ON ? OFF : ON;
}

void toggle_neighbours(int a, int b)
{
	int i, j, x, y;	
	for( i = -1; i < 2; i++)
	{
		x = a + i;
		if(x < 0 || x >= SIZE)
			continue;
			
		for( j = -1; j < 2; j++)
		{
			y = b + j;
			if(y < 0 || y >= SIZE)
				continue;
				
			toggle(x, y);				
		}
	}
}

void toggle_on_click(int x, int y)
{
	char   buffer[10];
	int i, j;
	
	int startx = START_X;
	int starty = START_Y;

	if(result == TRUE)
	{
		return;	
	}
	
	for(i=0; i<SIZE+1; i++)
	{
		if(x > (startx - X_INCR/2) && x < (startx + X_INCR/2))
			break; 
		startx += X_INCR;
	}
	if( i >= (SIZE+1) ) return;
	
	for(j=0; j<SIZE+1; j++)
	{
		if(y > (starty - Y_INCR/2) && y < (starty + Y_INCR/2))
			break; 
		starty += Y_INCR;
	}
	if( j >= (SIZE+1) ) return;
	
	i--; j--;
	toggle_neighbours(i, j);
	printTable(SIZE, SIZE);	
}

int main()
{
	int x, y, c;
	MEVENT event;
	
	initscr();		
	curs_set(1);
	keypad(stdscr, TRUE);
	mousemask(ALL_MOUSE_EVENTS, NULL);	
	
	initTable();
	printTable(SIZE, SIZE);	
	
	while (result == FALSE) 
	{
		c = wgetch(stdscr);
		if (KEY_MOUSE == c) 
		{
			if (OK == getmouse(&event))
            {
                x = event.x;
                y = event.y;                
                toggle_on_click(x, y);                
			}
		}
	}
	start_color();
	init_pair(1, COLOR_GREEN, COLOR_RED);
	attron(COLOR_PAIR(1));	
	printw("Wow You Win....");
	
	getch();
	endwin();
	return 0;
}
