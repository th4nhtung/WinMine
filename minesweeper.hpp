/*
 * +-----------------------------------------------+
 * | Project:	  Minesweeper					   |
 * | File:		  minesweeper.hpp				   |
 * | Author:	  Pham Thanh Tung				   |
 * | Student ID:  17020042						   |
 * | Class:		  QH - 2017 - I / CQ - C - A - C   |
 * +-----------------------------------------------+
 */

#ifndef __MINESWEEPER_HEADER__
#define __MINESWEEPER_HEADER__

#include <string>
#include <SDL2/SDL.h>

#define GAME_TITLE "Minesweeper"
#define FORU(i, a, b) for (int i = (a); i <= (b); ++i)
#define REP(i, n) for (int i = 0; i < (n); ++i)

// These values are collected from classic Minesweeper on Windows XP (Expert level)

const int N_ROWS			= 16;						// Number of board rows
const int N_COLUMNS 		= 30;						// Number of board columns
const int N_MINES 			= 99;						// Number of board mines
const int CELL_SIZE			= 30;						// Size of cell (calc. in pixels)
const int SCREEN_HEIGHT		= CELL_SIZE * N_ROWS;		// Height of the window (calc. in pixels)
const int SCREEN_WIDTH		= CELL_SIZE * N_COLUMNS;	// Width of the window (calc. in pixels)
const int MESSAGE_HEIGHT 	= 120;						// Height of the message (calc. in pixels)
const int MESSAGE_WIDTH 	= 540;						// Width of the message (calc. in pixels)

const int N_RES = 15;

enum ResourceID {
	CELL_FREE,
	CELL_FLAGGED,
	CELL_MINE,
	CELL_ZERO,
	CELL_ONE,
	CELL_TWO,
	CELL_THREE,
	CELL_FOUR,
	CELL_FIVE,
	CELL_SIX,
	CELL_SEVEN,
	CELL_EIGHT,
	CELL_EXPLODED,
	GAME_WIN,
	GAME_LOSE
};

enum GameStatus {
	INITIALIZING_BOARD,
	GENERATING_MINES,
	PLAYING_GAME,
	ENDING_GAME
};

// Initialize SDL and create window for game
void initSDL (void);

// Load the image of the specified path and optimize the surface
// so that the subsequent blits are faster
SDL_Surface* getResource (int);

// Load all necessary images for game
void loadImages (void);

// Free memory of SDL
void freeSDL (void);

// Initialize the two boards
void initBoard (char**, char**);

// Displays the player's board
void drawBoard (char**);

// Handle the events that are queued to check if the X in the window has been clicked
// or if a mouse button has been clicked
void handleEvents (bool*, bool*, bool*, int*, int*);

// Put mines on the hidden board by distributing them randomly
void putMines (int, int, char**);

// Return the number of mines around a cell coordinates (row, column)
char countMines (char**, int, int);

// In each cell of the board where there is no mine, put the number of mines around it
void putNumber (char**);

// Flip the cells that are necessary after clicking on a box and return the number of flipped cells
int flip (char**, char**, int, int);

// Check which type of cell and which button has been clicked and act accordingly
void play (char**, char**, int*, bool*, bool, bool, int, int);

#endif // __MINESWEEPER_HEADER__
