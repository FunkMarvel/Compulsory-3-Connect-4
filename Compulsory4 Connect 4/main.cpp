// Compulsory 3, Anders P. Asbo. 
#include "player.h"

void menu(vector<vector<char>>& board);
void gamePlayLoop(vector<vector<char>>& board, std::unique_ptr<Player>& player1, std::unique_ptr<Player>& player2, bool ai_on=false);
void startTask(int, vector<vector<char>>& board, std::unique_ptr<Player>& player1, std::unique_ptr<Player>& player2);
void createBoard(vector<vector<char>>& board, int num_rows, int num_cols);
void clearBoard(vector<vector<char>>& board);
void drawBoard(vector<vector<char>>& board, vector<vector<int>>* winning_positions = nullptr);
void drawBar(vector<char>& selection_bar, int position, char player, bool prompt=true);
int selectCol(vector<vector<char>>&, char player);
bool checkWin(vector<vector<char>>& board, vector<vector<int>> *winning_positions, int max_count = 3);
bool traverseDiag(vector<vector<char>>& board, vector<vector<int>> *winning_positions, int start_row, int start_col, int max_count, int step = 1);
void aiSelection(vector<vector<char>>& board, std::unique_ptr<Player>& player1, std::unique_ptr<Player>& player2, double weight = 0);
bool boardFull(vector<vector<char>>& board);

int main() {
	vector<vector<char>> board{};
	createBoard(board, 6, 7);

	menu(board);

	return 0;
}

void startTask(int position, vector<vector<char>>& board, std::unique_ptr<Player>& player1, std::unique_ptr<Player>& player2) {
	switch (position) {
	case 0:
		gamePlayLoop(board, player1, player2);
		clearBoard(board);
		break;
	case 1:
		gamePlayLoop(board, player1, player2, true);
		clearBoard(board);
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

void drawBoard(vector<vector<char>>& board, vector<vector<int>>* winning_positions) {
	bool win{ winning_positions != nullptr };
	vector<vector<int>> wpos{};
	if (win) wpos = *winning_positions;

	for (int i = 0; i <= board.size(); i++) {
		cout << termcolor::bright_blue << " |";
		for (int j = 0; j < 4.5*board.size(); j++) cout << "-";
		cout << "| " << termcolor::reset << endl;
		if (i == board.size()) break;

		for (int j = 0; j < board[0].size(); j++) {
			cout << termcolor::bright_blue << " | ";
			switch (board[i][j]) {
			case ' ':
				cout << termcolor::reset;
				break;
			case 'o':
				if (win && std::find(wpos.begin(), wpos.end(), vector<int>({ i, j })) != wpos.end()) { cout << termcolor::bright_green; }
				else { cout << termcolor::bright_yellow; }
				break;
			case 'x':
				if (win && std::find(wpos.begin(), wpos.end(), vector<int>({ i, j })) != wpos.end()) { cout << termcolor::bright_green; }
				else { cout << termcolor::bright_red; }
				break;
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
	int position{ (int)board[0].size() / 2 };
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
		case 'q':
			exit(0);
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
				else if (i == 0) {
					selection = false;
					break;
				}
			}
			if (selection) return position;
		}
	}
}

bool checkWin(vector<vector<char>>& board, vector<vector<int>>* winning_positions, int max_count) {
	bool win{ false };
	int count{};
	winning_positions->clear();

	for (size_t i = 0; i < board.size(); i++) {
		count = 0;

		for (size_t j = 1; j < board[0].size(); j++) {
			if (board[i][j - 1] == board[i][j] && board[i][j] != ' ') { count++; winning_positions->push_back(vector<int>({ int(i), int(j - 1) })); }
			else { count = 0; winning_positions->clear(); }
			if (count >= max_count) { winning_positions->push_back(vector<int>({ int(i), int(j) })); /*cout << " hline" << endl; system("pause");*/ return true; }
		}
	}

	winning_positions->clear();
	for (size_t j = 0; j < board[0].size(); j++) {
		count = 0;

		for (size_t i = 1; i < board.size(); i++) {
			if (board[i - 1][j] == board[i][j] && board[i][j] != ' ') { count++; winning_positions->push_back(vector<int>({ int(i - 1), int(j) })); }
			else { count = 0; winning_positions->clear(); }
			if (count >= max_count) { winning_positions->push_back(vector<int>({ int(i), int(j) })); /*cout << " vline" << endl; system("pause");*/ return true; }
		}
	}

	for (size_t i = 0; i < board.size() / 2; i++) {
		if (traverseDiag(board, winning_positions, i, 0, max_count)) { /*cout << " rdiag 1" << endl; system("pause");*/ return true; }
	}
	for (size_t j = 0; j < (board[0].size() + 1) / 2; j++) {
		if (traverseDiag(board, winning_positions, 0, j, max_count)) { /*cout << " rdiag 2" << endl; system("pause");*/ return true; }
	}
	for (size_t i = board.size()-1; i >= board.size() / 2; i--) {
		if (traverseDiag(board, winning_positions, i, 0, max_count, -1)) { /*cout << " ldiag 1" << endl; system("pause");*/ return true; }
	}
	for (size_t j = 1; j < (board[0].size() + 1) / 2; j++) {
		if (traverseDiag(board, winning_positions, board.size()-1, j, max_count,  -1)) { /*cout << " ldiag 2" << endl; system("pause");*/ return true; }
	}

	return false;
}

bool traverseDiag(vector<vector<char>>& board, vector<vector<int>>* winning_positions, int row, int col, int max_count, int step) {
	int count{};
	winning_positions->clear();

	for (size_t i = 0; i < board.size()*board[0].size(); i++) {
		if (board[row][col] == board[row + step][col + abs(step)] && board[row][col] != ' ') { count++; winning_positions->push_back(vector<int>({ row, col })); }
		else { count = 0; winning_positions->clear(); }
		row += step; col += abs(step);
		if (count >= max_count) { winning_positions->push_back(vector<int>({ row, col })); return true; }
		if (row + step >= board.size() || row + step < 0) return false;
		if (col + abs(step) >= board[0].size()) return false;
	}
}

void aiSelection(vector<vector<char>>& board, std::unique_ptr<Player>& player1, std::unique_ptr<Player>& player2, double weight) {
	vector<vector<int>> possible_moves{};
	vector<vector<char>> board_copy{};

	for (size_t i = 0; i < board.size(); i++) {
		board_copy.push_back(board[i]);
	}
	int selection{};
	
	for (int j = 0; j < board[0].size(); j++) {
		if (board[0][j] != ' ') continue;
		for (int i = 1; i < board.size(); i++) {
			if (board[i][j] != ' ') {
				possible_moves.push_back({ i - 1, j });
				break;
			}
			else if (i == board.size()-1) {
				possible_moves.push_back({ i , j });
			}
		}
	}

	std::random_device rd{};
	std::mt19937_64 gen(rd());
	std::uniform_int_distribution<int> RNG(0, possible_moves.size()-1);

	vector<vector<int>>* winning_positions = new vector<vector<int>>();

	selection = possible_moves[RNG(gen)][1];
	for (vector<int> &position : possible_moves) {
		board_copy[position[0]][position[1]] = player2->getMark();
		if (checkWin(board_copy, winning_positions)) {
			board_copy[position[0]][position[1]] = ' ';
			selection = position[1];
			break;
		}
		else if (checkWin(board_copy, winning_positions, 2)) {
			board_copy[position[0]][position[1]] = ' ';
			selection = position[1];
		}

		board_copy[position[0]][position[1]] = player1->getMark();
		if (checkWin(board_copy, winning_positions)) {
			board_copy[position[0]][position[1]] = ' ';
			selection = position[1];
			break;
		}
		else if (checkWin(board_copy, winning_positions, 2)) {
			board_copy[position[0]][position[1]] = ' ';
			selection = position[1];
		}

		board_copy[position[0]][position[1]] = ' ';
	}
	vector<char> selection_bar(board[0].size(), ' ');
	selection_bar[selection] = player2->getMark();

	delete winning_positions;
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

bool boardFull(vector<vector<char>>& board) {
	for (size_t j = 0; j < board[0].size(); j++) {
		if (board[0][j] == ' ') return false;
	}
	return true;
}

void menu(vector<vector<char>>& board) {
	std::unique_ptr<Player> player1; std::unique_ptr<Player> player2;
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
			startTask(position, board, player1, player2);
			break;
		case 'q':
			startTask(2,board,player1,player2);
			break;
		default:
			break;
		}

		// loops position around if out of menu range.
		if (position < 0) { position = menu_options.size() - 1; }
		else if (position >= menu_options.size()) { position = 0; }

	}
}

void gamePlayLoop(vector<vector<char>>& board, std::unique_ptr<Player>& player1, std::unique_ptr<Player>& player2, bool ai_on) {
	bool replay{ true };
	bool draw{ false };
	vector<vector<int>>* winning_moves = new vector<vector<int>>(4, vector<int>());

	createPlayer(1, player1);

	if (!ai_on) {
		createPlayer(2, player2);
	}
	else {
		player2 = std::make_unique<Player>(Player("AI", 'x'));
	}

	while (replay) {
		int turn{0};
		char mark{};

		while (true) {
			turn++;

			if (turn % 2) { mark = player1->getMark(); }
			else { mark = player2->getMark(); }

			if (!ai_on || (turn % 2)) { selectCol(board, mark); }
			else { aiSelection(board, player1, player2); }

			if (checkWin(board, winning_moves)) {
				if (turn % 2) { player1->incrementScore();}
				else { player2->incrementScore();}
				break;
			}
			else if (draw = boardFull(board)) {
				break;
			}
		}

		vector<string> options{ "Yes", "No" };
		int pos{};
		bool select{ false };
		while (!select) {
			char input{};
			system("cls");
			if (draw) { cout << termcolor::reset << " It's a draw!" << endl; drawBoard(board); }
			else {
				cout << termcolor::reset << " Winner is ";
				if (turn % 2) { cout << player1->getName(); }
				else { cout << player2->getName(); }
				cout << "!" << endl; drawBoard(board, winning_moves);
			}
			cout << " Play again?" << endl;
			//for (size_t i = 0; i < winning_moves->size(); i++) {
			//	cout << "(" << (*winning_moves)[i][0] << ", " << (*winning_moves)[i][1] << ")" << endl;
			//}
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
			case 'q':
				delete winning_moves;
				exit(0);
			default:
				break;
			}

			if (pos < 0) { pos = 0; }
			else if (pos >= 2) { pos = 1; }
		}
	}
	delete winning_moves;
}
