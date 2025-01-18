#ifndef RECTANGLES_H
#define RECTANGLES_H

#include <windows.h>
#include <vector>  // Include vector header
#include <cmath>

// A structure to hold rectangle information (position and color)
struct Brick {
    int mPosition;
    int mValue;
    COLORREF mColor = mValue==0 ? RGB(0,0,0) :
    RGB(90 * ((32*mValue % 27) + 1),
        121 * ((32*mValue % 27) + 1),
        150 * ((32*mValue %  27) + 1));

    int x1 = 1 + (mPosition % 4) * 100;
    int y1 = 51 + (mPosition / 4) * 100;
    int x2 = 99 + (mPosition % 4) * 100;
    int y2 = 149 + (mPosition / 4) * 100;
};

// Declare the external rectangles vector (no need to define it here)
extern std::vector<Brick> rectangles;

// Brings player to main menu
void mainMenu(HDC hdc,int windowWidth);

void updateScore(HDC hdc, int score);

void loseScreen(HDC hdc);

void winScreen(HDC hdc);

// Function to draw all rectangles
void drawAllRectangles(HDC hdc);

// Function to draw a single rectangle
void drawARectangle(HDC hdc, int gridNumber, COLORREF color);

void slideAnimation(WPARAM wParam, int init, HWND hwnd);

#endif // RECTANGLES_H