// Compulsory 3, Anders P. Asbo. 
#include <iostream>
#include <string>
#include <vector>
#include <conio.h>
#include <windows.h>

// using termcolor by Ihor Kalnytskyi for coloring text in terminal:
#include "termcolor.hpp"  // https://github.com/ikalnytskyi/termcolor.git
// thanks to Marcus Nesvik Henriksen for bringing this library to my attention.

using std::cout; using std::cin; using std::string;
using std::endl; using std::vector;

void start_task(int);

void menu() {
	int position{};
	char input{};
	vector<string> menu_options = { "Play against player.", "Play against AI." };

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
		case '\cr':  // selects current menu item on press of enter key.
			system("cls");
			start_task(position);
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

int main() {
	menu();
	return 0;
}

void start_task(int position) {
}
