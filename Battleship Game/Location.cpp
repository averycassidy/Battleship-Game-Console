#include "Location.h"
#include <iostream>

int Location::rowIndex() {
	return row - 1;
}

int Location::colIndex() {
	return col - 'A';
}

bool Location::withinRange() {

	bool rowLegal = rowIndex() >= 0 && rowIndex() < 10;
	bool colLegal = colIndex() >= 0 && colIndex() < 10;

	return rowLegal && colLegal;
}

Location Location::getInputLocation(std::string message) {

	Location loc;
	bool illegalInput;
	do {
		illegalInput = false;

		// Get user input
		std::string response;
		std::cout << message << " (CharInt, A3 as example): ";
		std::cin >> response;
		std::cin.get();
		
		// Check length
		if (response.length() > 3) {
			std::cout << "ERROR IN INPUT: Input length > 3" << std::endl;
			illegalInput = true;
			continue;
		}

		if (response.length() < 2) {
			std::cout << "ERROR IN INPUT: Input length < 2" << std::endl;
			illegalInput = true;
			continue;
		}

		// Check first character for range
		char col = '~';
		if ((response.front() >= 'A' && response.front() <= 'J') || (response.front() >= 'a' && response.front() <= 'j')) {
			col = response.front();

			// Capitalizes lowercase letters
			if (col >= 'a' && col <= 'j')
				col += 'A' - 'a';
		} else {
			std::cout << "Location must be a single character A-J and a single number 1-10" << std::endl;
			illegalInput = true;
			continue;
		}

		// Check number
		int row = -1;
		try {
			row = std::stoi(response.substr(1, response.length()), nullptr, 10);

			loc.col = col;
			loc.row = row;

		} catch (std::invalid_argument) {
			std::cout << "Location must be a single character A-J and a single number 1-10" << std::endl;
			illegalInput = true;
			continue;
		}

	} while (illegalInput);

	std::cout << "Location " << loc.col << loc.row << " accepted" << std::endl;
	return loc;
}
