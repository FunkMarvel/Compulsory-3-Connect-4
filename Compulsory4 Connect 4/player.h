#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <conio.h>
#include <cmath>

// using termcolor by Ihor Kalnytskyi for coloring text in terminal:
#include "termcolor.hpp"  // https://github.com/ikalnytskyi/termcolor.git
// thanks to Marcus Nesvik Henriksen for bringing this library to my attention.

using std::cout; using std::cin; using std::string;
using std::endl; using std::vector;

class Player {
public:
	Player() { name = "AI"; mark = 'x'; }
	Player(string _name, char _mark) { name = _name; mark = _mark; }
	~Player() {}

	inline string getName() { return name; }
	inline void setName(string _name) { name = _name; }

	inline char getMark() { return mark; }
	inline void setMark(char _mark) { mark = _mark; }

	inline unsigned int getScore() { return score; }
	inline void incrementScore() { score++; }

	void save(string filename);

	friend std::ostream& operator<<(std::ostream& os, Player player);
	friend std::ostream& operator<<(std::ostream& os, Player* player);

private:
	char mark;
	string name;
	unsigned int score{};

};

