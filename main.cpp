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

struct Coordinates {
	int X, Y;
};

class Snake {
	public:
		int direction,
			headX,
			headY,
			tailX,
			tailY;
};

class Console {
	public:
		int width,
			height;
};


Snake snake;
Console console;
Coordinates fruit;

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
void get_console_window_size() {
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	console.width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	console.height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
}

// Drawing the "walls" (outside border)
void draw_window_outline(int score) {
	_setmode(_fileno(stdout), 0x20000); // For ASCII to work

	for (int i = 0; i < console.width - 8; i++) { // Top Border (-8 compensates for "S N A K E")
		if (i == 0)
			wprintf(L"╔");
		else if (i == console.width - 9) // -9 compensates for "S N A K E"
			wprintf(L"╗");
		else if (i == console.width / 2)
			wprintf(L"S N A K E");
		else
			wprintf(L"═");
	}

	cursor_goto(0, console.height - 2); // Bottom Border
	for (int i = 0; i < console.width; i++) {
		if (i == 0)
			wprintf(L"╚");
		else if (i == console.width - 1)
			wprintf(L"╝");
		else
			wprintf(L"═"); 
	} 

	cursor_goto(0, 1); // Left border
	for (int i = 1; i < console.height - 1; i++) {
		wprintf(L"║");
		cursor_goto(0, i);
	} 

	cursor_goto(console.width - 1, 1); // Right border
	for (int i = 1; i < console.height - 1; i++) {
		wprintf(L"║");
		cursor_goto(console.width - 1, i);
	} 

	cursor_goto(1, console.height - 1);
	wprintf(L"Score: %i",score * 10);
}

void is_key_pressed() {
	if (_kbhit() && _getch()) {
		switch (_getch()) {
			case 72: // code for arrow up
			if (snake.direction != 2) snake.direction = 1;
			break;

			case 80: // code for arrow down
			if (snake.direction != 1) snake.direction = 2;
			break;

			case 75: // code for arrow left
			if (snake.direction != 4) snake.direction = 3;
			break;

			case 77: // code for arrow right
			if (snake.direction != 3) snake.direction = 4;
			break;
		}
	}
}

int main() {

	int score(0), gameOver(false);

	// We get and save the Console window size in "consoleWidth/Height" variables
	get_console_window_size();
	draw_window_outline(score);

	srand(time(NULL));
	snake.direction = rand() % 4 + 1;

	snake.headX = console.width / 2;
	snake.headY = console.height / 2;

	srand(time(NULL));
	fruit.X = rand() % (console.width - 2) + 1;
	fruit.Y = rand() % (console.height - 3) + 1;
	cursor_goto(fruit.X, fruit.Y);
	wprintf(L"@");

	while (!gameOver) {
		is_key_pressed();

		switch (snake.direction) {
		case 1:
			snake.headY--;
			break;
		case 2:
			snake.headY++;
			break;
		case 3:
			snake.headX--;
			break;
		case 4:
			snake.headX++;
			break;
		}

		if (snake.headX == console.width - 1) snake.headX = 1;
		if (snake.headX == 0) snake.headX = console.width - 2;
		if (snake.headY == console.height - 2) snake.headY = 1;
		if (snake.headY == 0) snake.headY = console.height - 3;

		if (snake.headX == fruit.X && snake.headY == fruit.Y) {
			wprintf(L" ");

			score++;
			cursor_goto(1, console.height - 1);
			wprintf(L"Score: %i", score * 10);
			Sleep(50);

			srand(time(NULL));
			fruit.X = rand() % (console.width - 2) + 1;
			fruit.Y = rand() % (console.height - 3) + 1;
			cursor_goto(fruit.X, fruit.Y);
			wprintf(L"@");
		}

		wprintf(L" ");
		cursor_goto(snake.headX, snake.headY);
		wprintf(L"█");
		cursor_goto(snake.headX, snake.headY);
		
		Sleep(50);
	}

	_setmode(_fileno(stdout), _O_TEXT); // Reset from ASCII (to fix "std::cout")
	std::cin.sync();
	pause_for_user();
    return 0;
}
