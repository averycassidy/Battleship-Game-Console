#include <iostream>
#include "Board.h"

Board::Board() {

	int rowNum = sizeof(board) / sizeof(board[0]);
	int colNum = sizeof(board[0]) / sizeof(board[0][0]);

	for (int row = 0; row < rowNum; ++row) {
		for (int col = 0; col < colNum; ++col)
			board[row][col] = ' ';
	}
}

bool Board::empty(Location loc) {
	if (loc.withinRange())
		return board[loc.rowIndex()][loc.colIndex()] == ' ';

	return false;
}

void Board::set(Location loc, char type) {
	if (loc.withinRange())
		board[loc.rowIndex()][loc.colIndex()] = type;
	else
		std::cout << "invalid set location" << std::endl;
}

char Board::get(Location loc) {
	if (loc.withinRange())
		return board[loc.rowIndex()][loc.colIndex()];
	else
		return NULL;
}

bool Board::has_ships() {
	int rowNum = sizeof(board) / sizeof(board[0]);
	int colNum = sizeof(board[0]) / sizeof(board[0][0]);

	for (int row = 0; row < rowNum; ++row) {
		for (int col = 0; col < colNum; ++col) {
			if (board[row][col] != ' ' && board[row][col] != 'O' && board[row][col] != 'X')
				return true;
		}
	}

	return false;
}

std::ostream& operator<<(std::ostream& output, const Board& game) {

	int rowNum = sizeof(game.board) / sizeof(game.board[0]);
	int colNum = sizeof(game.board[0]) / sizeof(game.board[0][0]);

	// Columb labels
	output << "   |";
	for (int col = 0; col < colNum; ++col)
		output << ' ' << char(col + 'A') << " |";
	output << "\n";

	// Draws divider at the beggining
	std::string divider = " --";
	for (int col = 0; col < colNum; ++col)
		divider += "----";
	divider += "-";

	output << divider << "\n";

	// Loops through each row
	for (int row = 0; row < rowNum; ++row) {

		// Row title
		if (int((row + 1) / 10) == 0)
			output << "  ";
		else
			output << " ";
		output << row + 1 << "|";

		for (int col = 0; col < colNum; ++col)
			output << " " << game.board[row][col] << " |";
		output << "\n";

		output << divider << "\n";
	}

	return output;
}