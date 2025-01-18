#include "Animations.h"
#include <vector>
#include <string>
#include <sstream>
#include <locale>
#include <windows.h>

// Global vector for storing rectangles
std::vector<Brick> rectangles;

// Function to bring player to main menu
void mainMenu(HDC hdc, int windowWidth)
{
    // Create and select the font for the text
    HFONT hFont = CreateFont(
        32, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
        L"Courier New");
    // Select the font into the device context
    SelectObject(hdc, hFont);

    std::wstring menuMessage = L"After pressing\n \"New Game,\" just use the"
                               " arrow keys to move the blocks around. The goal"
                               " is to get a 2048 tile. Good luck!";

    // Calculate the maximum width available (you could use windowWidth)
    int availableWidth = windowWidth - 50;  // 50 is padding from the left edge

    // Create a buffer for the wrapped text
    std::wstring wrappedText;
    std::wstring currentLine;
    SIZE textSize;

    // Create a stringstream to split words
    std::wistringstream wordStream(menuMessage);
    std::wstring word;

    // Iterate through each word and construct the lines
    while (wordStream >> word)
    {
        std::wstring testLine = currentLine + (currentLine.empty() ? L"" : L" ") + word;
        GetTextExtentPoint32(hdc, testLine.c_str(), testLine.length(), &textSize);

        if (textSize.cx <= availableWidth)
        {
            // If the line fits, add the word
            currentLine = testLine;
        }
        else
        {
            // If the line doesn't fit, push the current line to wrappedText and start a new line
            if (!currentLine.empty())
            {
                wrappedText += currentLine + L"\n";
            }
            currentLine = word;  // Start a new line with the current word
        }
    }

    // Add the last line
    if (!currentLine.empty())
    {
        wrappedText += currentLine;
    }

    // Calculate the position for the text (center the text horizontally)
    int textY = 50;  // Initial vertical position

    // Split the wrappedText into individual lines to draw each one separately
    size_t pos = 0;
    std::wstring line;
    while ((pos = wrappedText.find(L"\n")) != std::wstring::npos)
    {
        line = wrappedText.substr(0, pos);
        wrappedText.erase(0, pos + 1);  // Remove the newline character

        // Center the current line horizontally
        GetTextExtentPoint32(hdc, line.c_str(), line.length(), &textSize);
        int lineWidth = textSize.cx;
        int lineX = (windowWidth - lineWidth) / 2;

        // Output the line
        TextOutW(hdc, lineX, textY, line.c_str(), line.length());

        // Move down for the next line
        textY += textSize.cy + 10;  // Adjust vertical spacing between lines
    }

    // Output the remaining part of the text (if any)
    if (!wrappedText.empty())
    {
        GetTextExtentPoint32(hdc, wrappedText.c_str(), wrappedText.length(), &textSize);
        int lineX = (windowWidth - textSize.cx) / 2;
        TextOutW(hdc, lineX, textY, wrappedText.c_str(), wrappedText.length());
    }

    // Clean up resources
    DeleteObject(hFont);
}

// If you lose, this pops up.
void loseScreen(HDC hdc)
{
    HBRUSH hBrush = CreateSolidBrush(RGB(255,255,255));

    SelectObject(hdc, hBrush);

    //Background rectangle
    Rectangle(hdc, 80, 200, 320, 300);

    HFONT hFont = CreateFont(
        50, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
        L"Arial");

    SelectObject(hdc, hFont);

    // Prepare the message as a string to display
    std::wstring loseMessage = L"YOU LOSE!";

    // Decide on the location of the message
    int textX = 85;
    int textY = 225;

    // Set the text color to white
    SetTextColor(hdc, RGB(0,0,0));
    // Draw the message
    TextOutW(hdc, textX, textY, loseMessage.c_str(), loseMessage.length());

    // Clean up resources
    DeleteObject(hBrush);
    DeleteObject(hFont);
}

// If you get a 2048 block, this pops up.
void winScreen(HDC hdc)
{
    // Used for the outline
    HPEN hPen = CreatePen(PS_SOLID, 2, RGB(255,255,255));
    // Create a brush with the rectangle color
    HBRUSH hBrush = CreateSolidBrush(RGB(255,255,255));
    // Create and select the font for the text
    HFONT hFont = CreateFont(
        50, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
        L"Arial");

    // Select the pen into the device context
    SelectObject(hdc, hPen);
    // Select the brush into the device context
    SelectObject(hdc, hBrush);
    // Select the font into the device context
    SelectObject(hdc, hFont);

    // Draw the rectangle using the selected pen and brush
    Rectangle(hdc, 80, 200, 320, 300);

    // Prepare the message as a string to display
    std::wstring loseMessage = L"YOU WIN!";

    // Decide on the text location
    int textX {100};
    int textY {200};

    // Set the text color to white
    SetTextColor(hdc, RGB(0,0,0));
    // Draw the message
    TextOutW(hdc, textX, textY, loseMessage.c_str(), loseMessage.length());

    // Clean up resources
    DeleteObject(hPen);
    DeleteObject(hBrush);
    DeleteObject(hFont);
}

// Every time you combine two bricks the score goes up.
void updateScore(HDC hdc, int score)
{
    // Create and select the font for the text
    HFONT hFont = CreateFont(
        24, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
        L"Courier New");

    // Select the font into the device context
    SelectObject(hdc, hFont);

    // Prepare the message as a string to display
    std::wstring number = L"Score: " + std::to_wstring(score);

    // Decide on the text location
    int textX = 130;
    int textY = 10;

    // Set the text color to white
    SetTextColor(hdc, RGB(255,255,255));
    // Draw the score
    TextOutW(hdc, textX, textY, number.c_str(), number.length());

    // Clean up resources
    DeleteObject(hFont);
}

// Function to draw all rectangles along with their numbers
void drawAllRectangles(HDC hdc)
{
    for (const auto& rect : rectangles)
    {
        if (rect.mValue <2) {
            continue;
        }
        // Create a brush with the rectangle color
        HBRUSH hBrush = CreateSolidBrush(rect.mColor);
        // Create and select the font for the text
        HFONT hFont = CreateFont(
            50, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
            L"Arial");
        // Select the font into the device context
        SelectObject(hdc, hFont);
        // Select the brush into the device context
        SelectObject(hdc, hBrush);

        // Draw the rectangle using the selected pen and brush
        Rectangle(hdc, rect.x1, rect.y1, rect.x2, rect.y2);

        // Ensure rect.number has a valid value (you can add more robust checks here)
        if (rect.mValue <2) {
            DeleteObject(hBrush);
            DeleteObject(hFont);
            continue;
        }

        // Prepare the number as a string to display
        std::wstring number = std::to_wstring(rect.mValue);

        // Calculate the position for the text (center the text in the rectangle)
        SIZE textSize;
        if (!GetTextExtentPoint32W(hdc, number.c_str(), number.length(), &textSize)) {
            // If text size calculation fails, skip the text drawing
            DeleteObject(hBrush);
            DeleteObject(hFont);
            continue;
        }

        int textWidth = textSize.cx;
        int textHeight = textSize.cy;

        // Centering text in the rectangle
        int textX = (rect.x1 + rect.x2 - textWidth) / 2;
        int textY = (rect.y1 + rect.y2- textHeight) / 2;

        // Draw the number inside the rectangle
        // Set the text color to black
        SetTextColor(hdc, RGB(0,0,0));
        // Make the background transparent
        SetBkMode(hdc, TRANSPARENT);
        // Draw the number
        TextOutW(hdc, textX, textY, number.c_str(), number.length());

        // Clean up resources
        DeleteObject(hBrush);
        DeleteObject(hFont);
    }
}

// Function to draw all rectangles along with their numbers
void drawARectangle(HDC hdc, int gridNumber, COLORREF color)
{
    Brick rect = rectangles[gridNumber];
    // Create a brush with the rectangle color
    HBRUSH hBrush = CreateSolidBrush(color);
    // Create and select the font for the text
    HFONT hFont = CreateFont(
        50, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
        L"Arial");
    // Select the font into the device context
    SelectObject(hdc, hFont);
    // Select the brush into the device context
    SelectObject(hdc, hBrush);

    // Draw the rectangle using the selected pen and brush
    Rectangle(hdc, rect.x1, rect.y1, rect.x2, rect.y2);

    // Prepare the number as a string to display
    std::wstring number = std::to_wstring(rect.mValue);

    // Calculate the position for the text (center the text in the rectangle)
    SIZE textSize;
    if (!GetTextExtentPoint32W(hdc, number.c_str(), number.length(), &textSize)) {
        // If text size calculation fails, skip the text drawing
        DeleteObject(hBrush);
        DeleteObject(hFont);
    }

    int textWidth = textSize.cx;
    int textHeight = textSize.cy;

    // Centering text in the rectangle
    int textX = (rect.x1 + rect.x2 - textWidth) / 2;
    int textY = (rect.y1 + rect.y2- textHeight) / 2;

    // Set the text color to black
    SetTextColor(hdc, RGB(0,0,0));
    // Make the background transparent
    SetBkMode(hdc, TRANSPARENT);
    // Draw the number
    TextOutW(hdc, textX, textY, number.c_str(), number.length());

    // Clean up resources
    DeleteObject(hBrush);
    DeleteObject(hFont);
}

// When the blocks start sliding, they need to undergo this animation
void slideAnimation(WPARAM wParam, int init, HWND hwnd)
{
    // Used for updating the coordinates
    int xAddMe {0};
    int yAddMe {0};
    const int steps {50};
    switch (wParam)
    {
        case VK_LEFT:
            xAddMe = -100/steps;
        break;
        case VK_RIGHT:
            xAddMe = 100/steps;
        break;
        case VK_UP:
            yAddMe = -100/steps;
        break;
        case VK_DOWN:
            yAddMe = 100/steps;
        break;
        default:
            break;
    }
    for (int i {0}; i < steps; ++i)
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        // Clear the window (background color)
        drawARectangle(hdc, init, RGB(0,0,0));
        rectangles[init].x1+=xAddMe;
        rectangles[init].x2+=xAddMe;
        rectangles[init].y1+=yAddMe;
        rectangles[init].y2+=yAddMe;
        drawARectangle(hdc, init, rectangles[init].mColor);
        EndPaint(hwnd, &ps);
        InvalidateRect(hwnd, NULL, TRUE);
    }
}
