// Compulsory 3, Anders P. Asbo. 
#include "connect4.h"

int main() {
	vector<vector<char>> board{};
	createBoard(board, 6, 7);

	menu(board);

	return 0;
}
