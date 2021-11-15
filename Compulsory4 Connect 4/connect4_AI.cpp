// Compulsory 3, Anders P. Asbo.
// Source file for Ai logic.
#include "connect4.h"  // includes all necesary functions.

double difficultySelect() {
	// Function for selecting difficulty of AI. Returns a double corresponding to threshold for
	// wether ai plays according to logic or randomly on each of its turn.
	//

	vector<string> options{ "Easy", "Medium", "Hard", "Lunatic" };
	vector<double> weights{ 0.5, 0.8, 0.95, 1.1 };  // Higher threshold makes AI play less randomly.
													// Lunatic difficulty makes ai play only according to its logic.
													// Uncertain if this actually makes it harder.
	bool selection{ false };
	char input{};
	int position{};

	while (true) {

		system("cls");  // displays difficulty menu:
		cout << termcolor::bright_blue << "Select difficulty:" << termcolor::reset << endl;
		for (int i = 0; i < options.size(); i++) {
			if (i == position) {
				cout << termcolor::green << " ";
			}
			cout << " | " << i + 1 << ": " << options[i] << termcolor::reset << endl;
		}
		cout << endl << "Navigate with 'w' and 's'. Press 'enter' to select." << endl;

		input = _getch();

		switch (tolower(input)) {  // changes menu position according to input.
		case 'w':
			position--;
			break;
		case 's':
			position++;
			break;
		case '\r':  // selects current menu item on press of enter key.
			system("cls");
			selection = true;
			break;
		default:
			break;
		}
		if (selection) return weights[position];  // returns selected weight.

		// loops position around if out of menu range:
		if (position < 0) { position = options.size() - 1; }
		else if (position >= options.size()) { position = 0; }

	}
}

void aiSelection(vector<vector<char>>& board, std::unique_ptr<Player>& player1, std::unique_ptr<Player>& player2, double weight) {
	// Function that selects and places AI piece.
	// 
	// Args:
	//		board -- 2D char-vector passed by reference, contains game-board.
	//		player1 -- unique pointer to Player object, representing player 1.
	//		player2 -- unique pointer to Player object, representing player 2.
	//		weight -- double containing threshold for random behaviour.
	//

	vector<vector<int>> possible_moves{};

	// makes copy of game board to test moves on:
	vector<vector<char>> temp_board{};
	for (size_t i = 0; i < board.size(); i++) {
		temp_board.push_back(board[i]);
	}

	int selection{};
	int possible_win{ -1 };
	int possible_loss{ -1 };

	// finds the indices of possible moves:
	for (int j = 0; j < temp_board[0].size(); j++) {
		if (temp_board[0][j] != ' ') continue;
		for (int i = 1; i < temp_board.size(); i++) {
			if (temp_board[i][j] != ' ') {
				possible_moves.push_back({ i - 1, j });  // saves possible moves in 2D vector
				break;
			}
			else if (i == temp_board.size() - 1) {
				possible_moves.push_back({ i , j });  // saves possible moves in 2D vector
			}
		}
	}

	// Mersenne-Twister 64-bit for RNG:
	std::random_device rd{};
	std::mt19937_64 gen(rd());
	std::uniform_real_distribution<double> RNG_double(0, 1);
	std::uniform_int_distribution<int> RNG_int(0, possible_moves.size() - 1);
	double result{ RNG_double(gen) };  // number to compare against weight.

	vector<vector<int>>* winning_positions = new vector<vector<int>>();  // vector for checkWin().
																		 // Does nothing, but couldn't be botherd to make optional in chekWin().

	selection = possible_moves[RNG_int(gen)][1];  // initial selection is random.

	for (vector<int>& position : possible_moves) { // loops through possible moves, position is vector with form { row, column }.

		// checks for possibe win and saves it.
		temp_board[position[0]][position[1]] = player2->getMark();
		if (checkWin(temp_board, winning_positions)) {
			possible_win = position[1];
		}  // checks for possible 3 in a row:
		else if (checkWin(temp_board, winning_positions, 2)) {
			selection = position[1];
		}

		// checks for possible loss:
		temp_board[position[0]][position[1]] = player1->getMark();
		if (checkWin(temp_board, winning_positions)) {
			possible_loss = position[1];
		}  // checks for enemy 3 in a row:
		else if (checkWin(temp_board, winning_positions, 2)) {
			selection = position[1];
		}

		temp_board[position[0]][position[1]] = ' ';  // resets board copy.
	}
	delete winning_positions;  // delets winning_positions when it's no longer needed.

	if (result >= weight) selection = possible_moves[RNG_int(gen)][1];  // selects random position if result lower than weight.
	if (possible_win >= 0) { selection = possible_win; }  // else picks win if found.
	else if (possible_loss >= 0) { selection = possible_loss; }  // else prevents possible loss if found.

	// animates placement of piece:
	vector<char> selection_bar(board[0].size(), ' ');
	selection_bar[selection] = player2->getMark();

	for (int i = 0; i < board.size(); i++) {
		if (board[i][selection] == ' ') {
			board[i][selection] = player2->getMark();
			if (i != 0) board[i - 1][selection] = ' ';
			system("cls");
			drawBar(selection_bar, selection, player2->getMark());
			drawBoard(board);
			Sleep(100);
		}
	}
}