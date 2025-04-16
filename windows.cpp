#include <iostream>
#include <windows.h> // Required for console functions

using namespace std;

// Function to move the cursor to a specific position
void MoveCursor(int x, int y) {
    COORD coord;  // COORD structure to hold X and Y positions
    coord.X = x;  
    coord.Y = y;  
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

int main() {
    system("cls"); // Clear the screen

    cout << "This text is at the default position.\n";

    // Move cursor to (10, 5) and print text there
    MoveCursor(10, 5);
    cout << "Hello from (10,5)!";

    // Move cursor to (20, 10) and print another text
    MoveCursor(20, 10);
    cout << "Cursor moved to (20,10)!";

    // Move cursor to (5, 15) and print something else
    MoveCursor(5, 15);
    cout << "Custom cursor positioning works!";

    return 0;
}
