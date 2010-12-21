// A game of life simulation by Jeroen F.C. Hendrickx (c) 2010

#include <stdio.h>;
#include <stdlib.h>;
#include <curses.h>;
#include <time.h>;

#define GRID_SIZE 50
#define LENGTH 1000
#define SPEED 100

void fillGrid(int grid[GRID_SIZE][GRID_SIZE]);
void iterateGrid(int grid[GRID_SIZE][GRID_SIZE]);
int countNeighbours(int grid[GRID_SIZE][GRID_SIZE], int x, int y);
void finish(int sig);
void printGrid(int grid[GRID_SIZE][GRID_SIZE]);

int main() {
	signal(SIGINT, finish);
	initscr();
	refresh();

	int grid[GRID_SIZE][GRID_SIZE];

	fillGrid(grid);

	printGrid(grid);

	int i;
	for(i = 0; i < LENGTH; ++i) {
		struct timespec requestedTime, elapsedTime;
		requestedTime.tv_sec = 0;
		requestedTime.tv_nsec = SPEED * 1000 * 1000 - 1; //has to be lower than 9 billion

		while((requestedTime.tv_sec + requestedTime.tv_nsec) > 0)
		{
		   nanosleep(&requestedTime, &elapsedTime);
		   requestedTime.tv_sec = elapsedTime.tv_sec;
		   requestedTime.tv_nsec = elapsedTime.tv_nsec;
		}
		iterateGrid(grid);
		printGrid(grid);
	}

	//TODO: use ncurses to manage "windows" in output. ("http://invisible-island.net/ncurses/ncurses-intro.html#curses")
	return 0;
}

void printGrid(int grid[GRID_SIZE][GRID_SIZE]) {

	clear();

	int i, j;
	for(i = 0; i < GRID_SIZE; ++i) {
		for(j = 0; j < GRID_SIZE; ++j)
			printw(grid[i][j] == 0? " " : "X");
		printw("\n");
	}

	refresh();
}

void iterateGrid(int grid[GRID_SIZE][GRID_SIZE]) {
	static int newGrid[GRID_SIZE][GRID_SIZE];

	int i, j;
	for (i = 0; i < GRID_SIZE; ++i) {
		for (j = 0; j < GRID_SIZE; ++j) {
			int liveNeighbours = countNeighbours(grid, i, j);
			if (liveNeighbours < 2 || liveNeighbours > 3) {
				newGrid[i][j] = 0;
			} else if (liveNeighbours == 2 && grid[i][j] == 1) {
				newGrid[i][j] = 1;
			} else if (liveNeighbours == 3) {
				newGrid[i][j] = 1;
			}
		}
	}
	
	for (i = 0; i < GRID_SIZE; ++i) {
		for (j = 0; j < GRID_SIZE; ++j) {
			grid[i][j] = newGrid[i][j];
		}
	}
}

int countNeighbours(int grid[GRID_SIZE][GRID_SIZE], int x, int y) {
	int total = 0;
	//top left
	if(x - 1 >= 0 && y - 1 >= 0 && grid[x - 1][y - 1] == 1) {
		total++;
	}
	//center left
	if(x - 1 >= 0 && grid[x - 1][y] == 1) {
		total++;
	}
	//bottom left
	if(x - 1 >= 0 && y + 1 < GRID_SIZE && grid[x - 1][y + 1] == 1) {
		total++;
	}
	//top center
	if(y - 1 >= 0 && grid[x][y - 1] == 1) {
		total++;
	}
	//bottom center
	if(y + 1 < GRID_SIZE && grid[x][y + 1] == 1) {
		total++;
	}
	//top right
	if(x + 1 < GRID_SIZE && y - 1 >= 0 && grid[x + 1][y - 1] == 1) {
		total++;
	}
	//center right
	if(x + 1 < GRID_SIZE && grid[x + 1][y] == 1) {
		total++;
	}
	//bottom right
	if(x + 1 < GRID_SIZE && y + 1 < GRID_SIZE && grid[x + 1][y + 1] == 1) {
		total++;
	}
	return total;
}

void fillGrid(int grid[GRID_SIZE][GRID_SIZE]) {
	
	int i, j;

	//start new random number series
	int timeValue = time(0);
	srand(timeValue);

	//fill grid
	for(i = 0; i < GRID_SIZE; ++i) {
		for(j = 0; j < GRID_SIZE; ++j) {
			grid[i][j] = rand() % 2;
		}
	}
}

void finish(int sig) {
	endwin();
	exit(0);
}
