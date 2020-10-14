#pragma once
#include "Location.h"

class Board {

	char board[10][10];

public:
	Board();

	bool empty(Location loc);
	void set(Location loc, char type);
	char get(Location loc);
	bool has_ships();

	// Overloaded operators
	friend std::ostream& operator<<(std::ostream& output, const Board& game);
};