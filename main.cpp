using namespace std;

#include <iostream>
#include <iomanip>
#include <fstream>
#include <cmath>
#include <string>
#include <windows.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
//std::cout << "\261 \262"; - Interesting Snake Design

// Function to pause the program while using it/playing
void pause_for_user() { 
	string line;
	std::cin >> line;
}
 // Function to get the console window size by rows and columns (1 row/column = 1 pixel)
void get_console_window_size(int& columns, int& rows) {
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
}

// Function to set the cursor at X and Y position in the console window
void cursor_goto(int x, int y) {
	COORD coord = { x, y };
	SetConsoleCursorPosition(hConsole, coord);
}

// Drawing the "walls" (outside border)
void draw_window_outline(int width, int height) {
	for (int i = 0; i < width; i++) { cout << "-"; } // Upmost wall
	cursor_goto(0, height - 1);
	for (int i = 0; i < width; i++) { cout << "-"; } // Bottom wall
	cursor_goto(0, 1);
	for (int i = 1; i < height; i++) { cout << "|"; cursor_goto(0, i); } // Left wall
	cursor_goto(width - 1, 1);
	for (int i = 1; i < height; i++) { cout << "|"; cursor_goto(width - 1, i); } // Right wall
}

void is_key_pressed(int& button_direction) {
	if (_kbhit) {

		int c = _getch();
		switch (c) {
		case 72: // code for arrow up
			button_direction = 1;
			break;

		case 80: // code for arrow down
			button_direction = 2;
			break;

		case 75: // code for arrow left
			button_direction = 3;
			break;

		case 77: // code for arrow right
			button_direction = 4;
			break;
		}

	}
}

int main() {
	srand(time(NULL));

	// We get and save the Console window size in "consoleWidth/Height" variables
	int consoleWidth, consoleHeight;
	get_console_window_size(consoleWidth, consoleHeight);

	draw_window_outline(consoleWidth, consoleHeight);

	int snakeHeadX, snakeHeadY;
	snakeHeadX = consoleWidth / 2;
	snakeHeadY = consoleHeight / 2;

	// Initialize Snake
	cursor_goto(snakeHeadX, snakeHeadY); // We start at the centre
	cout << "O";
	cursor_goto(snakeHeadX, snakeHeadY); // Move the cursor back under the "Head"
	
	int button_direction; // 1-2-3-4 = up-down-left-right
	button_direction = rand() % 4 + 1;

	while(true) {
		
		switch (button_direction) {
		case 1:
			snakeHeadY--;
			break;
		case 2:
			snakeHeadY++;
			break;
		case 3:
			snakeHeadX--;
			break;
		case 4:
			snakeHeadX++;
			break;
		}

		if (snakeHeadX == consoleWidth - 1) snakeHeadX = 1;
		if (snakeHeadX == 0) snakeHeadX = consoleWidth - 2;
		if (snakeHeadY == consoleHeight - 1) snakeHeadY = 1;
		if (snakeHeadY == 0) snakeHeadY = consoleHeight - 2;

		cout << " ";
		cursor_goto(snakeHeadX, snakeHeadY);
		cout << "O";
		cursor_goto(snakeHeadX, snakeHeadY);
		Sleep(10);

		is_key_pressed(button_direction);
		FlushConsoleInputBuffer(hConsole);
	}

	std::cin.sync();
	//pause_for_user();
    return 0;
}
