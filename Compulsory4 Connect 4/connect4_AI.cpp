#include "connect4.h"

double difficultySelect() {
	vector<string> options{ "Easy", "Medium", "Hard", "Lunatic" };
	vector<double> weights{ 0.5, 0.9, 0.99, 1.1 };
	bool selection{ false };
	char input{};
	int position{};

	while (true) {

		system("cls");
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
		if (selection) return weights[position];
		// loops position around if out of menu range.
		if (position < 0) { position = options.size() - 1; }
		else if (position >= options.size()) { position = 0; }

	}
	return 0.0;
}

void aiSelection(vector<vector<char>>& board, std::unique_ptr<Player>& player1, std::unique_ptr<Player>& player2, double weight) {
	vector<vector<int>> possible_moves{};
	vector<vector<char>> temp_board{};

	for (size_t i = 0; i < board.size(); i++) {
		temp_board.push_back(board[i]);
	}

	int selection{};
	int possible_win{ -1 };
	int possible_loss{ -1 };

	for (int j = 0; j < temp_board[0].size(); j++) {
		if (temp_board[0][j] != ' ') continue;
		for (int i = 1; i < temp_board.size(); i++) {
			if (temp_board[i][j] != ' ') {
				possible_moves.push_back({ i - 1, j });
				break;
			}
			else if (i == temp_board.size() - 1) {
				possible_moves.push_back({ i , j });
			}
		}
	}

	std::random_device rd{};
	std::mt19937_64 gen(rd());
	std::uniform_real_distribution<double> RNG_double(0, 1);
	std::uniform_int_distribution<int> RNG_int(0, possible_moves.size() - 1);
	double result{ RNG_double(gen) };

	vector<vector<int>>* winning_positions = new vector<vector<int>>();

	selection = possible_moves[RNG_int(gen)][1];
	for (vector<int>& position : possible_moves) {

		temp_board[position[0]][position[1]] = player2->getMark();
		if (checkWin(temp_board, winning_positions)) {
			possible_win = position[1];
			break;
		}
		else if (checkWin(temp_board, winning_positions, 2)) {
			selection = position[1];
		}

		temp_board[position[0]][position[1]] = player1->getMark();
		if (checkWin(temp_board, winning_positions)) {
			possible_loss = position[1];
		}
		else if (checkWin(temp_board, winning_positions, 2)) {
			selection = position[1];
		}

		temp_board[position[0]][position[1]] = ' ';
	}
	delete winning_positions;

	if (result >= weight) selection = possible_moves[RNG_int(gen)][1];
	if (possible_win >= 0) { selection = possible_win; }
	else if (possible_loss >= 0) { selection = possible_loss; }

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