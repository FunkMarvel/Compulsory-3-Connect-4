#pragma once
// Compulsory 3, Anders P. Asbo.
// AI header.

#include "player.h"  // includes player class and related functions.
#include <random>
#include <conio.h>

// using termcolor by Ihor Kalnytskyi for coloring text in terminal:
#include "termcolor.hpp"  // https://github.com/ikalnytskyi/termcolor.git
// thanks to Marcus Nesvik Henriksen for bringing this library to my attention.

// ai functions:
double difficultySelect();
void aiSelection(vector<vector<char>>& board, std::unique_ptr<Player>& player1, std::unique_ptr<Player>& player2, double weight);

