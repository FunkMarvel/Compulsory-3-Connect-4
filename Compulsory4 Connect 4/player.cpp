#include "player.h"
#include <fstream>

void Player::save(string filename) {
	// Method for saving player-name and score to file.
	//
	// Args:
	//	filename -- string containing filename of file.

	std::ofstream ofile(filename, std::ios::app);
	if (ofile.is_open()) { // saves data to end of file if file is open.
		ofile << this << std::endl;
		ofile.close();
	}
	else {
		// exits on failure to open savefile:
		std::cout << "Save failed." << std::endl;
		exit(1);
	}
}

std::ostream& operator<<(std::ostream& os, Player player) {
	os << "{" << player.name << ":" << player.score << "}" << endl;
	return os;
}

std::ostream& operator<<(std::ostream& os, Player* player) {
	os << *player;
	return os;
}
