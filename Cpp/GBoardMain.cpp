#include "GBoard.hpp"
#include <iostream>

int main ()
{
	std::cout << "\nThis is a game of Tic-Tac-Toe. First to get five in a row wins!\n";
	std::cout << "Please enter the row and column (1-15) and player ('x' or 'o') when prompted.\n\n";

	GBoard rp1;
	char row, col, player;
	int rowNum, colNum;
	while (1)
	{
		std::cout << "Row: ";
		std::cin >> row;
		std::cout << "Column: ";
		std::cin >> col;
		std::cout << "Player: ";
		std::cin >> player;
		std::cout << "\n";
		
		rowNum = row - '0';
		colNum = col - '0';
		rp1.makeMove(rowNum, colNum, player);

		if (rp1.hasWon(rowNum, colNum, player))
		{
			std::cout << "Player " << player << " has won!\n";
			return 0;
		}
		else if (rp1.isFull())
		{
			std::cout << "Game is a draw :(\n";
			return 0;
		}
	}
}