// Compulsory 3, Anders P. Asbo.
#include "connect4.h"

void startTask(int position, vector<vector<char>>& board, std::unique_ptr<Player>& player1, std::unique_ptr<Player>& player2) {
	//	Function for starting the tasks associated with each main-menu item.
	//
	//	Args:
	//		position -- integer with position of menu cursor, used to start correct task.
	//		board -- 2D char-vector passed by reference, contains game-board.
	//		player1 -- unique pointer to Player object, representing player 1.
	//		player2 -- unique pointer to Player object, representing player 2. 

	switch (position) {
	case 0:
		gamePlayLoop(board, player1, player2);  // starts Player vs Player.
		clearBoard(board);  // clears board after completed game.
		break;
	case 1:
		gamePlayLoop(board, player1, player2, true);  // starts Player vs AI.
		clearBoard(board);
		break;
	case 2:
		leaderBoard();  // displays leader board.
		break;
	case 3:
		exit(0);  // exits program.
	default:
		break;
	}
}

void createBoard(vector<vector<char>>& board, int num_rows, int num_cols) {
	//	Function for creating game board of dimensions num_rows*num_cols. The resulting 2D vector is stored in the board-parameter.
	//
	//	Args:
	//		board -- 2D char-vector passed by reference, contains game-board.
	//		num_rows -- integer containing number of rows.
	//		num_cols -- integer containing number of columns.

	for (size_t i = 0; i < num_rows; i++) {
		board.push_back(vector<char>(num_cols, ' '));  // filling up board with ' '-chars representing free tile on the game board.
	}
}

void clearBoard(vector<vector<char>>& board) {
	//	Function that sets all elements in provided 2D char-vector to ' '.
	//
	//	Args:
	//		board -- 2D char-vector passed by reference, contains game-board.

	for (size_t i = 0; i < board.size(); i++) {
		for (size_t j = 0; j < board[0].size(); j++) {
			board[i][j] = ' ';  // filling up board with ' '-chars representing free tile on the game board.
		}
	}
}

void drawBoard(vector<vector<char>>& board, vector<vector<int>>* winning_positions) {
	//	Function for creating game board of dimensions num_rows*num_cols. The resulting 2D vector is stored in the board-parameter.
	//
	//	Args:
	//		board -- 2D char-vector passed by reference, contains game-board.
	// 
	//	KeyWordArgs:
	//		winning_positions -- pointer to 2D vector of ints (default = nullptr) containing the positions of elements on game board
	//							 that form a connected line of 4.

	bool win{ winning_positions != nullptr };  // checks if someone has won the game.
	vector<vector<int>> wpos{};
	if (win) wpos = *winning_positions;  // copies winning positions if there has been a win.

	for (int i = 0; i <= board.size(); i++) {  // iterates through entire game board.

		// prints horisontal divider between each row of the game board:
		cout << termcolor::bright_blue << " |";
		for (int j = 0; j < 4.5 * board.size(); j++) cout << "-";
		cout << "| " << termcolor::reset << endl;
		if (i == board.size()) break;  // makes sure divider is printed undearneath final row.

		for (int j = 0; j < board[0].size(); j++) {  // prints actual characters from game board:
			cout << termcolor::bright_blue << " | ";  // prints vertical dividers between each column.

			switch (board[i][j]) {  // switches to yellow text for player 1 piece, red for player 2 piece and green if piece is part of winning combination:
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
			cout << board[i][j];  // prints actuall piece.
		}
		cout << termcolor::bright_blue << " | " << termcolor::reset << endl; // prints vertical dividers after final column.
	}
}

void drawBar(vector<char>& selection_bar, int position, char player, bool prompt) {
	// Function for drawing the selection bar that shows where a piece will be dropped on the board.
	// 
	// Args:
	//		selection_bar -- vector of char passed by reference, contains the piece that is to be dropped.
	//		position -- int with initial position of cursor along selection bar.
	//		player -- char containing the mark used for current player's pieces.
	// 
	// KeyWordArgs:
	//		prompt -- bool (default = true) telling function wether to accept user input on position of cursor.

	if (prompt) cout << termcolor::reset << " Navigate with 'a' and 'd', select column with 'enter':" << endl;
	for (int i = 0; i < selection_bar.size(); i++) {  // prints cursor in green:
		if (i == position) { cout << termcolor::bright_green << "   v"; }
		else { cout << "   " << selection_bar[i]; } 
	}
	cout << termcolor::reset << endl;

	for (int i = 0; i < selection_bar.size(); i++) {  // prints piece to drop in corresponding color:
		if (i == position && player == 'o') cout << termcolor::bright_yellow;
		if (i == position && player == 'x') cout << termcolor::bright_red;
		cout << "   " << selection_bar[i];
	}
	cout << termcolor::reset << endl;
}

int selectCol(vector<vector<char>>& board, char player) {
	// Function for letting non-AI player select where to drop their piece. Returns the chosen column.
	// 
	// Args:
	//		board -- 2D char-vector passed by reference, contains game-board.
	//		player -- char containing mark used for current player's pieces.
	//

	char input{};
	int position{ (int)board[0].size() / 2 };  // starts cursor at center of selection bar.
	bool selection{ false };
	vector<char> selection_bar(board[0].size(), ' ');

	while (true) {  // loops until return
		system("cls");
		selection_bar[position] = player;  // sets marker at correct position in selection bar.

		drawBar(selection_bar, position, player);  // draws bar.

		drawBoard(board);  // draws board.

		input = _getch();

		switch (tolower(input)) {  // moving cursor position allong acording to input:
		case 'a':
			selection_bar[position--] = ' ';
			break;
		case 'd':
			selection_bar[position++] = ' ';
			break;
		case '\r':  // if enter is hit, toggles flag that selection has been made.
			selection = true;
			selection_bar[position] = ' ';  // removes marker from selection bar as part of drop-animation.
			break;
		default:
			break;
		}

		// loops cursor around if out of bounds:
		if (position < 0) { position = selection_bar.size() - 1; }
		else if (position >= selection_bar.size()) { position = 0; }

		if (selection) {  // animates drop of piece if selection has been made:
			for (int i = 0; i < board.size(); i++) {
				if (board[i][position] == ' ') {
					board[i][position] = player;
					if (i != 0) board[i - 1][position] = ' ';
					system("cls");
					drawBar(selection_bar, position, player);
					drawBoard(board);
					Sleep(100);
				}
				else if (i == 0) {  // forces new selection of position if current column is full:
					selection = false;
					break;
				}
			}
			if (selection) return position;
		}
	}
}

bool checkWin(vector<vector<char>>& board, vector<vector<int>>* winning_positions, int max_count) {
	// Function that checks if the provided board contains at least one winning combination of pieces.
	// 
	//  Args:
	//		board -- 2D char-vector passed by reference, contains game-board.
	//		winning_positions -- pointer to 2D vector of ints (default = nullptr) containing the positions of elements on game board
	//							 that form a connected line of 4.
	//	KeyWordArgs:
	//		max_count -- int (default = 3) containing how many comparisons must be true before a win has been made.
	
	bool win{ false };
	int count{};
	winning_positions->clear();  // sanitizing vector for winning combination.

	for (size_t i = 0; i < board.size(); i++) {  // checks for 4 in a row:
		count = 0;

		for (size_t j = 1; j < board[0].size(); j++) {
			if (board[i][j - 1] == board[i][j] && board[i][j] != ' ') { count++; winning_positions->push_back(vector<int>({ int(i), int(j - 1) })); }
			else { count = 0; winning_positions->clear(); }  // sanitizing vector for winning combination, and resetting coutn if not max_count + 1 in a row.
			if (count >= max_count) { winning_positions->push_back(vector<int>({ int(i), int(j) })); return true; } // if number of true comparisons reaches max_count, there is max_count + 1 in a row.
		}
	}

	winning_positions->clear();
	for (size_t j = 0; j < board[0].size(); j++) { // checks for 4 in a column:
		count = 0;

		for (size_t i = 1; i < board.size(); i++) {
			if (board[i - 1][j] == board[i][j] && board[i][j] != ' ') { count++; winning_positions->push_back(vector<int>({ int(i - 1), int(j) })); }
			else { count = 0; winning_positions->clear(); } // sanitizing vector for winning combination, and resetting coutn if not max_count + 1 in a column.
			if (count >= max_count) { winning_positions->push_back(vector<int>({ int(i), int(j) })); return true; } // if number of true comparisons reaches max_count, there is max_count + 1 in a column.
		}
	}

	for (size_t i = 0; i < board.size() / 2; i++) { // checks all upper-left to lower-right diagonals that start in column 0:
		if (traverseDiag(board, winning_positions, i, 0, max_count)) return true;
	}
	for (size_t j = 0; j < (board[0].size() + 1) / 2; j++) { // checks all upper-left to lower-right diagonals that don't start in column 0:
		if (traverseDiag(board, winning_positions, 0, j, max_count)) return true;
	}
	for (size_t i = board.size() - 1; i >= board.size() / 2; i--) { // checks all lower-left to upper-right diagonals that start in column 0:
		if (traverseDiag(board, winning_positions, i, 0, max_count, -1)) return true;
	}
	for (size_t j = 1; j < (board[0].size() + 1) / 2; j++) { // checks all lower-left to upper-right diagonals that don't start in column 0:
		if (traverseDiag(board, winning_positions, board.size() - 1, j, max_count, -1)) return true;
	}

	return false;  // returns false if no win.
}

bool traverseDiag(vector<vector<char>>& board, vector<vector<int>>* winning_positions, int row, int col, int max_count, int step) {
	// Function for traversing diagonal in game board. Used in checkWin. Returns true if max_count + 1 equal successive elements in diagonal.
	// 
	// Args:
	//		board -- 2D char-vector passed by reference, contains game-board.
	//		winning_positions -- pointer to 2D vector of ints (default = nullptr) containing the positions of elements on game board
	//							 that form a connected line of 4.
	//		row -- int row-index for first element.
	//		col -- int column-index for first element.
	//		max_count -- int containing how many comparisons must be true before a win has been made.
	//		
	// KeyWordArgs:
	//		step -- int containing stepsize for moving along diagonal. Positive step gives upper-left to lower-right diagonal,
	//				negative step gives upper-right to lower-left diagonal.

	int count{};
	winning_positions->clear(); // sanitizing vector for winning combination.

	for (size_t i = 0; i < board.size() * board[0].size(); i++) {  // counts number of equal succesive elements along diagonal:
		if (board[row][col] == board[row + step][col + abs(step)] && board[row][col] != ' ') { count++; winning_positions->push_back(vector<int>({ row, col })); }
		else { count = 0; winning_positions->clear(); } // sanitizing vector for winning combination, and resetting coutn if not max_count + 1 in a row.
		row += step; col += abs(step);  // incrementing indices.
		if (count >= max_count) { winning_positions->push_back(vector<int>({ row, col })); return true; }  // returning true if win-combination.

		// returning false if out of bounds:
		if (row + step >= board.size() || row + step < 0) return false;
		if (col + abs(step) >= board[0].size()) return false;
	}
}

bool boardFull(vector<vector<char>>& board) {
	// Function checks if board is full. Returns true if full, false otherwise.
	// 
	// Args:
	//		board -- 2D char-vector passed by reference, contains game-board.

	for (size_t j = 0; j < board[0].size(); j++) {
		if (board[0][j] == ' ') return false;  // checking if there is at least one free tile in top row.
	}
	return true;
}

void leaderBoard() {
	// Function that prints leader board with player scores.

	system("cls");
	vector<Player> players{ loadPlayers(Player::getFilename()) };  // retrieves sorted vector of all saved players except AI.

	// prints board headers:
	cout << termcolor::bright_blue << "\t  ~~ Leader Board ~~" << endl;
	cout << termcolor::reset << std::setw(5) << std::left << "Rank:" << " | " << std::setw(20) << std::left << "Name:" << " | " << std::setw(12) << std::left << "Total Wins:" << endl;
	for (int i = players.size() - 1; i >= 0; i--) {  // vector is sorted from lowest to highest score, so loop prints elements in reverse:
		cout << std::setw(5) << players.size() - i << " | " << std::setw(20) << players[i].getName() << " | " << std::setw(12) << players[i].getHighScore() << endl;
	}
	system("pause");
}

void menu(vector<vector<char>>& board) {
	// Function displaying main menu.
	//
	// Args:
	//		board -- 2D char-vector passed by reference, contains game-board.

	// creates unique pointers for active players. (not really necesarry, but i can't be bothered to change it):
	std::unique_ptr<Player> player1; std::unique_ptr<Player> player2;
	int position{};
	char input{};

	// Menu options to display:
	vector<string> menu_options = { "Play against player.", "Play against AI.", "Display leader board", "Quit to desktop." };

	while (true) {  // loop that displays menu and handles input:

		system("cls");  // Printing menu options:
		cout << termcolor::bright_blue << " ~~~ Connect 4 ~~~" << termcolor::reset << endl;
		for (int i = 0; i < menu_options.size(); i++) {
			if (i == position) {  // ads cursor at current selection.
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
		case 'q':  // quits on press of q.
			startTask(3, board, player1, player2);
			break;
		default:
			break;
		}

		// loops position around if out of menu range:
		if (position < 0) { position = menu_options.size() - 1; }
		else if (position >= menu_options.size()) { position = 0; }

	}
}

void gamePlayLoop(vector<vector<char>>& board, std::unique_ptr<Player>& player1, std::unique_ptr<Player>& player2, bool ai_on) {
	//	Function for starting the tasks associated with each main-menu item.
	//
	//	Args:
	//		board -- 2D char-vector passed by reference, contains game-board.
	//		player1 -- unique pointer to Player object, representing player 1.
	//		player2 -- unique pointer to Player object, representing player 2. 
	//	KeyWordArgs:
	//		ai_on -- bool (default = false), uses ai as player 2 if true.
	//				 If false, player 2 is human controlled.

	bool replay{ true };
	bool draw{ false };
	double weight{};
	vector<vector<int>>* winning_moves = new vector<vector<int>>(4, vector<int>());

	createPlayer(1, player1);  // asks for player name and assigns Player-object to player1 pointer.

	if (!ai_on) {
		createPlayer(2, player2);  // asks for player name and assigns Player-object to player2 pointer if ai is off.
	}
	else {
		weight = difficultySelect();  // assigns difficulty weight for aiSelection() if ai on.
		player2 = std::make_unique<Player>(Player("A I", 'x')); //  assigns AI Player-object to player2 pointer if ai on.
	}

	// loads existing scores of playeres if they have one:
	player1->load(Player::getFilename());
	player2->load(Player::getFilename());

	while (replay) {  // replay loop persists until user no longer wants to play.
		int turn{ 0 };  // turn-counter.
		char mark{};

		while (true) {  // actual game play loop:
			turn++;

			if (turn % 2) { mark = player1->getMark(); }  // player one on odd turns.
			else { mark = player2->getMark(); }  // player 2 on even turns.

			// asks for selection if human player, or runs aiSelection() if ai's turn:
			if (!ai_on || (turn % 2)) { selectCol(board, mark); }
			else { aiSelection(board, player1, player2, weight); }

			if (checkWin(board, winning_moves)) {  // checks for win, and increments winner's score if win:
				if (turn % 2) { player1->incrementScore(); }
				else { player2->incrementScore(); }
				break;  // breaks out of game play loop
			}
			else if (draw = boardFull(board)) { // checks for draw, and breaks out of game play loop if true.
				break;
			}
		}

		// this section is the replay menu:
		vector<string> options{ "Yes", "No" };
		int pos{};
		bool select{ false };

		while (!select) {  // loops until player has chosen wether to replay:
			char input{};

			system("cls");  // prints game end message and final board:
			if (draw) { drawBoard(board);  cout << termcolor::reset << " It's a draw!" << endl; }
			else {
				drawBoard(board, winning_moves);
				cout << termcolor::reset << " Winner is ";
				if (turn % 2) { cout << player1->getName(); }
				else { cout << player2->getName(); }
				cout << "!" << endl;
			}

			// prints scores of current players:
			cout << " Scores: " << endl;
			cout << "  Player 1  | This session: | Total: |" << endl;
			cout << " " << std::setw(10) << player1->getName() << " | " << std::setw(13) << player1->getScore() << " | " << std::setw(6) << player1->getHighScore() << " |" << endl;
			cout << "  Player 2  | This session: | Total: |" << endl;
			cout << " " << std::setw(10) << player2->getName() << " | " << std::setw(13) << player2->getScore() << " | " << std::setw(6) << player2->getHighScore() << " |" << endl;

			// prints options for replay:
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

			switch (input) {  // moves cursor or selects menu item on user input:
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
			case 'q':  // terminate program on 'q'.
				delete winning_moves;
				exit(0);
			default:
				break;
			}

			// bounds checking for menu:
			if (pos < 0) { pos = 0; }
			else if (pos >= 2) { pos = 1; }
		}
	}

	// saves scores of players if user chooses to return to menu:
	player1->save(Player::getFilename());
	player2->save(Player::getFilename());
	delete winning_moves;
}