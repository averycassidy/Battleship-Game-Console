#pragma once
#include <string>
#include <map>
#include "Board.h"

struct Player {
	std::string name;
	Board enemyBoard;
	Board playerBoard;
	std::map<char, int> shipHit;

	Player();
	Player(const std::string playerName);
};
