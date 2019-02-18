#include <iostream>
#include <iomanip>
#include <fstream>
#include <cmath>
#include <string>

#include <windows.h>
#include <conio.h> // _kbhit() / _getch()
#include <stdlib.h>
#include <time.h> // For srand randomization

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
	std::string line;
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
		wprintf(L"║");
		cursor_goto(0, i);
	} 

	cursor_goto(width - 1, 1); // Right border
	for (int i = 1; i < height - 1; i++) { 
		wprintf(L"║");
		cursor_goto(width - 1, i);
	} 

	cursor_goto(1, height - 1);
	wprintf(L"Score: %i",score);
}

void is_key_pressed(int& direction) {
	if (_kbhit() && _getch()) {
		switch (_getch()) {
			case 72: // code for arrow up
			if (direction != 2) direction = 1;
			break;

			case 80: // code for arrow down
			if (direction != 1) direction = 2;
			break;

			case 75: // code for arrow left
			if (direction != 4) direction = 3;
			break;

			case 77: // code for arrow right
			if (direction != 3) direction = 4;
			break;
		}
	}
}

int main() {

	int score(0),
		consoleWidth, consoleHeight,
		snakeHeadX, snakeHeadY,
		fruitX, fruitY,
		direction; // 1-2-3-4 = up-down-left-right

	// We get and save the Console window size in "consoleWidth/Height" variables
	get_console_window_size(consoleWidth, consoleHeight);
	draw_window_outline(consoleWidth, consoleHeight, score);

	srand(time(NULL));
	direction = rand() % 4 + 1;

	snakeHeadX = consoleWidth / 2;
	snakeHeadY = consoleHeight / 2;

	srand(time(NULL));
	fruitX = rand() % (consoleWidth - 2) + 1;
	fruitY = rand() % (consoleHeight - 3) + 1;
	cursor_goto(fruitX, fruitY);
	wprintf(L"@");

	while(true) {
		is_key_pressed(direction);

		switch (direction) {
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

		if (snakeHeadX == fruitX && snakeHeadY == fruitY) {
			wprintf(L" ");

			score += 10;
			cursor_goto(1, consoleHeight - 1);
			wprintf(L"Score: %i", score);
			Sleep(50);

			srand(time(NULL));
			fruitX = rand() % (consoleWidth - 2) + 1;
			fruitY = rand() % (consoleHeight - 3) + 1;
			cursor_goto(fruitX, fruitY);
			wprintf(L"@");
		}

		wprintf(L" ");
		cursor_goto(snakeHeadX, snakeHeadY);
		wprintf(L"█");
		cursor_goto(snakeHeadX, snakeHeadY);
		
		Sleep(100);
	}

	_setmode(_fileno(stdout), _O_TEXT); // Reset from ASCII (to fix "std::cout")
	std::cin.sync();
	pause_for_user();
  return 0;
}
