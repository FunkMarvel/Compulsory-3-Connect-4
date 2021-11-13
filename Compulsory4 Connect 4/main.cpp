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
void aiSelection(vector<vector<char>>& board, Player* player1, Player* player2, double weight = 0);
int minimax(vector<vector<char>> board, vector<vector<int>> possible_moves, Player* player, Player* AI, vector<int> position, int depth, /*int alpha, int beta,*/ bool maximising);
bool boardFull(vector<vector<char>>& board);
int scorePosition(vector<vector<char>> board, vector<int> position);

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
		gamePlayLoop(board, player1, player2, true);
		clearBoard(board);
		break;
	case 2:
		delete player1;
		delete player2;
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

	for (size_t i = 0; i < board.size() / 2; i++) {
		if (traverseDiag(board, i, 0)) return true;
	}
	for (size_t j = 0; j < (board[0].size() + 1) / 2; j++) {
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

void aiSelection(vector<vector<char>>& board, Player* player1, Player* player2, double weight) {
	vector<vector<int>> possible_moves{};
	int possible_win{};
	int possible_loss{};
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

	int prev_eval{10000};
	selection = possible_moves[RNG(gen)][1];
	for (vector<int> &position : possible_moves) {
		int eval{ minimax(board, possible_moves, player1, player2, position, 4, /*-10000, 10000,*/ true) };
		if (prev_eval <= eval) { selection = position[1]; prev_eval = eval; }
		//cout << " s = " << selection << endl;
		//system("pause");
	}

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

int minimax(vector<vector<char>> board, vector<vector<int>> possible_moves, Player* player, Player* AI, vector<int> position, int depth, /*int alpha, int beta,*/ bool maximising) {
	board[position[0]][position[1]] = AI->getMark();
	if (checkWin(board)) {
		if (maximising) return 10000;
		return -10000;
	}

	board[position[0]][position[1]] = player->getMark();
	if (checkWin(board)) {
		if (maximising) return 10000;
		return -10000;
	}
	board[position[0]][position[1]] = ' ';

	if (depth == 0 || boardFull(board)) {
		return scorePosition(board, position);
	}

	if (maximising) {
		int max_eval{ -10000 };
		int eval{};
		board[position[0]][position[1]] = AI->getMark();

		for (vector<int> &new_position : possible_moves) {
			if (new_position == position)  new_position[0]--;
			if (new_position[0] < 0) continue;
			eval = minimax(board, possible_moves, player, AI, new_position,  depth - 1, /*alpha, beta,*/ !maximising);
			max_eval = max(max_eval, eval);
			//alpha = max(alpha, max_eval);
			//if (beta <= alpha) break;
		}
		//board[position[0]][position[1]] = ' ';
		return max_eval;
	}
	else {
		int min_eval{ 10000 };
		int eval{};
		board[position[0]][position[1]] = player->getMark();

		for (vector<int> &new_position : possible_moves) {
			if (new_position == position)  new_position[0]--;
			if (new_position[0] < 0) continue;
			eval = minimax(board, possible_moves, player, AI, new_position, depth - 1, /*alpha, beta,*/ !maximising);
			min_eval = min(min_eval, eval);
			//beta = min(beta, min_eval);
			//if (beta <= alpha) break;
		}
		//board[position[0]][position[1]] = ' ';
		return min_eval;
	}
}

bool boardFull(vector<vector<char>>& board) {
	for (size_t j = 0; j < board[0].size(); j++) {
		if (board[0][j] == ' ') return false;
	}
	return true;
}

int scorePosition(vector<vector<char>> board, vector<int> position) {
	int score{};
	int weight{ 100 };


	for (int i = 1; i < board.size(); i++) {
		if (position[0] - i >= 0 && board[position[0]][position[1]] == board[position[0] - i][position[1]]) { score += weight; }
		else { break; }
	}

	for (int i = 1; i < board.size(); i++) {
		if (position[0] + i < board.size() && board[position[0]][position[1]] == board[position[0] + i][position[1]]) { score += weight; }
		else { break; }
	}

	for (int i = 1; i < board[0].size(); i++) {
		if (position[1] - i >= 0 && board[position[0]][position[1]] == board[position[0]][position[1] - i]) { score += weight; }
		else { break; }
	}

	for (int i = 1; i < board[0].size(); i++) {
		if (position[1] + i < board[0].size() && board[position[0]][position[1]] == board[position[0]][position[1] + i]) { score += weight; }
		else { break; }
	}

	for (int i = 1; i < board.size() * board[0].size(); i++) {
		if (position[1] - i >= 0 && position[0] - i >= 0 && board[position[0]][position[1]] == board[position[0] - i][position[1] - i]) { score += weight; }
		else { break; }
	}

	for (int i = 1; i < board.size() * board[0].size(); i++) {
		if (position[1] + i < board[0].size() && position[0] + i < board.size() && board[position[0]][position[1]] == board[position[0] + i][position[1] + i]) { score += weight; }
		else { break; }
	}

	for (int i = 1; i < board.size() * board[0].size(); i++) {
		if (position[1] - i >= 0 && position[0] + i < board.size() && board[position[0]][position[1]] == board[position[0] + i][position[1] - i]) { score += weight; }
		else { break; }
	}

	for (int i = 0; i < board.size() * board[0].size(); i++) {
		if (position[1] + i < board[0].size() && position[0] - i >= 0 && board[position[0]][position[1]] == board[position[0] - i][position[1] + i]) { score += weight; }
		else { break; }
	}

	return score;
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

		delete player1;
		delete player2;
	}

}

void gamePlayLoop(vector<vector<char>>& board, Player* player1, Player* player2, bool ai_on) {
	bool replay{ true };
	bool draw{ false };

	player1 = createPlayer(1);

	if (!ai_on) {
		player2 = createPlayer(2);
	}
	else {
		player2 = new Player("AI", 'x');
	}

	while (replay) {
		int turn{0};
		char mark{};
		Player* player{ nullptr };

		while (true) {
			turn++;

			if (turn % 2) { mark = player1->getMark(); }
			else { mark = player2->getMark(); }

			if (!ai_on || (turn % 2)) { selectCol(board, mark); }
			else { aiSelection(board, player1, player2); }

			if (checkWin(board)) {
				if (turn % 2) { player1->incrementScore(); player = player1; }
				else { player2->incrementScore(); player = player2; }
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
			if (draw) { cout << termcolor::reset << "It's a draw!" << endl; }
			else { cout << termcolor::reset << "Winner is " << player->getName() << "!" << endl; }
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
			case 'q':
				delete player1;
				delete player2;
				exit(0);
			default:
				break;
			}

			if (pos < 0) { pos = 0; }
			else if (pos >= 2) { pos = 1; }
		}
	}
}
