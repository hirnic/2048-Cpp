// Welcome! This is the code for the most popular game of ALLLLL TIIIIIIME: 2048.

#include "Animations.h"
#include "Mechanics.h"
#include <iostream>
#include <windows.h>

// The game grid that keeps track of the rectangle values and positions
// It has 16 positions and 1 dimension. To find the coordinates of a square,
// see the Brick struct in the Animations.h file.
std::vector<int> Grid;

// Window procedure callback function
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // Clear the window (background color)
            FillRect(hdc, &ps.rcPaint, (HBRUSH)(CreateSolidBrush(RGB(0, 0, 0))));

            // Draw all rectangles
            if (playing)
            {
                drawAllRectangles(hdc);
                updateScore(hdc, score);
                if (loseCondition(Grid))
                {
                    populate(Grid);
                    loseScreen(hdc);
                }
                if (contains(Grid, 2048) and !won)
                {
                    won = true;
                    keysOff = true;
                    winScreen(hdc);
                    ShowWindow(GetDlgItem(hwnd, 3), SW_SHOW);
                }
            }
            else
            {
                mainMenu(hdc, 400);
                EnableWindow(GetDlgItem(hwnd, 2), FALSE);
            }

            EndPaint(hwnd, &ps);
            break;
        }

        // These handle the button clicks.
        case WM_COMMAND:
        {
            // Check the ID of the button that was clicked using LOWORD(wParam)
            switch (LOWORD(wParam))
            {
                // This handles the "New Game Button"
                case 1:
                    playing = true;
                    Grid = std::vector<int>(16, 0);         // Reset grid
                    generateSquare(Grid);                       // Add a new square
                    generateSquare(Grid);                       // Add another square
                    populate(Grid);                             // Turns the grid into rectangles
                    InvalidateRect(hwnd, NULL, TRUE);  // Request a redraw
                    SetFocus(hwnd);                                // Ensure the window gets focus after the button click
                    EnableWindow(GetDlgItem(hwnd, 1), FALSE);   // Turn off new game button
                    EnableWindow(GetDlgItem(hwnd, 2), TRUE);    // Turn on quit game button
                    break;

                // This handles the quit button
                case 2:
                    playing = false;
                    InvalidateRect(hwnd, NULL, TRUE);
                    EnableWindow(GetDlgItem(hwnd, 1), TRUE);    // Turn on new game button
                    EnableWindow(GetDlgItem(hwnd, 2), FALSE);   // Turn off quit game button
                    break;

                // This handles the continue game button when you win.
                case 3:
                    keysOff = false;
                    InvalidateRect(hwnd, NULL, TRUE);
                    ShowWindow(GetDlgItem(hwnd, 3), SW_HIDE);
                    break;

                default:
                    break;
            }
            break;
        }

        // These are the key press commands
        case WM_KEYDOWN:
        {
            if (keysOff) {
                break;
            }
            gridSlide(Grid, wParam, hwnd);
            populate(Grid);
            InvalidateRect(hwnd, NULL, TRUE);
            break;
        }

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% //

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    const wchar_t CLASS_NAME[] = L"Sample Window Class";

    WNDCLASS wc = { };

    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = CLASS_NAME;

    if (!RegisterClass(&wc)) {
        return 0;  // If registration fails
    }

    HWND hwnd = CreateWindowEx(         // CreateWindowEx returns a handle to the new window
        0,                              // Optional window styles
        CLASS_NAME,                     // Window class
        L"2-To-The-Power-Of-Eleven",    // Window text
        WS_OVERLAPPEDWINDOW,            // Window style
        CW_USEDEFAULT,                  // Top Left corner x-coord
        CW_USEDEFAULT,                  // Top Left corner y-coord
        418,                            // Width
        490,                            // Height
        NULL,                           // Parent window
        NULL,                           // Menu
        hInstance,                      // Instance handle
        NULL                            // Additional Application Data
    );

    if (hwnd == NULL) {
        return 0;
    }

    // New Game Button
    CreateWindowEx(
        0,                      // Extended style
        L"BUTTON",               // Button class
        L"New Game",           // Button text
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, // Style
        10, 10,                 // Position
        100, 30,                 // Size
        hwnd,                    // Parent window
        (HMENU)1,                // Button ID
        hInstance,               // Instance handle
        NULL                     // Additional data
    );

    // Quit Button
    CreateWindowEx(
        0,                      // Extended style
        L"BUTTON",               // Button class
        L"Quit Game",           // Button text
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, // Style
        292, 10,                 // Position
        100, 30,                 // Size
        hwnd,                    // Parent window
        (HMENU)2,                // Button ID
        hInstance,               // Instance handle
        NULL                     // Additional data
    );

    // Continue playing button
    CreateWindowEx(
        0,                      // Extended style
        L"BUTTON",               // Button class
        L"Continue Playing",           // Button text
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, // Style
        135, 250,                 // Position
        125, 40,                 // Size
        hwnd,                    // Parent window
        (HMENU)3,                // Button ID
        hInstance,               // Instance handle
        NULL                     // Additional data
    );

    ShowWindow(GetDlgItem(hwnd, 3), SW_HIDE);


    ShowWindow(hwnd, nCmdShow);  // Show the window
    UpdateWindow(hwnd);          // Force a paint message

    // Run the Message Loop
    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int) msg.wParam;
}