/***************************************************************************
** Author:		Gaetan Ingrassia
** Date:		2018-11-28
** Description:	This implements the class GBoard. It contains a default
**				constructor to initialize the elements of the board to empty
**				and the game state to unfinished. It has methods to get the
**				game state, make moves, determine who has won, and if the 
**				board is full.  
***************************************************************************/


#include "GBoard.hpp"


// Default Constructor
GBoard::GBoard()
{
	// Set the space character to represent an empty space on the board
	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < 15; j++)
			board[i][j] = ' ';
	}

	gameState = UNFINISHED;
}

GameState GBoard::getGameState()
{
	return gameState;
}

// Places an 'x' or an 'o' at the location specified by the input,
// returns true if the operation was completed, and changes the game
// state if necessary 
bool GBoard::makeMove(int row, int col, char player)
{
	// Return false if the space is not empty or the game is finished
	if ((board[row][col] != ' ') || (gameState != UNFINISHED))
		return false;

	// Place 'x' or 'o'
	board[row][col] = player;

	// Change game state if needed
	if (player == 'x' && hasWon(row, col, player))
		gameState = X_WON;
	else if (player == 'o' && hasWon(row, col, player))
		gameState = O_WON;
	else if (isFull())
		gameState = DRAW;

	return true;
}

// Returns true if the board is full by checking for no spaces
bool GBoard::isFull()
{
	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < 15; j++)
		{
			if (board[i][j] == ' ')
				return false;
		}
	}

	return true;
}

// Checks the elements adjacent to the input element for 4 more x's or o's
// in a row, column, or diagonally and returns true if that is the case
bool GBoard::hasWon(int row, int col, char player)
{
	int counter;
	int rowChecker;
	int colChecker;

	// Checks the row behind and in front of the element
	counter = 0;
	colChecker = row;
	rowChecker = col - 1;
	while (rowChecker >= 0 && rowChecker <= 14 && board[colChecker][rowChecker] == player)
	{
		counter++;
		rowChecker--;
	}
	colChecker = row;
	rowChecker = col + 1;
	while (rowChecker >= 0 && rowChecker <= 14 && board[colChecker][rowChecker] == player)
	{
		counter++;
		rowChecker++;
	}
	if (counter == 4)
		return true;

	// Checks the column above and below the element
	counter = 0;
	colChecker = row - 1;
	rowChecker = col;
	while (colChecker >= 0 && colChecker <= 14 && board[colChecker][rowChecker] == player)
	{
		counter++;
		colChecker--;
	}
	colChecker = row + 1;
	rowChecker = col;
	while (colChecker >= 0 && colChecker <= 14 && board[colChecker][rowChecker] == player)
	{
		counter++;
		colChecker++;
	}
	if (counter == 4)
		return true;

	// Checks the backward slanting diagonal
	counter = 0;
	colChecker = row - 1;
	rowChecker = col - 1;
	while (rowChecker >= 0 && rowChecker <= 14 && colChecker >= 0 
			&& colChecker <= 14 && board[colChecker][rowChecker] == player)
	{
		counter++;
		colChecker--;
		rowChecker--;
	}
	colChecker = row + 1;
	rowChecker = col + 1;
	while (rowChecker >= 0 && rowChecker <= 14 && colChecker >= 0 
			&& colChecker <= 14 && board[colChecker][rowChecker] == player)
	{
		counter++;
		colChecker++;
		rowChecker++;
	}
	if (counter == 4)
		return true;

	// Checks the forward slanting diagonal
	counter = 0;
	colChecker = row - 1;
	rowChecker = col + 1;
	while (rowChecker >= 0 && rowChecker <= 14 && colChecker >= 0 
			&& colChecker <= 14 && board[colChecker][rowChecker] == player)
	{
		counter++;
		colChecker--;
		rowChecker++;
	}
	colChecker = row + 1;
	rowChecker = col - 1;
	while (rowChecker >= 0 && rowChecker <= 14 && colChecker >= 0 
			&& colChecker <= 14 && board[colChecker][rowChecker] == player)
	{
		counter++;
		colChecker++;
		rowChecker--;
	}
	if (counter == 4)
		return true;

	return false;
}