#include "player.h"
#include <fstream>

void Player::save(string filename) {
	// Method for saving player-name and score to file.
	//
	// Args:
	//	filename -- string containing filename of file.

	std::fstream ofile(filename, std::ios::in | std::ios::out | std::ios::ate);
	string line{};
	int start{}, middle{}, line_pos{};
	vector<string> lines{};
	calcHighScore();
	
	if (ofile.is_open()) { // saves data to end of file if file is open.
		while (getline(ofile, line)) {
			lines.push_back(line);
			if (line.size() != 0 && line.substr(start + 1, middle - start - 1) == name) {
				line_pos = lines.size() - 1;
			}
		}
		ofile.seekg(0);
		for (size_t i = 0; i < lines.size(); i++) {
			if (i != line_pos) {
				ofile << lines[i] << endl;
			}
			else {
				ofile << this << std::endl;
			}
		}
		ofile.close();
	}
	else {
		// exits on failure to open savefile:
		std::cout << "Save failed." << std::endl;
		exit(1);
	}
}

void Player::load(string filename) {
	std::ifstream ifile(filename);
	if (ifile.is_open()) {
		string line{};
		int start{}, middle{}, end{};
		while (getline(ifile, line)) {
			start = line.find('{');
			middle = line.find(':');
			end = line.find('}');

			if (line.size() != 0 && line.substr(start + 1, middle - start - 1) == name) {
				old_score = stoi(line.substr(middle + 1, end - middle - 1));
				break;
			}
		}
		ifile.close();
	}
	else {
		// exits on failure to open savefile:
		std::cout << "Load failed." << std::endl;
		exit(1);
	}
}

std::ostream& operator<<(std::ostream& os, Player player) {
	os << "{" << player.name << ":" << player.old_score << "}" << endl;
	return os;
}


std::ostream& operator<<(std::ostream& os, Player* player) {
	os << *player;
	return os;
}

Player* createPlayer(int player_number) {
	Player* player{};
	string name{};
	char mark{'x'};
	if (player_number % 2) mark = 'o';

	cout << " Enter player " << player_number << " name (no spaces): ";
	cin >> name;
	player = new Player(name, mark);

	return player;
}