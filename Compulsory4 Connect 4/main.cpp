// Compulsory 3, Anders P. Asbo. 
#include "connect4.h"  // main header for program.

int main() {
	// create and populate game board:
	vector<vector<char>> board{};
	createBoard(board, 6, 7);

	menu(board);  // display main-menu.

	return 0;
}
