// Compulsory 3, Anders P. Asbo. 
#include <iostream>
#include <string>
#include <vector>
#include <conio.h>

// using termcolor by Ihor Kalnytskyi for coloring text in terminal:
#include "termcolor.hpp"  // https://github.com/ikalnytskyi/termcolor.git
// thanks to Marcus Nesvik Henriksen for bringing this library to my attention.

using std::cout; using std::cin; using std::string;
using std::endl; using std::vector;

void menu(vector<vector<char>>& board);
void gamePlayLoop(vector<vector<char>>& board);
void startTask(int, vector<vector<char>>& board);
void createBoard(vector<vector<char>>& board, int num_rows, int num_cols);
void drawBoard(vector<vector<char>>& board);
void drawBar(vector<char>& selection_bar, int position, char player, bool prompt=true);
void selectCol(vector<vector<char>>&, char player);

int main() {
	vector<vector<char>> board{};
	createBoard(board, 6, 7);

	menu(board);

	return 0;
}

void startTask(int position, vector<vector<char>>& board) {
	switch (position) {
	case 0:
		gamePlayLoop(board);
		break;
	case 1:
		break;
	case 2:
		exit(0);
	default:
		break;
	}
}

void createBoard(vector<vector<char>>& board, int num_rows, int num_cols) {
	for (size_t i = 0; i < num_rows; i++) {
		board.push_back(vector<char>(num_cols, ' '));
	}
}

void drawBoard(vector<vector<char>>& board) {
	for (size_t i = 0; i <= board.size(); i++) {
		cout << termcolor::bright_blue << " |";
		for (size_t j = 0; j < 4.5*board.size(); j++) cout << "-";
		cout << "| " << termcolor::reset << endl;
		if (i == board.size()) break;

		for (size_t j = 0; j < board[0].size(); j++) {
			cout << termcolor::bright_blue << " | ";
			switch (board[i][j]) {
			case ' ':
				cout << termcolor::reset;
				break;
			case 'o':
				cout << termcolor::bright_yellow;
				break;
			case 'x':
				cout << termcolor::bright_red;
			default:
				break;
			}
			cout << board[i][j];
		}
		cout << termcolor::bright_blue << " | " << termcolor::reset << endl;
	}
}

void drawBar(vector<char>& selection_bar, int position, char player, bool prompt) {
	if (prompt) cout << termcolor::reset << " Navigate with 'a' and 'd', select column with 'enter':" << endl;
	for (int i = 0; i < selection_bar.size(); i++) {
		if (i == position) { cout << termcolor::bright_green << "   v"; }
		else { cout << "   " << selection_bar[i]; }
	}
	cout << termcolor::reset << endl;

	for (int i = 0; i < selection_bar.size(); i++) {
		if (i == position && player == 'o') cout << termcolor::bright_yellow;
		if (i == position && player == 'x') cout << termcolor::bright_red;
		cout << "   " << selection_bar[i];
	}
	cout << termcolor::reset << endl;
}

void selectCol(vector<vector<char>>& board, char player) {
	char input{};
	int position{(int) board[0].size()/2};
	bool selection{ false };
	vector<char> selection_bar(board[0].size(), ' ');
	
	while (true) {
		system("cls");
		selection_bar[position] = player;
		
		drawBar(selection_bar, position, player);
		
		drawBoard(board);
		
		input = _getch();

		switch (input) {
		case 'a':
			selection_bar[position--] = ' ';
			break;
		case 'd':
			selection_bar[position++] = ' ';
			break;
		case '\r':
			selection = true;
			selection_bar[position] = ' ';
			break;
		default:
			break;
		}

		if (position < 0) { position = selection_bar.size() - 1; }
		else if (position >= selection_bar.size()) { position = 0; }

		if (selection) {
			for (int i = 0; i < board.size(); i++) {
				if (board[i][position] == ' ') {
					board[i][position] = player;
					if (i != 0) board[i - 1][position] = ' ';
					system("cls");
					drawBar(selection_bar, position, player);
					drawBoard(board);
					Sleep(100);
				}
			}
			return;
		}
	}
}

void menu(vector<vector<char>>& board) {
	int position{};
	char input{};
	vector<string> menu_options = { "Play against player.", "Play against AI.", "Quit to desktop." };

	while (true) {
		system("cls");
		cout << termcolor::bright_blue << " ~~~ Connect 4 ~~~" << termcolor::reset << endl;
		for (int i = 0; i < menu_options.size(); i++) {
			if (i == position) {
				cout << termcolor::green << " ";
			}
			cout << " | " << i + 1 << ": " << menu_options[i] << termcolor::reset << endl;
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
			startTask(position, board);
			system("pause");
			break;
		default:
			break;
		}

		// loops position around if out of menu range.
		if (position < 0) { position = menu_options.size() - 1; }
		else if (position >= menu_options.size()) { position = 0; }

	}

}

void gamePlayLoop(vector<vector<char>>& board) {
	char player{ 'o' };
	bool ai_on{ false };
	int turn{};

	while (true) {
		if (turn % 2 == 0) { player = 'x'; }
		else { player = 'o'; }

		selectCol(board, player);

		turn++;
	}
}
