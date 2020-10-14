#pragma once
#include <string>

struct Location {
	char col;
	int row;

	int colIndex();
	int rowIndex();
	bool withinRange();
	static Location getInputLocation(std::string message);
};