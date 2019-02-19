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

class Snake {
private:
	int direction,
		  headX,
		  headY,
		  tailX,
		  tailY;
public:
	int GetHeadX() { return this->headX; }
	int GetHeadY() { return this->headY; }
	int GetDirection() { return this->direction; }

	void SetHeadX(int x) { this->headX = x; }
	void SetHeadY(int y) { this->headY = y; }
	void SetDirection(int dir) { this->direction = dir; }
};

class Fruit {
private:
	int X, Y;
public:
	int GetX() {return this->X; }
	int GetY() {return this->Y; }

	void Set(int X, int Y) { this->X = X; this->Y = Y; }
};

class Console {
private:
	int width,
		  height;
public:
	int GetWidth() { return this->width; }
	int GetHeight() { return this->height; }

	void SetWidth(int w) { this->width = w; }
	void SetHeight(int h) { this->height = h; }
};


Snake snake;
Fruit fruit;
Console console;

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
	console.SetWidth(csbi.srWindow.Right - csbi.srWindow.Left + 1);
	console.SetHeight(csbi.srWindow.Bottom - csbi.srWindow.Top + 1);
}

// Drawing the "walls" (outside border)
void draw_window_outline(int score) {
	_setmode(_fileno(stdout), 0x20000); // For ASCII to work

	for (int i = 0; i < console.GetWidth() - 8; i++) { // Top Border (-8 compensates for "S N A K E")
		if (i == 0)
			wprintf(L"╔");
		else if (i == console.GetWidth() - 9) // -9 compensates for "S N A K E"
			wprintf(L"╗");
		else if (i == console.GetWidth() / 2)
			wprintf(L"S N A K E");
		else
			wprintf(L"═");
	}

	cursor_goto(0, console.GetHeight() - 2); // Bottom Border
	for (int i = 0; i < console.GetWidth(); i++) {
		if (i == 0)
			wprintf(L"╚");
		else if (i == console.GetWidth() - 1)
			wprintf(L"╝");
		else
			wprintf(L"═");
	}

	cursor_goto(0, 1); // Left border
	for (int i = 1; i < console.GetHeight() - 1; i++) {
		wprintf(L"║");
		cursor_goto(0, i);
	}

	cursor_goto(console.GetWidth() - 1, 1); // Right border
	for (int i = 1; i < console.GetHeight() - 1; i++) {
		wprintf(L"║");
		cursor_goto(console.GetWidth() - 1, i);
	}

	cursor_goto(1, console.GetHeight() - 1);
	wprintf(L"Score: %i", score * 10);
}

void is_key_pressed() {
	if (_kbhit() && _getch()) {
		switch (_getch()) {
		case 72: // code for arrow up
			if (snake.GetDirection() != 2) snake.SetDirection(1);
			break;

		case 80: // code for arrow down
			if (snake.GetDirection() != 1) snake.SetDirection(2);
			break;

		case 75: // code for arrow left
			if (snake.GetDirection() != 4) snake.SetDirection(3);
			break;

		case 77: // code for arrow right
			if (snake.GetDirection() != 3) snake.SetDirection(4);
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
	snake.SetDirection(rand() % 4 + 1);

	snake.SetHeadX(console.GetWidth() / 2);
	snake.SetHeadY(console.GetHeight() / 2);

	srand(time(NULL));
	/*fruit.X = rand() % (console.GetWidth() - 2) + 1;
	fruit.Y = rand() % (console.GetHeight() - 3) + 1;*/
	fruit.Set(rand() % (console.GetWidth() - 2) + 1, rand() % (console.GetHeight() - 3) + 1);
	cursor_goto(fruit.GetX(), fruit.GetY());
	wprintf(L"@");

	while (!gameOver) {
		is_key_pressed();

		switch (snake.GetDirection()) {
		case 1:
			snake.SetHeadY(snake.GetHeadY()-1); // snakeHeadY--;
			break;
		case 2:
			snake.SetHeadY(snake.GetHeadY()+1);
			break;
		case 3:
			snake.SetHeadX(snake.GetHeadX()-1);
			break;
		case 4:
			snake.SetHeadX(snake.GetHeadX()+1);
			break;
		}

		if (snake.GetHeadX() == console.GetWidth() - 1) snake.SetHeadX(1);
		if (snake.GetHeadX() == 0) snake.SetHeadX(console.GetWidth() - 2);
		if (snake.GetHeadY() == console.GetHeight() - 2) snake.SetHeadY(1);
		if (snake.GetHeadY() == 0) snake.SetHeadY(console.GetHeight() - 3);

		if (snake.GetHeadX() == fruit.GetX() && snake.GetHeadY() == fruit.GetY()) {
			wprintf(L" ");

			score++;
			cursor_goto(1, console.GetHeight() - 1);
			wprintf(L"Score: %i", score * 10);
			Sleep(50);

			srand(time(NULL));
			/*fruit.X = rand() % (console.GetWidth() - 2) + 1;
			fruit.Y = rand() % (console.GetHeight() - 3) + 1;*/
			fruit.Set(rand() % (console.GetWidth() - 2) + 1, rand() % (console.GetHeight() - 3) + 1);
			cursor_goto(fruit.GetX(), fruit.GetY());
			wprintf(L"@");
		}

		wprintf(L" ");
		cursor_goto(snake.GetHeadX(), snake.GetHeadY());
		wprintf(L"█");
		cursor_goto(snake.GetHeadX(), snake.GetHeadY());

		Sleep(50);
	}

	_setmode(_fileno(stdout), _O_TEXT); // Reset from ASCII (to fix "std::cout")
	std::cin.sync();
	pause_for_user();
	return 0;
}
