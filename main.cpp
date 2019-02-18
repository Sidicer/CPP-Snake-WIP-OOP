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

// For ASCII Character Support
#include <io.h> // need to add this include for _setmode
#include <fcntl.h> // need to add this include for _O_TEXT
#include <cstdio> // need to add this include for wprintf

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
//std::cout << "\261 \262"; - Interesting Snake Design

// Function to set the cursor at X and Y position in the console window
void cursor_goto(int x, int y) {
	COORD coord = { x, y };
	SetConsoleCursorPosition(hConsole, coord);
}

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

// Drawing the "walls" (outside border)
void draw_window_outline(int width, int height, int score) {
	_setmode(_fileno(stdout), 0x20000); // For ASCII to work

	for (int i = 0; i < width - 8; i++) { // Top Border (-8 compensates for "S N A K E")
		if (i == 0)
			wprintf(L"╔");
		else if (i == width - 9) // -9 compensates for "S N A K E"
			wprintf(L"╗");
		else if (i == width / 2)
			wprintf(L"S N A K E");
		else
			wprintf(L"═");
	}

	cursor_goto(0, height - 2); // Bottom Border
	for (int i = 0; i < width; i++) { 
		if (i == 0)
			wprintf(L"╚");
		else if (i == width - 1)
			wprintf(L"╝");
		else
			wprintf(L"═"); 
	} 

	cursor_goto(0, 1); // Left border
	for (int i = 1; i < height - 1; i++) {
		wprintf(L"║"); cursor_goto(0, i);
	} 

	cursor_goto(width - 1, 1); // Right border
	for (int i = 1; i < height - 1; i++) { 
		wprintf(L"║"); cursor_goto(width - 1, i);
	} 

	_setmode(_fileno(stdout), _O_TEXT); // Reset from ASCII (to fix cout)

	cursor_goto(1, height - 1);
	cout << "Score: " << score;
}

void is_key_pressed(int& button_direction) {
	if (_kbhit() && _getch()) {
		switch (_getch()) {
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
	int score = 0;

	// We get and save the Console window size in "consoleWidth/Height" variables
	int consoleWidth, consoleHeight;
	get_console_window_size(consoleWidth, consoleHeight);
	draw_window_outline(consoleWidth, consoleHeight, score);

	int snakeHeadX, snakeHeadY;
	snakeHeadX = consoleWidth / 2;
	snakeHeadY = consoleHeight / 2;

	// Initialize Snake
	cursor_goto(snakeHeadX, snakeHeadY); // We start at the centre
	cout << "\262";
	cursor_goto(snakeHeadX, snakeHeadY); // Move the cursor back under the "Head"
	
	srand(time(NULL));
	int button_direction; // 1-2-3-4 = up-down-left-right
	button_direction = rand() % 4 + 1;

	while(true) {
		is_key_pressed(button_direction);

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
		if (snakeHeadY == consoleHeight - 2) snakeHeadY = 1;
		if (snakeHeadY == 0) snakeHeadY = consoleHeight - 3;

		cout << " ";
		cursor_goto(snakeHeadX, snakeHeadY);
		cout << "\262";
		cursor_goto(snakeHeadX, snakeHeadY);
		
		Sleep(100);
	}
	std::cin.sync();
	//pause_for_user();
    return 0;
}
