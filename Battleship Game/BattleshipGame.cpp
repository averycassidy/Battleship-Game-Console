#include <sstream>
#include <algorithm>
#include <random>
#include <stdlib.h>
#include "BattleshipGame.h"

#if defined _WIN32
	void clear() { system("cls"); }
#elif defined (__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
	void clear() { system("clear"); }
#elif defined (__APPLE__)
	void clear() { system("clear"); }
#endif


void BattleshipGame::changePlayers(bool needConfirmation) {

	// Confirm to change players from current player
	if (needConfirmation) {
		
		std::cout << "Press ENTER when ready to move to next player" << std::endl;
		std::cin.get();
		clear();
	}

	Player** temp = &(*currentPlayer);
	currentPlayer = &(*otherPlayer);
	otherPlayer = &(*temp);

	// Confirm to continue from new current player
	if (needConfirmation) {

		std::cout << "Press ENTER when " << (*currentPlayer)->name << " is ready" << std::endl;
		std::cin.get();
		clear();
	}
}

BattleshipGame::BattleshipGame() {

	player1 = new Player();
	player2 = new Player();
	currentPlayer = &player1;
	otherPlayer = &player2;
	winner = nullptr;

	// Goes through each player and has the user input a name
	std::string playerName;
	for (int player = 1; player <= 2; ++player) {

		if (currentPlayer == &player1)
			std::cout << "Enter name of PLayer 1: ";
		else
			std::cout << "Enter name of Player 2: ";
			
		std::cin >> playerName;

		(*currentPlayer)->name = playerName;
		changePlayers(false);
	}
}

BattleshipGame::BattleshipGame(const std::string player1Name, const std::string player2Name) {

	player1 = new Player(player1Name);
	player2 = new Player(player2Name);

	currentPlayer = &player1;
	otherPlayer = &player2;
	winner = nullptr;
}

void BattleshipGame::displayBoards() {

	// Get the display from the boards and convert them to a string
	std::ostringstream enemyBoardStream;
	std::ostringstream playerBoardStream;
	enemyBoardStream << (*currentPlayer)->enemyBoard;
	playerBoardStream << (*currentPlayer)->playerBoard;

	std::string enemyBoard = enemyBoardStream.str();
	std::string playerBoard = playerBoardStream.str();
	
	// Create centered board titles
	std::string enemyBoardTitle = "Your shots";
	std::string playerBoardTitle = "Your ships";
	int addSpaces = (enemyBoard.find("\n") - enemyBoardTitle.length())/ 2;
	for (int i = 0; i < addSpaces; ++i) {
		enemyBoardTitle = " " + enemyBoardTitle + " ";
		playerBoardTitle = " " + playerBoardTitle + " ";
	}

	enemyBoard = enemyBoardTitle + "\n" + enemyBoard;
	playerBoard = playerBoardTitle + "\n" + playerBoard;

	// Split the boards at their new lines and join them side by side
	std::string horizontalBoard;
	int nextLine;
	int lastPos = 0;
	do {
		nextLine = enemyBoard.find("\n");
		horizontalBoard += enemyBoard.substr(lastPos, nextLine - lastPos);
		enemyBoard = enemyBoard.substr(0, nextLine) + enemyBoard.substr(nextLine + 1);
		
		if(nextLine != std::string::npos)
			horizontalBoard += "  |&| ";

		horizontalBoard += playerBoard.substr(lastPos, nextLine - lastPos);
		playerBoard = playerBoard.substr(0, nextLine) + playerBoard.substr(nextLine + 1);

		lastPos = nextLine;
		horizontalBoard += "\n";
	} while (nextLine != std::string::npos);

	// Create centered title
	std::string title = (*currentPlayer)->name + "'s Board";
	addSpaces = (horizontalBoard.find("\n") - title.length()) / 2;
	for (int i = 0; i < addSpaces; ++i)
		title = " " + title + " ";

	std::cout << std::endl << title << std::endl;
	std::cout << horizontalBoard << std::endl;
}

bool BattleshipGame::placeShip(Location start, Location end, Ship ship) {

	// Start or end locations not within range
	if (!start.withinRange()) {
		std::cout << "Starting location not within board bounds (" << start.col << start.row << std::endl;
		return false;
	}

	if (!start.withinRange()) {
		std::cout << "Ending location not within board bounds (" << start.col << start.row << std::endl;
		return false;
	}

	// Diagonal ship placement
	if (start.row != end.row && start.col != end.col) {
		std::cout << "Start and end locations must be on either the same row or the same column." << std::endl;
		return false;
	}

	// Horizontal placement
	if (start.row == end.row) {
		if (std::max(start.col, end.col) - ship.length + 1 != std::min(start.col, end.col)) {
			std::cout << "Locations are not the correct length apart" << std::endl;
			return false;
		}
		
		// Check if ship collides with existing ship
		Location loc;
		loc.row = start.row;
		for (int i = 0; i < ship.length; ++i) {
			loc.col = char(std::min(start.col, end.col) + i);
			if (!(*currentPlayer)->playerBoard.empty(loc)) {
				std::cout << "Ship collides with existing ship at location " << loc.col << loc.row << std::endl;
				return false;
			}
		}

		// Write to board
		loc.row = start.row;
		for (int i = 0; i < ship.length; ++i) {
			loc.col = char(std::min(start.col, end.col) + i);
			(*currentPlayer)->playerBoard.set(loc, ship.symbol);
		}
		return true;
	
	// Vertical placement
	} else {
	
		if (std::max(start.row, end.row) - ship.length + 1 != std::min(start.row, end.row)) {
			std::cout << "Locations are not the correct length apart" << std::endl;
			return false;
		}

		// Check if ship collides with existing ship
		Location loc;
		loc.col = start.col;
		for (int i = 0; i < ship.length; ++i) {
			loc.row = std::min(start.row, end.row) + i;
			if (!(*currentPlayer)->playerBoard.empty(loc)) {
				std::cout << "Ship collides with existing ship at location " << loc.col << loc.row << std::endl;
				return false;
			}
		}

		// Write to board
		loc.col = start.col;
		for (int i = 0; i < ship.length; ++i) {
			loc.row = std::min(start.row, end.row) + i;
			(*currentPlayer)->playerBoard.set(loc, ship.symbol);
		}
		return true;
	}
}

bool BattleshipGame::shootAt(Location target) {

	if (!target.withinRange()) {
		std::cout << "Target was not within range!" << std::endl;
		return false;
	}

	if (!(*currentPlayer)->enemyBoard.empty(target)) {
		std::cout << "Target has already been hit" << std::endl;
		return false;
	}

	char symbol = (*otherPlayer)->playerBoard.empty(target) ? 'O' : 'X';

	if (symbol == 'X') {
		std::cout << "Hit (" << target.col << target.row << ")" << std::endl;

		// Handle sinking ships
		char shipHit = (*otherPlayer)->playerBoard.get(target);
		(*otherPlayer)->shipHit[shipHit]++;

		if ((*otherPlayer)->shipHit[shipHit] >= ships.at(shipHit).length)
			std::cout << "You just sunk " << (*otherPlayer)->name << "'s " << ships.at(shipHit).name << "!" << std::endl;

	} else
		std::cout << "Missed (" << target.col << target.row << ")" << std::endl;

	(*currentPlayer)->enemyBoard.set(target, symbol);
	(*otherPlayer)->playerBoard.set(target, symbol);

	return true;
}

void BattleshipGame::play() {

	if (winner != nullptr)
		throw "Game cannot be played a second time, winner already declaired";

	// Place ships
	for (int player = 1; player <= 2; ++player) {

		// Type of selection, either manual or random
		std::string response;
		do {
			std::cout << "Do you want to manually place ships or randomly place ships? (MANUAL/RANDOM): ";
			std::cin >> response;
			std::cin.get();

			if (response != "MANUAL" && response != "RANDOM")
				std::cout << "Input must be either MANUAL or RANDOM" << std::endl;

		} while (response != "MANUAL" && response != "RANDOM");

		// Ship placement
		
			
		if (response == "MANUAL") {
				
			for (const auto& kv : ships) {

				Ship currentShip = kv.second;

				Location startLocation;
				Location endLocation;

				clear();

				displayBoards();

				do {
					std::cout << "Currently placing your " << currentShip.name << ". Length = " << currentShip.length << std::endl;

					// Start location
					do {
						startLocation = Location::getInputLocation("Starting location");

					} while (!startLocation.withinRange());

					// End location
					do {
						endLocation = Location::getInputLocation("Ending location");

					} while (!endLocation.withinRange());

				} while (!placeShip(startLocation, endLocation, currentShip));
			}
		}
		else if (response == "RANDOM") {

			// Random engine generator
			static std::random_device entropySource;
			static std::mt19937 randomGenerator(entropySource());
			std::uniform_int_distribution<int> randomColRow(1, 10);
			std::uniform_int_distribution<int> randomDirection(0, 3);

			enum class Direction {
				NORTH, SOUTH, EAST, WEST
			};

			for (const auto& kv : ships) {

				Ship currentShip = kv.second;

				Location startLocation;
				Location endLocation;

				clear();

				bool canPlace;
				Direction direction;
				do {
					canPlace = true;

					// Random start location and random direction
					startLocation.col = char(randomColRow(randomGenerator) + 'A');
					startLocation.row = randomColRow(randomGenerator);

					direction = Direction(randomDirection(randomGenerator));

					// Based on the direction, traverse down the ship checking for out-of-bounds errors and collisions with other ships
					switch (direction) {
					case Direction::NORTH:
						endLocation.col = startLocation.col;
						for (int i = 0; i < currentShip.length; ++i) {
							endLocation.row = startLocation.row - i;
							if (!endLocation.withinRange() || !(*currentPlayer)->playerBoard.empty(endLocation))
								canPlace = false;
						}
						break;
					case Direction::SOUTH:
						endLocation.col = startLocation.col;
						for (int i = 0; i < currentShip.length; ++i) {
							endLocation.row = startLocation.row + i;
							if (!endLocation.withinRange() || !(*currentPlayer)->playerBoard.empty(endLocation))
								canPlace = false;
						}
						break;
					case Direction::EAST:
						endLocation.row = startLocation.row;
						for (int i = 0; i < currentShip.length; ++i) {
							endLocation.col = char(startLocation.col + i);
							if (!endLocation.withinRange() || !(*currentPlayer)->playerBoard.empty(endLocation))
								canPlace = false;
						}
						break;
					case Direction::WEST:
						endLocation.row = startLocation.row;
						for (int i = 0; i < currentShip.length; ++i) {
							endLocation.col = char(startLocation.col - i);
							if (!endLocation.withinRange() || !(*currentPlayer)->playerBoard.empty(endLocation))
								canPlace = false;
						}
						break;
					}

				} while (!canPlace);

				placeShip(startLocation, endLocation, currentShip);
			}
		}

		changePlayers(true);
	}

	clear();

	// Loops until a winner is declaired
	Location* previousShot = nullptr;
	while (winner == nullptr) {

		displayBoards();

		if (previousShot != nullptr)
			std::wcout << "Previous player fired at " << previousShot->col << previousShot->row << std::endl;
		
		// Get firing target
		std::cout << "Choose a location to fire at" << std::endl;
		Location target;
		do {
			target = Location::getInputLocation("Select target");

		} while (!shootAt(target));

		// Declairs winner if applicable
		if (!(*otherPlayer)->playerBoard.has_ships())
			winner = &(*currentPlayer);
		else {
			std::cout << "Press ENTER when ready to move to next player" << std::endl;
			std::cin.get();

			changePlayers(true);
		}

		previousShot = &target;
		clear();
	}
	delete(previousShot);

	std::cout << "The winner is " << (*currentPlayer)->name << "!" << std::endl;
	std::cout << std::endl;
}

BattleshipGame::~BattleshipGame() {

	delete(player1);
	delete(player2);
}