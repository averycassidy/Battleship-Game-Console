#pragma once
#include <string>


struct Ship {
	std::string name;
	unsigned char symbol;
	unsigned short length;

	Ship() = delete;
	Ship(std::string name, unsigned char symbol, unsigned short length);

};
