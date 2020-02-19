/*****************************************************************************
** Author:		Gaetan Ingrassia
** Date:		2018-11-28
** Description:	This header file declares the class GBoard. It defines a 15x15
**				game board where 'x' or 'o' can get 5 in a row and either win or
**				finish with a draw. The member functions make moves for the 'x'
**				and 'o' players and return the state of the game. 
*****************************************************************************/


#ifndef GBOARD_HPP
#define GBOARD_HPP

enum GameState {X_WON, O_WON, DRAW, UNFINISHED};


class GBoard
{
	private:
		char board[15][15];
		GameState gameState;
	public:
		GBoard();
		GameState getGameState();
		bool makeMove(int, int, char);
		bool isFull();
		bool hasWon(int, int, char);
};
#endif