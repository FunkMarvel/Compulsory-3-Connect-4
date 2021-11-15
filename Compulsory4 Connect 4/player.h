#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using std::cout; using std::cin; using std::string;
using std::endl; using std::vector;

class Player {
public:
	Player() { name = "AI"; mark = 'x'; }
	Player(string _name, char _mark) { name = _name; mark = _mark; }
	Player(string _name, int _score) { name = _name; old_score = _score; }
	~Player() {}

	inline string getName() { return name; }
	inline void setName(string _name) { name = _name; }

	inline char getMark() { return mark; }
	inline void setMark(char _mark) { mark = _mark; }

	inline unsigned int getScore() { return current_score; }
	inline unsigned int getHighScore() { return old_score + current_score; }
	inline void incrementScore() { current_score++; }

	void save(string filename);
	void load(string filename);

	friend std::ostream& operator<<(std::ostream& os, Player player);
	friend std::ostream& operator<<(std::ostream& os, std::unique_ptr<Player>& player);
	friend bool operator<(Player player1, Player player2) { return player1.getHighScore() < player2.getHighScore(); }

	inline const static string getFilename() { return "savefile.txt"; }

private:
	char mark;
	string name;
	unsigned int current_score{};
	unsigned int old_score{};

	inline void calcHighScore() { old_score += current_score; }

};

void createPlayer(int, std::unique_ptr<Player>&);
vector<Player> loadPlayers(string filename);