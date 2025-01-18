// This takes care of the internal rules and mechanics of the game

#include "Animations.h"
#include "Mechanics.h"

int score {0};
bool isMoving {false};
bool won {false};
bool playing {false};
bool keysOff {false};

// This checks list membership
bool contains(const std::vector<int>& vec, int value)
{
    for (int v : vec)
    {
        if (v == value)
        {
            return true;  // Found the value
        }
    }
    return false;  // Value not found
}

// Converts the grid to rectangles in the Animations file.
void populate(std::vector<int>& grid)
{
    rectangles = {};
    for (int i {0}; i < 16; i++)
    {
        rectangles.push_back({i, grid[i]});
    }
}

// This command randomly generates a square in the available space
void generateSquare(std::vector<int>& grid)
{
    // First we find which spaces are unoccupied
    std::vector<int> gridFree;
    for (int i {0}; i < 16; i++)
    {
        if (grid[i] == 0)
        {
            gridFree.push_back(i);
        }
    }
    // Create a random number generator
    std::random_device rd;
    std::mt19937 gen(rd());     // Mersenne Twister engine
    std::uniform_int_distribution<> dis(0, gridFree.size() - 1);  // Random index in range [0, vec.size() - 1]
    // Choose a random entry from the vector gridFree
    int randomIndex {dis(gen)};
    int randomValue {gridFree[randomIndex]};
    // Define a uniform distribution between 0.0 and 1.0
    std::uniform_real_distribution<> dist(0.0, 1.0);
    // Generate a random number
    double randomNumber {dist(gen)};
    int newRectangle {2};
    if (randomNumber > 0.9)
    {
        newRectangle = 4;
    }
    grid[randomValue] = newRectangle;
}

// This command takes care of sliding all the rectangles in the direction of the key pressed.
void gridSlide(std::vector<int>& grid, WPARAM wParam, HWND hwnd)
{
    const std::vector<int> newGrid {grid};
    // We go row-by-row (or column-by-column) 3 times each.
    // For each time we do that, we go square by square, checking the current square with
    // the previous square in the sliding direction.
    int current {0}, previous {0};
    int scoreMultiplier {1};
    for (int number {0}; number < 4; ++number) // Row number or column number
    {
        for (int k {0}; k < 3; ++k)         // Iterating 3 times ensures the sliding process goes to completion
        {
            for (int i {0}; i < 3; ++i)     // This slides each rectangle in a row or column exactly once
            {
                switch (wParam)
                {
                    // These cases decide the identity of the current square and the next square.
                    // Remember, grid is a vector of 16 numbers.
                    case VK_LEFT:
                        current = 4 * number + i;
                        previous = 4 * number + i + 1;
                    break;
                    case VK_RIGHT:
                        current = 4 * number + 3 - i;
                        previous = 4*number + 2 - i;
                    break;
                    case VK_UP:
                        current = number + 4 * i;
                        previous = number + 4 * (i+1);
                    break;
                    case VK_DOWN:
                        current = number + 4 * (3-i);
                        previous = number + 4*(2-i);
                    break;
                    default:
                        break;
                }
                if (grid[current] == 0)
                {
                    grid[current] = grid[previous];
                    grid[previous] = 0;
                    // This if statement is designed to save computation time
                    if (grid[current] != 0)
                    {
                        slideAnimation(wParam, previous, hwnd);
                        populate(grid);
                    }
                }
                // This is where blocks combine
                else if (grid[current] == grid[previous])
                {
                    grid[current] = 2 * grid[previous];
                    grid[previous] = 0;
                    // We want the score to increase more if multiple combinations happen that if only one happened.
                    // The only issue I have with this is that it indicates a preference for the combinations at the
                    // bottom and the right over those at the top and the left. I don't feel like finding a workaround.
                    score +=  scoreMultiplier * grid[current];
                    scoreMultiplier += 1;
                    slideAnimation(wParam, previous, hwnd);
                    populate(grid);
                }
            }
        }
    }
    if (newGrid != grid)
    {
        generateSquare(grid);
    }
}

// This checks if the grid is full and there are no more moves to make.
bool loseCondition(const std::vector<int>& grid)
{
    // This checks if the grid is actually full.
    if (contains(grid, 0))
    {
        return false;
    }
    // This loop check each pair of adjacent blocks in all four rows.
    for (int number{0}; number < 4; ++number)
    {
        for (int k {0}; k < 3; ++k)
        {
            if (grid[4*number + k] == grid[4*number + k+1])
            {
                return false;
            }
        }
    }
    // This loop checks each pair of adjacent blocks in all four columns.
    for (int number{0}; number < 4; ++number)
    {
        for (int k{0}; k < 3; ++k) {
            if (grid[number + 4*k] == grid[number + 4*(k+1)])
            {
                return false;
            }
        }
    }
    return true;
}
