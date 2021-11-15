// Compulsory 3, Anders P. Asbo.
// Contains Player class methods and related functions.

#include "player.h"  // includes player decleration.
#include <fstream>

void Player::save(string filename) {
	// Method for saving player-name and score to file.
	//
	// Args:
	//	filename -- string containing filename of file.

	bool is_saved{ false }, found_line{ false };  // bools for file parsing.

	// opens file for both reading and writing:
	std::fstream ofile(filename, std::ios::in | std::ios::out | std::ios::ate);
	string line{};
	int start{}, middle{}, line_pos{};  // for finding current player's name in file.
	vector<string> lines{};  // vector for storing all lines read from file.
	calcHighScore();  // updates total score.
	
	
	if (ofile.is_open()) { // saves data to end of file if file is open.
		ofile.seekg(0);  // makes sure to read from top of file.

		while (getline(ofile, line)) { // loops through file and finds names:
			start = line.find('{');
			middle = line.find(':');
			lines.push_back(line);  // saves current line in vector.

			// if name in current line matches name of current player, save line number:
			if (line.size() != 0 && line.substr(start + 1, middle - start - 1) == name) {
				line_pos = lines.size() - 1;
				found_line = true;
			}
		}
		ofile.clear();  // clears flags.
		ofile.seekg(0);  // moves to top again.

		for (size_t i = 0; i < lines.size(); i++) {
			if (i != line_pos || !found_line) {  // writes stored lines back to file in same order:
				ofile << lines[i] << endl;
			}
			else {  // if line with current player is reached, write new score:
				ofile << *this << endl;
				is_saved = true;
			}
		}

		if (!is_saved) ofile << *this << endl;  // if current player was not written to file, append current player to bottom of file.
		ofile.close();
	}
	else {
		// exits on failure to open savefile:
		cout << "Save failed." << endl;
		exit(1);
	}
}

void Player::load(string filename) {
	// Method for loading player score from file.
	//
	// Args:
	//	filename -- string containing filename of file.

	std::ifstream ifile(filename);

	if (!(ifile.is_open())) { // creates new savefile if opening failed:
		std::fstream ofile(filename, std::ios::out);
		ofile.close();
		ifile = std::ifstream(filename);
	}

	if (ifile.is_open()) {  // seeks through file:
		string line{};
		int start{}, middle{}, end{};
		while (getline(ifile, line)) {
			start = line.find('{');
			middle = line.find(':');
			end = line.find('}');

			// if current entry in file has same name as current player, load score:
			if (line.size() != 0 && line.substr(start + 1, middle - start - 1) == name) {
				old_score = stoi(line.substr(middle + 1, end - middle - 1));
				break;
			}
		}
		ifile.close();  // old_score remains 0 if no saved score is found.
	}
	else {
		// exits on failure to open savefile:
		cout << "Load failed." << endl;
		exit(1);
	}
}

std::ostream& operator<<(std::ostream& os, Player player) {
	// insertion operator overload. Returns refrence to ostream after inserting string {name:score}
	//
	// Args:
	//		os -- ostream refrence to insert into.
	//		player -- Player-object to retrieve name and score from.

	os << "{" << player.name << ":" << player.old_score << "}";
	return os;
}


std::ostream& operator<<(std::ostream& os, std::unique_ptr<Player>& player) {
	// insertion operator overload. Returns refrence to ostream after inserting string {name:score}
	//
	// Args:
	//		os -- ostream refrence to insert into.
	//		player -- Player-object to retrieve name and score from.

	os << *player;  // uses previous overload.
	return os;
}

void createPlayer(int player_number, std::unique_ptr<Player>& player) {
	// Function creating player from user-provided name. Player object is stored in
	// memory-location pointed too by player-argument.
	//
	// Args:
	//		player_number -- int corresponding to wether creating player 1 or 2.
	//		player -- unique_ptr that points to memory-location where created player will be stored.

	string name{};
	char mark{'x'}; // sets correct game-piece:
	if (player_number % 2) mark = 'o';

	// asks for name, no spaces allowed:
	cout << " Enter player " << player_number << " name (no spaces): ";
	cin >> name;
	player = std::make_unique<Player>(Player(name, mark));  // stores new player object.
}

vector<Player> loadPlayers(string filename) {
	// Function for loading all players from file.
	//
	// Args:
	//	filename -- string containing filename of file.

	vector<Player> players{};
	std::ifstream ifile(filename);

	if (ifile.is_open()) {
		string line{};
		int start{}, middle{}, end{};

		// loads name and score from each line:
		while (getline(ifile, line)) {
			string name{};
			int score{};

			start = line.find('{');
			middle = line.find(':');
			end = line.find('}');

			if (line.size() != 0) {
				name = line.substr(start + 1, middle - start - 1);
				score = stoi(line.substr(middle + 1, end - middle - 1));

				players.push_back(Player(name, score));  // saves player to return-vector.
			}
		}
		ifile.close();

		int ai_index{ -1 };  // finds AI entry in return-vector and erases it.
		for (size_t i = 0; i < players.size(); i++) {
			if (players[i].getName() == "A I") ai_index = i;  // Only AI has space in name, since users can't input name with spaces.
		}
		if (ai_index >= 0) players.erase(players.begin() + ai_index);

		// uses operator< overload and std::sort to sort players in return-vector from lowest score to highest core.
		std::sort(players.begin(), players.end());
		return players;
	}
	else {
		// exits on failure to open savefile:
		cout << "Load failed." << endl;
		return players;
	}
}
