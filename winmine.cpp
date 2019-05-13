/*
 * +-----------------------------------------------+
 * | Project:     Minesweeper                      |
 * | Author:      Pham Thanh Tung                  |
 * | Student ID:  17020042                         |
 * | Class:       QH - 2017 - I / CQ - C - A - C   |
 * +-----------------------------------------------+
 */

#include "winmine.hpp"
#include "random.hpp"
#include <iostream>
#include <sstream>
#include <utility>
#include <vector>

// Main window of game
SDL_Window* window = NULL;

// Surface of the window
SDL_Surface* layer = NULL;

// Window icon
SDL_Surface* window_icon = NULL;

// Surface for resources
SDL_Surface* resources[N_RES];

// Events of SDL
SDL_Event events;

// Check in-game event
bool mouseL, mouseR, exploded, quit;

// Mouse position
int mX, mY;

// Time of game
clock_t start_time, end_time;

// Random distributor
Random* rnd = new Random();

// Initialize SDL
void initSDL (void) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		std::cerr << "SDL Error: " << SDL_GetError() << std::endl;
	else {
		window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, LAYER_WIDTH, LAYER_HEIGHT, SDL_WINDOW_SHOWN);
		if (window == NULL)
			std::cerr << "SDL Error: " << SDL_GetError() << std::endl;
		else
			layer = SDL_GetWindowSurface(window);
	}
}

// Load an image
SDL_Surface* getResource (int id) {
	std::stringstream ss;
	ss << id;
	const char* file = ("res/" + (id < 0 ? "icon" : ss.str()) + ".bmp").c_str();

	SDL_Surface* tmp_srf = SDL_LoadBMP(file);
	SDL_Surface* opt_srf = NULL;

	if (tmp_srf == NULL) {
		std::cerr << "SDL Error: " << SDL_GetError() << std::endl;
	}
	else {
		opt_srf = SDL_ConvertSurface(tmp_srf, layer->format, 0);
		if (opt_srf == NULL)
			std::cerr << "SDL Error: " << SDL_GetError() << std::endl;

		SDL_FreeSurface(tmp_srf);
	}

	return opt_srf;
}

// Load all necessary images for game
void loadImages (void) {
	window_icon = getResource(-1);

	REP(i, N_RES)
		resources[i] = getResource(i);
}

// Free memory of SDL
void freeSDL (void) {
	REP(i, N_RES) {
		SDL_FreeSurface(resources[i]);
		resources[i] = NULL;
	}

	SDL_DestroyWindow(window);
	window = NULL;

	SDL_Quit();
}

// Draw board to layer
void drawBoard (char board[N_ROWS][N_COLUMNS]) {
	SDL_Rect rect; // Position to draw cell

	rect.w = rect.h = CELL_SIZE;

	REP(i, N_ROWS)
		REP(j, N_COLUMNS) {
			rect.x = j * CELL_SIZE;
			rect.y = i * CELL_SIZE;
			SDL_BlitSurface(resources[(int)board[i][j]], NULL, layer, &rect);
		}

	SDL_UpdateWindowSurface(window);
}

// Catch in-game events
void catchEvents (void) {
	while (SDL_PollEvent(&events) != 0)
		if (events.type == SDL_QUIT) // The X in the window has been clicked
			quit = true;
		else if (events.type == SDL_MOUSEBUTTONDOWN) { // A mouse button has been clicked
			SDL_GetMouseState(&mX, &mY);
			if (events.button.button == SDL_BUTTON_LEFT) // Left mouse button has been clicked
				mouseL = true;
			else if (events.button.button == SDL_BUTTON_RIGHT) // Right mouse button has been clicked
				mouseR = true;
		}
}

// Check if a value is number
bool isNumber (char value) {
	return CELL_ZERO <= value && value <= CELL_EIGHT;
}

// Check if coordinate is out of board
bool outOfBoard (int row, int column) {
	return (row < 0 || row >= N_ROWS || column < 0 || column >= N_COLUMNS);
}

// Initialize boards
void initBoard (char hidden[N_ROWS][N_COLUMNS], char display[N_ROWS][N_COLUMNS]) {
	REP(i, N_ROWS)
		REP(j, N_COLUMNS) {
			hidden[i][j] = CELL_ZERO;
			display[i][j] = CELL_FREE;
		}
}

// Return the number of mines around a cell
char countMines (char board[N_ROWS][N_COLUMNS], int row, int column) {
	char mines = 0;	// Count the number of mines around the square

	FORU(i, row - 1, row + 1)
		FORU(j, column - 1, column + 1)
			if (!outOfBoard(i, j))
				mines += (board[i][j] == CELL_MINE);

	return mines;
}

// Put mines and numbers
void genBoard (int _row, int _column, char board[N_ROWS][N_COLUMNS]) {
	std::vector <std::pair<int, int>> cellList; // List of free cells

	REP(i, N_ROWS)
		REP(j, N_COLUMNS)
			cellList.push_back(std::make_pair(i, j));

	REP(k, N_MINES) {
		// Randomize a cell
		int rd_cell = rnd->get(0, cellList.size() - 1);

		int row = cellList[rd_cell].first;
		int column = cellList[rd_cell].second;

		// If this cell is not the first clicked cell, then put a mine there
		if (row != _row || column != _column) {
			board[row][column] = CELL_MINE;
			cellList.erase(cellList.begin() + rd_cell);
		}
	}

	REP(i, N_ROWS)
		REP(j, N_COLUMNS)
			if (board[i][j] != CELL_MINE)
				board[i][j] = countMines(board, i, j) + CELL_ZERO;
}

// Return the number of flagged cells around a cell
char countFlags (char board[N_ROWS][N_COLUMNS], int row, int column) {
	char flags = 0;	// Count the number of flags around the square

	// Count flags around this cell
	FORU(i, row - 1, row + 1)
		FORU(j, column - 1, column + 1)
			if (!outOfBoard(i, j))
				flags += (board[i][j] == CELL_FLAGGED);

	// Return the number of flags around the cell
	return flags;
}

// Flip the cells and return the number of flipped cells
int flip (char hidden[N_ROWS][N_COLUMNS], char display[N_ROWS][N_COLUMNS], int row, int column) {
	if (display[row][column] == CELL_FREE && hidden[row][column] == CELL_MINE) {
		display[row][column] = CELL_EXPLODED;
		exploded = true;
	}
	if (display[row][column] != CELL_FREE) {
		if (isNumber(display[row][column]) && countFlags(display, row, column) == display[row][column] - CELL_ZERO)
			return flip_adj(hidden, display, row, column);
		else
			return 0;	// If the cell was already flipped, no cells are flipped
	}
	else {
		// This cell is flipped, then show it
		display[row][column] = hidden[row][column];

		if (display[row][column] != CELL_ZERO)
			return 1;	// If this cell was not CELL_ZERO, then only this cell is flipped
		else {
			// Number of flipped cells (initialize as 1 included this cell!)
			int flipped_cells = 1;

			// Recursive flip all cells around it
			FORU(i, row - 1, row + 1)
				FORU(j, column - 1, column + 1)
					if (!outOfBoard(i, j) && (i != row || j != column))
						flipped_cells += flip(hidden, display, i, j);

			return flipped_cells;	// If the cell was CELL_ZERO, then more than one cell has been flipped
		}
	}
}

// Flip all adjacent cells if all flags are put around it is the same as the number inside this cell
int flip_adj (char hidden[N_ROWS][N_COLUMNS], char display[N_ROWS][N_COLUMNS], int row, int column) {
	if (display[row][column] == CELL_FREE && display[row][column] == CELL_MINE) {
		display[row][column] = CELL_EXPLODED;
		exploded = true;
	}
	// Number of flipped cells
	int flipped_cells = 0;

	// Recursive flip all cells around it
	FORU(i, row - 1, row + 1)
		FORU(j, column - 1, column + 1)
			if (!outOfBoard(i, j) && (i != row || j != column) && display[i][j] == CELL_FREE)
				flipped_cells += flip(hidden, display, i, j);

	return flipped_cells;
}

// Act depending on type of cell and clicked button
void play (char hidden[N_ROWS][N_COLUMNS], char display[N_ROWS][N_COLUMNS], int& unknown_cells, int row, int column) {
	// Flip cell using left mouse
	if (mouseL) {
		unknown_cells -= flip(hidden, display, row, column);
	}

	// Mark as flag or remove flag using right mouse
	if (mouseR) {
		char& cell = display[row][column];
		cell = (cell == CELL_FLAGGED ? CELL_FREE : (cell == CELL_FREE ? CELL_FLAGGED : cell));
	}

	// If you have lost, the rest of the hidden mines are flipped
	if (exploded)
		REP(i, N_ROWS)
			REP(j, N_COLUMNS) {
				if (display[i][j] == CELL_FREE && hidden[i][j] == CELL_MINE)
					display[i][j] = CELL_MINE;
				if (display[i][j] == CELL_FLAGGED && isNumber(hidden[i][j]))
					display[i][j] = CELL_WRONG_FLAGGED;
			}
}

// Show message when game ends
void showMessage (bool exp) {
	// Create message string
	std::stringstream msg;
	msg.setf(std::ios::fixed, std::ios::floatfield);
	msg.precision(3);
	msg << "You " << (exp ? "lose" : "win") << "! Time: " << (double) (end_time - start_time) / CLOCKS_PER_SEC << " s";

	bool quit = false;

    TTF_Init();

    // Show message
    SDL_Window* window = SDL_CreateWindow(MESSAGE_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, MESSAGE_WIDTH, MESSAGE_HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_SetWindowIcon(window, window_icon);

    TTF_Font* font = TTF_OpenFont("res/times.ttf", MESSAGE_FONTSIZE);
    SDL_Color color[2] = {{63, 72, 204}, {255, 255, 255}};
    SDL_Color bg[2] = {{0, 255, 0, 255}, {237, 28, 36, 255}};

    SDL_SetRenderDrawColor(renderer, bg[exp].r, bg[exp].g, bg[exp].b, bg[exp].a);
    SDL_RenderClear(renderer);

    SDL_Surface* surface = TTF_RenderText_Blended(font, msg.str().c_str(), color[exp]);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    int msgW = MESSAGE_WIDTH;
    int msgH = MESSAGE_HEIGHT;
    SDL_QueryTexture(texture, NULL, NULL, &msgW, &msgH);
    SDL_Rect dstrect = {0, 0, msgW, msgH};

	SDL_RenderCopy(renderer, texture, NULL, &dstrect);
	SDL_RenderPresent(renderer);

    while (!quit)
		while (SDL_PollEvent(&events) != 0)
			quit = (events.window.event == SDL_WINDOWEVENT_CLOSE);

	// Free SDL

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
}

int main (int argc, char** argv) {
	char hidden_board[N_ROWS][N_COLUMNS], display_board[N_ROWS][N_COLUMNS]; // Board of game
	int unknown_cells; // Number of cell which has not been flipped

	// At least one of the mouse button must be initialize as true so the game can play
	mouseL = true; mouseR = quit = false;

	GameStatus game_status = WAITING;

	initSDL();

	loadImages();

	SDL_SetWindowIcon(window, window_icon);

	// Infinite loop until user exit the game
	while (!quit) {
		catchEvents();

		if (mouseL || mouseR || exploded || unknown_cells == N_MINES) {
			int row = mY / CELL_SIZE;
			int column = mX / CELL_SIZE;

			if (game_status == WAITING) {
				initBoard(hidden_board, display_board);
				drawBoard(display_board);
				unknown_cells = N_COLUMNS * N_ROWS;
				game_status = GENERATING;
			}
			else if (game_status == GENERATING) {
				genBoard(row, column, hidden_board);
				start_time = clock();
				unknown_cells -= flip(hidden_board, display_board, row, column);
				drawBoard(display_board);
				game_status = PLAYING;
			}
			else if (game_status == PLAYING) {
				if (unknown_cells > N_MINES && !exploded) {
					play(hidden_board, display_board, unknown_cells, row, column);
					drawBoard(display_board);
				}
				else
					game_status = FINISHED;
			}
			else if (game_status == FINISHED) {
				end_time = clock();
				showMessage(exploded);

				if (exploded) exploded = false;
				else --unknown_cells;

				game_status = WAITING;
			}
			mouseL = mouseR = false;
		}

		SDL_Delay(27); // Just my lucky value: 4 + 8 + 6 + 9 :))
	}

	freeSDL();

	return 0;
}
