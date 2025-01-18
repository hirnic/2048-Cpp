// This takes care of the internal rules and mechanics of the game

#ifndef MECHANICS_H
#define MECHANICS_H
#include <vector>
#include <random>

extern int score;
extern bool isMoving;
extern bool won;
extern bool playing;
extern bool keysOff;

// This checks list membership
bool contains(const std::vector<int>& vec, int value);

// Converts the grid to rectangles in the Rectangles file.
void populate(std::vector<int>& grid);

// This command randomly generates a square in the available space
void generateSquare(std::vector<int>& grid);

// This command takes care of sliding all the rectangles in the direction of the key pressed.
void gridSlide(std::vector<int>& grid, WPARAM wParam, HWND hwnd);

// This checks if the grid is full and there are no more moves to make.
bool loseCondition(const std::vector<int>& grid);

#endif //MECHANICS_H
