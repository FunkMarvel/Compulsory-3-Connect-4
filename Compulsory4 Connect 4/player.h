#pragma once
// Compulsory 3, Anders P. Asbo.
// Top-level header. Contains Player class and related functions.

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

// common std-functions:
using std::cout; using std::cin; using std::string;
using std::endl; using std::vector;

class Player {  // class for player storing name, character for game piece, total score and scor of current session.
public:
	// constructors:
	Player() { name = "A I"; mark = 'x'; }  // Default constructor makes AI player.
	Player(string _name, char _mark) { name = _name; mark = _mark; }  // Construct with provided name and mark.
	Player(string _name, int _score) { name = _name; old_score = _score; }  // Construct with provided name and high-score.

	// destructor:
	~Player() {}

	// functions for retrieving and setting name:
	inline string getName() { return name; }
	inline void setName(string _name) { name = _name; }

	// functions for retrieving and setting game-piece character:
	inline char getMark() { return mark; }
	inline void setMark(char _mark) { mark = _mark; }

	// functions for retrieving and incrementing score:
	inline unsigned int getScore() { return current_score; }
	inline unsigned int getHighScore() { return old_score + current_score; }
	inline void incrementScore() { current_score++; }

	// functions for saving and loading name and score to and from file:
	void save(string filename);
	void load(string filename);

	// operator overloads:
	friend std::ostream& operator<<(std::ostream& os, Player player);  // insertion operator used when saving to file
	friend std::ostream& operator<<(std::ostream& os, std::unique_ptr<Player>& player);  // insertion operator used when saving to file
	friend bool operator<(Player player1, Player player2) { return player1.getHighScore() < player2.getHighScore(); }  // < operator allows using std::sort on vector<Player>() object.

	inline const static string getFilename() { return "savefile.txt"; }  // static member for retrieving default filename.

private:
	// private variables:
	char mark;  // game-piece
	string name;
	unsigned int current_score{};  // session score.
	unsigned int old_score{};  // score loaded from file.

	// function calculating new total score for saving to file:
	inline void calcHighScore() { old_score += current_score; }

};

// related stand-alone functions.
void createPlayer(int, std::unique_ptr<Player>&);
vector<Player> loadPlayers(string filename);