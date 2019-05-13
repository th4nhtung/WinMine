/*
 * +-----------------------------------------------+
 * | Project:     Minesweeper                      |
 * | Author:      Pham Thanh Tung                  |
 * | Student ID:  17020042                         |
 * | Class:       QH - 2017 - I / CQ - C - A - C   |
 * +-----------------------------------------------+
 */

/**
 * @file winmine.hpp
 * @author Tung Pham Thanh
 * @brief This is a header file for game core
 */

#ifndef WINMINE_HPP_INCLUDED
#define WINMINE_HPP_INCLUDED

#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#define TITLE "Minesweeper"
#define MESSAGE_TITLE "Statistic"
#define FORU(i, a, b) for (int i = (a); i <= (b); ++i)
#define REP(i, n) for (int i = 0; i < (n); ++i)

// These values are collected from classic Minesweeper on Windows XP (Expert level)

const int N_ROWS			= 16;						// Number of board rows
const int N_COLUMNS 		= 30;						// Number of board columns
const int N_MINES 			= 99;						// Number of board mines
const int CELL_SIZE			= 16;						// Size of cell (calc. in pixels)
const int LAYER_HEIGHT		= CELL_SIZE * N_ROWS;		// Height of the layer (calc. in pixels)
const int LAYER_WIDTH		= CELL_SIZE * N_COLUMNS;	// Width of the layer (calc. in pixels)
const int MESSAGE_HEIGHT 	= 50;						// Height of the message (calc. in pixels)
const int MESSAGE_WIDTH 	= 400;						// Width of the message (calc. in pixels)
const int MESSAGE_FONTSIZE 	= 30;						// Font size of the message

const int N_RES = 14;

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
	CELL_WRONG_FLAGGED
};

enum GameStatus {
	WAITING,
	GENERATING,
	PLAYING,
	FINISHED
};

/**
 * @brief Initialize SDL, create window and layer
 */

void initSDL (void);

/**
 * @brief Load an image
 * @param[in] id The ID of resource
 * @return A pointer to SDL_Surface which contains image
 */

SDL_Surface* getResource (int id);

/**
 * @brief Load all necessary images for game
 */

void loadImages (void);

/**
 * @brief Free memory of SDL
 */

void freeSDL (void);

/**
 * @brief Draw board to layer
 * @param[in] board An array of data to draw to layer
 */

void drawBoard (char board[N_ROWS][N_COLUMNS]);

/**
 * @brief Catch in-game events
 */

void catchEvents (void);

/**
 * @brief Check if a value is number
 * @param[in] value A value to check whether it is a number cell
 * @return A boolean value, true if it is a number
 */

bool isNumber (char value);

/**
 * @brief Check if coordinate is out of board
 * @param[in] row Row of cell
 * @param[in] column Column of cell
 * @return A boolean value, true if this cell is out of board
 */

bool outOfBoard (int row, int column);

/**
 * @brief Initialize boards
 * @param[out] hidden The hidden board, contains data of current game
 * @param[out] display The board to show player
 */

void initBoard (char hidden[N_ROWS][N_COLUMNS], char display[N_ROWS][N_COLUMNS]);

/**
 * @brief Return the number of mines around a cell
 * @param[in] board The board which contains this cell
 * @param[in] row Row of cell
 * @param[in] column Column of cell
 * @return Number of mines around this cell
 */

char countMines (char board[N_ROWS][N_COLUMNS], int row, int column);

/**
 * @brief Put mines and numbers
 * @param[in] _row Row of the first clicked cell
 * @param[in] _column Column of the first clicked cell
 * @param[out] board Board to put mines and numbers in
 */

void genBoard (int _row, int _column, char board[N_ROWS][N_COLUMNS]);

/**
 * @brief Return the number of flagged cells around a cell
 * @param[in] board The board which contains this cell
 * @param[in] row Row of cell
 * @param[in] column Column of cell
 * @return Number of flags have been put around it
 */

char countFlags (char board[N_ROWS][N_COLUMNS], int row, int column);

/**
 * @brief Flip the cells and return the number of flipped cells
 * @param[in] hidden The hidden board, contains data of current game
 * @param[in] display The board to show player
 * @param[in] row Row of the clicked cell
 * @param[in] column Column of the clicked cell
 * @return Number of cells have been flipped
 */

int flip (char hidden[N_ROWS][N_COLUMNS], char display[N_ROWS][N_COLUMNS], int row, int column);

/**
 * @brief Flip all adjacent cells if all flags are put around it is the same as the number inside this cell
 * @param[in] hidden The hidden board, contains data of current game
 * @param[in] display The board to show player
 * @param[in] row Row of the clicked cell
 * @param[in] column Column of the clicked cell
 * @return Number of cells have been flipped
 */

int flip_adj (char hidden[N_ROWS][N_COLUMNS], char display[N_ROWS][N_COLUMNS], int row, int column);

/**
 * @brief Act depending on type of cell and clicked button
 * @param[in] hidden The hidden board, contains data of current game
 * @param[in] display The board to show player
 * @param[out] unknown_cells Number of cells which have not been flipped
 * @param[in] row Row of the clicked cell
 * @param[in] column Column of the clicked cell
 */

void play (char hidden[N_ROWS][N_COLUMNS], char display[N_ROWS][N_COLUMNS], int& unknown_cells, int row, int column);

/**
 * @brief Show message when game ends
 * @param[in] exp True if a mine has been clicked and exploded
 */

void showMessage (bool exp);

#endif // WINMINE_HPP_INCLUDED
