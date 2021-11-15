#pragma once
// Compulsory 3, Anders P. Asbo.
// main header for program.

#include "connect4_AI.h"  // includes player and ai functions.
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <memory>
#include <iomanip>

// user interface functions:
void menu(vector<vector<char>>& board);
void startTask(int, vector<vector<char>>& board, std::unique_ptr<Player>& player1, std::unique_ptr<Player>& player2);
int selectCol(vector<vector<char>>&, char player);
void drawBar(vector<char>& selection_bar, int position, char player, bool prompt = true);
void drawBoard(vector<vector<char>>& board, vector<vector<int>>* winning_positions = nullptr);
void leaderBoard();

// board creation and clearing:
void createBoard(vector<vector<char>>& board, int num_rows, int num_cols);
void clearBoard(vector<vector<char>>& board);

// general game logic:
void gamePlayLoop(vector<vector<char>>& board, std::unique_ptr<Player>& player1, std::unique_ptr<Player>& player2, bool ai_on = false);
bool checkWin(vector<vector<char>>& board, vector<vector<int>>* winning_positions, int max_count = 3);
bool traverseDiag(vector<vector<char>>& board, vector<vector<int>>* winning_positions, int start_row, int start_col, int max_count, int step = 1);
bool boardFull(vector<vector<char>>& board);