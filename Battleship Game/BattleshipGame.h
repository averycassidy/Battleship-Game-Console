#pragma once
#include <iostream>
#include "Player.h"
#include "Ship.h"

class BattleshipGame {

	Player* player1;
	Player* player2;

	Player** currentPlayer;
	Player** otherPlayer;
	Player** winner;

	const std::map<char, Ship> ships = {
	   {'C', Ship("Carrier", 'C', 5)},
	   {'D', Ship("Destroyer", 'D', 4)},
	   {'B', Ship("Battleship", 'B', 3)},
	   {'S', Ship("Submarine", 'S', 3)},
	   {'P', Ship("Patrol Boat", 'P', 2)}
	};

	void displayBoards();
	bool placeShip(Location start, Location end, Ship ship);
	bool shootAt(Location loc);
	void changePlayers(bool needConfirmation);

public:

	BattleshipGame();
	BattleshipGame(const std::string player1Name, const std::string player2Name);
	void play();
	~BattleshipGame();

};

