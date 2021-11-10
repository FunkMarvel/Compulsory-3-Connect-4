// Compulsory 3, Anders P. Asbo. 
#include "player.h"

void menu(vector<vector<char>>& board);
void gamePlayLoop(vector<vector<char>>& board, Player* player1, Player* player2, bool ai_on=false);
void startTask(int, vector<vector<char>>& board, Player* player1, Player* player2);
void createBoard(vector<vector<char>>& board, int num_rows, int num_cols);
void clearBoard(vector<vector<char>>& board);
void drawBoard(vector<vector<char>>& board);
void drawBar(vector<char>& selection_bar, int position, char player, bool prompt=true);
int selectCol(vector<vector<char>>&, char player);
bool checkWin(vector<vector<char>>& board);
bool traverseDiag(vector<vector<char>>& board, int start_row, int start_col, int step = 1);
int aiSelection(vector<vector<char>>& board);
int minimax(vector<vector<char>>& board, int position, int depth, int alpha, int beta, bool maximising);

int main() {
	vector<vector<char>> board{};
	createBoard(board, 6, 7);

	menu(board);

	return 0;
}

void startTask(int position, vector<vector<char>>& board, Player* player1, Player* player2) {
	switch (position) {
	case 0:
		gamePlayLoop(board, player1, player2);
		clearBoard(board);
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

void clearBoard(vector<vector<char>>& board) {
	for (size_t i = 0; i < board.size(); i++) {
		for (size_t j = 0; j < board[0].size(); j++) {
			board[i][j] = ' ';
		}
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

int selectCol(vector<vector<char>>& board, char player) {
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

		switch (tolower(input)) {
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
			return position;
		}
	}
}

bool checkWin(vector<vector<char>>& board) {
	bool win{ false };
	int count{};

	for (size_t i = 0; i < board.size(); i++) {
		for (size_t j = 1; j < board[0].size(); j++) {
			if (board[i][j - 1] == board[i][j] && board[i][j] != ' ') { count++; }
			else { count = 0; }
			if (count >= 3) return true;
		}
	}

	count = 0;
	for (size_t j = 0; j < board[0].size(); j++) {
		for (size_t i = 1; i < board.size(); i++) {
			if (board[i - 1][j] == board[i][j] && board[i][j] != ' ') { count++; }
			else { count = 0; }
			if (count >= 3) return true;
		}
	}

	for (size_t i = 0; i < board.size()/2; i++) {
		if (traverseDiag(board, i, 0)) return true;
	}
	for (size_t j = 0; j < (board[0].size() + 1)/2; j++) {
		if (traverseDiag(board, 0, j)) return true;
	}
	for (size_t i = board.size()-1; i > board.size() / 2; i--) {
		if (traverseDiag(board, i, 0,-1)) return true;
	}
	for (size_t j = 1; j < (board[0].size() + 1) / 2; j++) {
		if (traverseDiag(board, board.size()-1, j,-1)) return true;
	}

	return false;
}

bool traverseDiag(vector<vector<char>>& board, int row, int col, int step) {
	int count{};

	for (size_t i = 0; i < board.size()*board[0].size(); i++) {
		if (board[row][col] == board[row + step][col + abs(step)] && board[row][col] != ' ') { count++; }
		else { count = 0; }
		if (count >= 3) return true;
		row += step; col += abs(step);
		if (row + step >= board.size() || row + step < 0) return false;
		if (col + abs(step) >= board[0].size()) return false;
	}
}

int aiSelection(vector<vector<char>>& board) {
	return 0;
}

int minimax(vector<vector<char>> &board, int position, int depth, int alpha, int beta, bool maximising) {
	if (depth == 0 /* or game over*/) {
		if (checkWin(board)) return 10000;
		return 0;
	}

	if (maximising) {
		int max_eval{ -10000 };
		int eval{};
		for (size_t i = 0; i < board.size(); i++) {
			eval = minimax(board, i/*pos*/, depth - 1, alpha, beta, false);
			max_eval = max(max_eval, eval);
			alpha = max(alpha, max_eval);
			if (beta <= alpha) break;
			return max_eval;
		}
	}
	else {
		int min_eval{ 10000 };
		int eval{};
		for (size_t i = 0; i < board.size(); i++) {
			eval = minimax(board, i/*pos*/, depth - 1, alpha, beta, false);
			min_eval = min(min_eval, eval);
			alpha = min(alpha, min_eval);
			if (beta <= alpha) break;
			return min_eval;
		}
	}
	return 0;
}

void menu(vector<vector<char>>& board) {
	int position{};
	char input{};
	vector<string> menu_options = { "Play against player.", "Play against AI.", "Quit to desktop." };

	while (true) {
		Player* player1{ nullptr }; Player* player2{ nullptr };

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
			startTask(position, board, player1, player2);
			system("pause");
			break;
		default:
			break;
		}

		// loops position around if out of menu range.
		if (position < 0) { position = menu_options.size() - 1; }
		else if (position >= menu_options.size()) { position = 0; }

		delete player1;
		delete player2;
	}

}

void gamePlayLoop(vector<vector<char>>& board, Player* player1, Player* player2, bool ai_on) {
	bool replay{ true };

	player1 = createPlayer(1);

	if (!ai_on) {
		player2 = createPlayer(2);
	}
	else {
		player2 = new Player("ai", 'x');
	}

	while (replay) {
		int turn{0};
		char mark{};
		Player* player{ nullptr };

		while (true) {
			turn++;

			if (turn % 2) { mark = player1->getMark(); }
			else { mark = player2->getMark(); }

			if (!ai_on || !(turn % 2)) { selectCol(board, mark); }
			else { aiSelection(board); }

			if (checkWin(board)) {
				if (turn % 2) { player1->incrementScore(); player = player1; }
				else { player2->incrementScore(); player = player2; }
				break;
			}
		}

		vector<string> options{ "Yes", "No" };
		int pos{};
		bool select{ false };
		while (!select) {
			char input{};
			system("cls");
			cout << termcolor::reset << "Winner is " << player->getName() << "!" << endl;
			drawBoard(board);
			cout << " Play again?" << endl;
			for (size_t i = 0; i < 2; i++) {
				if (i == pos) {
					cout << termcolor::bright_green << " >";
				}
				else {
					cout << termcolor::reset << " ";
				}
				cout << "| " << options[i] << "\t";
			}
			cout << termcolor::reset << endl;

			input = _getch();

			switch (input) {
			case 'a':
				pos--;
				break;
			case 'd':
				pos++;
				break;
			case '\r':
				clearBoard(board);
				if (pos) replay = false;
				select = true;
				break;
			default:
				break;
			}

			if (pos < 0) { pos = 0; }
			else if (pos >= 2) { pos = 1; }
		}
	}
}
