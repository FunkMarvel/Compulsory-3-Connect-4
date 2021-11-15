#include "player.h"
#include <fstream>

void Player::save(string filename) {
	// Method for saving player-name and score to file.
	//
	// Args:
	//	filename -- string containing filename of file.

	bool is_saved{ false }, found_line{ false };
	std::fstream ofile(filename, std::ios::in | std::ios::out | std::ios::ate);
	string line{};
	int start{}, middle{}, line_pos{};
	vector<string> lines{};
	calcHighScore();
	
	
	if (ofile.is_open()) { // saves data to end of file if file is open.
		ofile.seekg(0);
		while (getline(ofile, line)) {
			start = line.find('{');
			middle = line.find(':');

			lines.push_back(line);
			if (line.size() != 0 && line.substr(start + 1, middle - start - 1) == name) {
				line_pos = lines.size() - 1;
				found_line = true;
			}
		}
		ofile.clear();
		ofile.seekg(0);
		for (size_t i = 0; i < lines.size(); i++) {
			if (i != line_pos || !found_line) {
				ofile << lines[i] << endl;
			}
			else {
				ofile << *this << endl;
				is_saved = true;
			}
		}

		if (!is_saved) ofile << *this << endl;
		ofile.close();
	}
	else {
		// exits on failure to open savefile:
		cout << "Save failed." << endl;
		exit(1);
	}
}

void Player::load(string filename) {
	std::ifstream ifile(filename);

	if (!(ifile.is_open())) {
		std::fstream ofile(filename, std::ios::out);
		ofile.close();
		ifile = std::ifstream(filename);
	}

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
		cout << "Load failed." << endl;
		exit(1);
	}
}

std::ostream& operator<<(std::ostream& os, Player player) {
	os << "{" << player.name << ":" << player.old_score << "}";
	return os;
}


std::ostream& operator<<(std::ostream& os, std::unique_ptr<Player>& player) {
	os << *player;
	return os;
}

void createPlayer(int player_number, std::unique_ptr<Player>& player) {
	string name{};
	char mark{'x'};
	if (player_number % 2) mark = 'o';

	cout << " Enter player " << player_number << " name (no spaces): ";
	cin >> name;
	player = std::make_unique<Player>(Player(name, mark));
}

vector<Player> loadPlayers(string filename) {
	vector<Player> players{};
	std::ifstream ifile(filename);

	if (ifile.is_open()) {
		string line{};
		int start{}, middle{}, end{};

		while (getline(ifile, line)) {
			string name{};
			int score{};

			start = line.find('{');
			middle = line.find(':');
			end = line.find('}');

			if (line.size() != 0) {
				name = line.substr(start + 1, middle - start - 1);
				score = stoi(line.substr(middle + 1, end - middle - 1));

				players.push_back(Player(name, score));
			}
		}
		ifile.close();

		int ai_index{ -1 };
		for (size_t i = 0; i < players.size(); i++) {
			if (players[i].getName() == "A I") ai_index = i;
		}

		if (ai_index >= 0) players.erase(players.begin() + ai_index);
		std::sort(players.begin(), players.end());
		return players;
	}
	else {
		// exits on failure to open savefile:
		cout << "Load failed." << endl;
		return players;
	}
}
