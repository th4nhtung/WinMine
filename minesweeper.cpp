/*
 * +-----------------------------------------------+
 * | Project:	  Minesweeper					   |
 * | File:		  minesweeper.cpp				   |
 * | Author:	  Pham Thanh Tung				   |
 * | Student ID:  17020042						   |
 * | Class:		  QH - 2017 - I / CQ - C - A - C   |
 * +-----------------------------------------------+
 */

#include "minesweeper.hpp"
#include "random.hpp"
#include <iostream>
#include <sstream>
#include <SDL2/SDL.h>

// Main window of game
SDL_Window* window = NULL;

// Surface of the window
SDL_Surface* window_surface = NULL;

// Window icon
SDL_Surface* window_icon = NULL;

// Surface for each of the images
SDL_Surface* resources[N_RES];

// Events of SDL
SDL_Event events;

// Random distributor
Random* rnd = new Random();

// Function to put error
void puterr (std::string err) {
	std::cout << err << std::endl;
}

// Initialize SDL and create window for game
void initSDL (void) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		puterr(SDL_GetError());
	else {
		window = SDL_CreateWindow(GAME_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
								  SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (window == NULL)
			puterr(SDL_GetError());
		else
			window_surface = SDL_GetWindowSurface(window);
	}
}

// Load the image of the specified path and optimize the surface
// so that the subsequent blits are faster
SDL_Surface* getResource (int id) {
	std::stringstream ss;
	ss << id;
	const char* file = ("res/" + (id < 0 ? "icon" : ss.str()) + ".bmp").c_str();

	SDL_Surface* loaded_surface = SDL_LoadBMP(file);
	SDL_Surface* optimized_surface = NULL;

	if (loaded_surface == NULL) {
		puterr(SDL_GetError());
	}
	else {
		optimized_surface = SDL_ConvertSurface(loaded_surface, window_surface->format, 0);
		if (optimized_surface == NULL)
			puterr(SDL_GetError());

		// The surface is released without optimizing since we will not need it again
		SDL_FreeSurface(loaded_surface);
	}

	return optimized_surface;
}

// Load all necessary images for game
void loadImages (void) {
	REP(i, N_RES)
		resources[i] = getResource(i);
	window_icon = getResource(-1);
}

// Free memory of SDL
void freeSDL (void) {
	// Free all surfaces, except the window_surface
	REP(i, N_RES) {
		SDL_FreeSurface(resources[i]);
		resources[i] = NULL;
	}

	// Free the window and surface_window
	SDL_DestroyWindow(window);
	window = NULL;

	SDL_Quit();
}

// Initialize the two boards
void initBoard (char _board[N_ROWS][N_COLUMNS], char game_board[N_ROWS][N_COLUMNS]) {
	REP(i, N_ROWS)
		REP(j, N_COLUMNS) {
			_board[i][j] = CELL_ZERO;
			game_board[i][j] = CELL_FREE;
		}
}

// Displays the game's board
void drawBoard (char game_board[N_ROWS][N_COLUMNS]) {
	SDL_Rect rect; // Position of the surface on which the box will be drawn

	rect.w = CELL_SIZE;
	rect.h = CELL_SIZE;

	REP(i, N_ROWS)
		REP(j, N_COLUMNS) {
			rect.x = j * CELL_SIZE;
			rect.y = i * CELL_SIZE;
			SDL_BlitSurface(resources[game_board[i][j]], NULL, window_surface, &rect);
		}

	SDL_UpdateWindowSurface(window);
}

// Handle the events that are queued to check if the X in the window has been clicked
// or if a mouse button has been clicked
void handleEvents (bool* quit, bool* mouseL, bool* mouseR, int* mx, int* my) {
	while (SDL_PollEvent(&events) != 0)
		if (events.type == SDL_QUIT)
			*quit = true; // The X in the window has been pressed to exit the game
		else if (events.type == SDL_MOUSEBUTTONDOWN) {
			SDL_GetMouseState(mx, my);
			if (events.button.button == SDL_BUTTON_LEFT)
				*mouseL = true; // The left mouse button has been clicked
			else if (events.button.button == SDL_BUTTON_RIGHT)
				*mouseR = true; // The right mouse button has been clicked
		}
}

// Put mines on the hidden board by distributing them randomly
void putMines (int cell_x, int cell_y, char _board[N_ROWS][N_COLUMNS]) {
	int mines_left = N_MINES;	// Number of missing mines to be placed on the board
	int rd_row;					// Row of the cell where the mine is placed
	int rd_column;				// Column of the cell where the mine is placed

	do {
		// Randomize a cell
		rd_row = rnd->get(0, N_ROWS - 1);
		rd_column = rnd->get(0, N_COLUMNS - 1);

		// If a mine had not already been set in the box, nor is it the first space the player has uncovered
		// (cell_x, cell_y), then a mine is placed in that square
		if (_board[rd_row][rd_column] != CELL_MINE && (rd_row != cell_y || rd_column != cell_x)) {
			_board[rd_row][rd_column] = CELL_MINE;
			--mines_left;
		}
	}
	while (mines_left > 0);
}

// Return the number of mines around a cell coordinates (row, column)
char countMines (char _board[N_ROWS][N_COLUMNS], int row, int column) {
	char mines = 0;					// Count the number of mines around the square

	// The boundaries between the mines around the cell
	int max_row = row + 1; 			// Row under the cell
	int min_row = row - 1;			// Row above the cell
	int min_column = column - 1;	// Column to the left of the cell
	int max_column = column + 1;	// Column to the right of the cell

	// If the cell is on the edges of the board, then the limits must be modified
	// so as not to go off the board
	if (row == 0) min_row = 0;
	else if (row == N_ROWS - 1) max_row = N_ROWS - 1;
	if (column == 0) min_column = 0;
	else if (column == N_COLUMNS - 1) max_column = N_COLUMNS - 1;

	// Count mines around this cell
	FORU(i, min_row, max_row)
		FORU(j, min_column, max_column)
			mines += (_board[i][j] == CELL_MINE);

	// Return the number of mines around the cell
	return mines;
}

// In each cell of the board where there is no mine, put the number of mines around it
void putNumber (char _board[N_ROWS][N_COLUMNS]) {
	REP(i, N_ROWS)
		REP(j, N_COLUMNS)
			if (_board[i][j] != CELL_MINE)
				_board[i][j] = countMines(_board, i, j) + CELL_ZERO;
}

// Flip the cells that are necessary after clicking on a box and return the number of flipped cells
int flip (char _board[N_ROWS][N_COLUMNS], char game_board[N_ROWS][N_COLUMNS], int row, int column) {
	if (game_board[row][column] != CELL_FREE)
		return 0;	// If the cell was already flipped, no cells are flipped
	else {
		// The cell on the game's board is flipped, then show what was hidden in it
		game_board[row][column] = _board[row][column];

		if (game_board[row][column] != CELL_ZERO)
			return 1;	// If the cell was not CELL_ZERO, only this cell is flipped
		else {
			// Number of flipped cells
			int flipped_cells = 1;

			// The boundaries between the mines around the cell
			int max_row = row + 1; 			// Row under the cell
			int min_row = row - 1;			// Row above the cell
			int min_column = column - 1;	// Column to the left of the cell
			int max_column = column + 1;	// Column to the right of the cell

			// If the cell is on the edges of the board, then the limits must be modified
			// so as not to go off the board
			if (row == 0) min_row = 0;
			else if (row == N_ROWS - 1) max_row = N_ROWS - 1;
			if (column == 0) min_column = 0;
			else if (column == N_COLUMNS - 1) max_column = N_COLUMNS - 1;

			// Recursive flip all cells around it
			FORU(i, min_row, max_row)
				FORU(j, min_column, max_column)
					if (i != row || j != column)
						flipped_cells += flip(_board, game_board, i, j);

			return flipped_cells;	// If the cell was CELL_ZERO, more than one cell has been flipped
		}
	}
}

// Check which type of cell and which button has been clicked and act accordingly
void play (char game_board[N_ROWS][N_COLUMNS], char _board[N_ROWS][N_COLUMNS], int* free_cells,
		   bool* game_over, bool mouseL, bool mouseR, int cell_y, int cell_x) {
	// If the left mouse button has been clicked
	if (mouseL) {
		if (game_board[cell_y][cell_x] == CELL_FREE && _board[cell_y][cell_x] == CELL_MINE) {
			game_board[cell_y][cell_x] = CELL_EXPLODED;
			*game_over = true;	// You lost because you flipped a mine!
		}
		else
			*free_cells -= flip(_board, game_board, cell_y, cell_x);
	}

	// If the right mouse button has been clicked
	if (mouseR) {
		if (game_board[cell_y][cell_x] == CELL_FREE)
			game_board[cell_y][cell_x] = CELL_FLAGGED;
		else if (game_board[cell_y][cell_x] == CELL_FLAGGED)
			game_board[cell_y][cell_x] = CELL_FREE;
	}

	// If you have lost, the rest of the hidden mines are flipped
	if (*game_over)
		REP(i, N_ROWS)
			REP(j, N_COLUMNS)
				if (game_board[i][j] != CELL_EXPLODED && _board[i][j] == CELL_MINE)
					game_board[i][j] = CELL_MINE;
}

int main (int argc, char** argv) {
	char _board[N_ROWS][N_COLUMNS];	// Hidden board in which the positions of the mines will be stored
	char game_board[N_ROWS][N_COLUMNS];	// Board shown to the player on the screen

	int mouse_x, mouse_y;	// Position of the pointer in pixels
	int cell_x, cell_y;		// Position of the pointer in cells
	int free_cells;			// Number of free cells

	bool mouseL = true;
	bool mouseR = true;
	bool game_over = false;
	bool quit = false;

	// All possible status in which the game can be found
	GameStatus game_status = INITIALIZING_BOARD;

	// Position of the surface where the message of won or lost will be drawn
	SDL_Rect msg_rect;
	msg_rect.x = SCREEN_WIDTH / 2 - MESSAGE_WIDTH / 2;
	msg_rect.y = SCREEN_HEIGHT / 2 - MESSAGE_HEIGHT / 2;
	msg_rect.w = MESSAGE_WIDTH;
	msg_rect.h = MESSAGE_HEIGHT;

	initSDL();

	loadImages();

	SDL_SetWindowIcon(window, window_icon);

	// Infinite loop until user quit the game
	while (!quit) {
		handleEvents(&quit, &mouseL, &mouseR, &mouse_x, &mouse_y);
		cell_x = mouse_x / CELL_SIZE;
		cell_y = mouse_y / CELL_SIZE;

		if (mouseL || mouseR || game_over || free_cells == N_MINES) {
			switch (game_status) {
				case INITIALIZING_BOARD:
					initBoard(_board, game_board);
					drawBoard(game_board);
					free_cells = N_COLUMNS * N_ROWS;
					game_status = GENERATING_MINES;
					break;

				case GENERATING_MINES:
					putMines(cell_x, cell_y, _board);
					putNumber(_board);
					free_cells -= flip(_board, game_board, cell_y, cell_x);
					drawBoard(game_board);
					game_status = PLAYING_GAME;
					break;

				case PLAYING_GAME:
					if (free_cells > N_MINES && !game_over) {
						play(game_board, _board, &free_cells, &game_over,
							 mouseL, mouseR, cell_y, cell_x);
						drawBoard(game_board);
					}
					if (free_cells == N_MINES || game_over)
						game_status = ENDING_GAME;
					break;

				case ENDING_GAME:
					if (game_over) {
						SDL_BlitSurface(resources[GAME_LOSE], NULL, window_surface, &msg_rect);
						game_over = false;
					}
					else {
						SDL_BlitSurface(resources[GAME_WIN], NULL, window_surface, &msg_rect);
						--free_cells;
					}
					SDL_UpdateWindowSurface(window);
					game_status = INITIALIZING_BOARD;
					break;
			}

			mouseL = false;
			mouseR = false;
		}

		SDL_Delay(50);
	}

	freeSDL();

	return 0;
}
