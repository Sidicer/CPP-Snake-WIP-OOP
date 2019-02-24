#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <chrono>
using namespace std;

#include <stdio.h>
#include <Windows.h>
#include <conio.h>
#include <time.h> // For srand randomization

enum COLOUR
{
	FG_BLACK = 0x0000,
	FG_DARK_BLUE = 0x0001,
	FG_DARK_GREEN = 0x0002,
	FG_DARK_CYAN = 0x0003,
	FG_DARK_RED = 0x0004,
	FG_DARK_MAGENTA = 0x0005,
	FG_DARK_YELLOW = 0x0006,
	FG_GREY = 0x0007,
	FG_DARK_GREY = 0x0008,
	FG_BLUE = 0x0009,
	FG_GREEN = 0x000A,
	FG_CYAN = 0x000B,
	FG_RED = 0x000C,
	FG_MAGENTA = 0x000D,
	FG_YELLOW = 0x000E,
	FG_WHITE = 0x000F,
	BG_BLACK = 0x0000,
	BG_DARK_BLUE = 0x0010,
	BG_DARK_GREEN = 0x0020,
	BG_DARK_CYAN = 0x0030,
	BG_DARK_RED = 0x0040,
	BG_DARK_MAGENTA = 0x0050,
	BG_DARK_YELLOW = 0x0060,
	BG_GREY = 0x0070,
	BG_DARK_GREY = 0x0080,
	BG_BLUE = 0x0090,
	BG_GREEN = 0x00A0,
	BG_CYAN = 0x00B0,
	BG_RED = 0x00C0,
	BG_MAGENTA = 0x00D0,
	BG_YELLOW = 0x00E0,
	BG_WHITE = 0x00F0,
};

enum PIXEL_TYPE
{
	PIXEL_SOLID = 0x2588,
	PIXEL_THREEQUARTERS = 0x2593,
	PIXEL_HALF = 0x2592,
	PIXEL_QUARTER = 0x2591,
};

class console {
private:
	unsigned short width,
					height;

public:
	unsigned short getWidth() { return this->width; }
	unsigned short getHeight() { return this->height; }

	void set(unsigned short width, unsigned short height) { this->width = width; this->height = height; }
	void setWidth(unsigned short width) { this->width = width; }
	void setHeight(unsigned short height) { this->height = height; }

	std::wstring consoleName = L"Sidicers Console Game";
};

class player {
private:
	int positionX,
		positionY,
		lastPositionX[100],
		lastPositionY[100],
		size = 0,
		direction;
public:
	int getPositionX() { return this->positionX; }
	int getPositionY() { return this->positionY; }

	int getLastPositionX(int i) { return this->lastPositionX[i]; }
	int getLastPositionY(int i) { return this->lastPositionY[i]; }

	int getSize() { return this->size;  }
	int getDirection() { return this->direction; }

	void set(int positionX, int positionY) { this->positionX = positionX; this->positionY = positionY; }
	void setPositionX(int positionX) { this->positionX = positionX; }
	void setPositionY(int positionY) { this->positionY = positionY; }

	void setLast(int lastPositionX[], int lastPositionY[], int i) { this->lastPositionX[i] = lastPositionX[i]; this->lastPositionY[i] = lastPositionY[i]; }
	void setLastPositionX(int lastPositionX, int i) { this->lastPositionX[i] = lastPositionX; }
	void setLastPositionY(int lastPositionY, int i) { this->lastPositionY[i] = lastPositionY; }

	void setSize(int size) { this->size = size; }
	void setDirection(int direction) { this->direction = direction; }
};

class fruit {
private:
	int X, Y;
public:
	int getX() { return this->X; }
	int getY() { return this->Y; }

	void set(int X, int Y) { this->X = X; this->Y = Y; }
};

int lastX[100],
lastY[100];

console MainConsole;
player MainPlayer;
fruit Fruit;

int main()
{
	auto tp1 = chrono::system_clock::now();
	auto tp2 = chrono::system_clock::now();

	bool GameOver(false);

	// Get Console Info (Width, Height)
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	MainConsole.set(csbi.srWindow.Right - csbi.srWindow.Left + 1, csbi.srWindow.Bottom - csbi.srWindow.Top + 1);

	// Initiate the Console buffer
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SMALL_RECT consoleInfo;
	consoleInfo = { 0, 0, MainConsole.getWidth() - 1, MainConsole.getHeight() - 1 };
	CHAR_INFO *screenBuffer = new CHAR_INFO[MainConsole.getWidth()*MainConsole.getHeight()];;
	memset(screenBuffer, 0, sizeof(CHAR_INFO) * MainConsole.getWidth() * MainConsole.getHeight());
	SetConsoleWindowInfo(hConsole, TRUE, &consoleInfo);

	// Set player position in the middle and with a random direction
	srand(time(NULL));
	MainPlayer.set(MainConsole.getWidth() / 2, MainConsole.getHeight() / 2);
	MainPlayer.setDirection(rand() % 4 + 1);

	// Place fruit in random position
	srand(time(NULL));
	Fruit.set(rand() % (MainConsole.getWidth() - 2) + 1, rand() % (MainConsole.getHeight() - 3) + 1);

	while (!GameOver)
	{
		// Tracking time passed
		tp2 = chrono::system_clock::now();
		chrono::duration<float> elapsedTime = tp2 - tp1;
		tp1 = tp2;
		float timePassed = elapsedTime.count();

		// Draw Borders
		for (int i = 0; i < MainConsole.getWidth()*MainConsole.getHeight(); i++) {
			screenBuffer[i].Char.UnicodeChar = L' ';														// Fill empty spaces with ' '
			screenBuffer[i].Attributes = BG_DARK_BLUE | FG_WHITE;											// Set the background color to DARK RED

			if (i <= MainConsole.getWidth() - 1)															// Top Border
				screenBuffer[i].Char.UnicodeChar = L'═';

			if (i >= (MainConsole.getHeight() * MainConsole.getWidth() - MainConsole.getWidth()))			// Bottom Border
				screenBuffer[i].Char.UnicodeChar = L'═';

			for (int j = 1; j < MainConsole.getHeight() - 1; j++)											// Left Border
				if (i == j * MainConsole.getWidth())
					screenBuffer[i].Char.UnicodeChar = L'║';

			for (int j = 1; j < MainConsole.getHeight() - 1; j++)											// Right Border
				if (i == j * MainConsole.getWidth() + MainConsole.getWidth() - 1)
					screenBuffer[i].Char.UnicodeChar = L'║';

			if (i == 0)																						// Top Left Corner
				screenBuffer[i].Char.UnicodeChar = L'╔';
			if (i == MainConsole.getWidth() - 1)															// Top Right Corner
				screenBuffer[i].Char.UnicodeChar = L'╗';
			if (i == (MainConsole.getHeight() - 1) * MainConsole.getWidth())								// Bottom Left Corner
				screenBuffer[i].Char.UnicodeChar = L'╚';
			if (i == (MainConsole.getHeight() - 1) * MainConsole.getWidth() + MainConsole.getWidth() - 1)	// Bottom Right Corner
				screenBuffer[i].Char.UnicodeChar = L'╝';
		}

		// Change the direction with keyboard presses
		if (_kbhit() && _getch()) {
			switch (_getch()) {
				case 72: if (MainPlayer.getDirection() != 2) MainPlayer.setDirection(1); // code for arrow up
				break;
				case 80: if (MainPlayer.getDirection() != 1) MainPlayer.setDirection(2); // code for arrow down
				break;
				case 75: if (MainPlayer.getDirection() != 4) MainPlayer.setDirection(3);// code for arrow left
				break;
				case 77: if (MainPlayer.getDirection() != 3) MainPlayer.setDirection(4);// code for arrow right
				break;
			}
		}

		// Move player by the direction that it's facing
		switch (MainPlayer.getDirection()) {
			case 1: MainPlayer.setPositionY(MainPlayer.getPositionY() - 1);
			break;
			case 2: MainPlayer.setPositionY(MainPlayer.getPositionY() + 1);
			break;
			case 3: MainPlayer.setPositionX(MainPlayer.getPositionX() - 1);
			break;
			case 4: MainPlayer.setPositionX(MainPlayer.getPositionX() + 1);
			break;
		}

		// If border is touched - move to the opposite side
		if (MainPlayer.getPositionX() == MainConsole.getWidth() - 1) MainPlayer.setPositionX(1);
		if (MainPlayer.getPositionX() == 0) MainPlayer.setPositionX(MainConsole.getWidth() - 2);
		if (MainPlayer.getPositionY() == MainConsole.getHeight() - 1) MainPlayer.setPositionY(2);
		if (MainPlayer.getPositionY() == 0) MainPlayer.setPositionY(MainConsole.getHeight() - 3);

		// If fruit is "touched"
		if (MainPlayer.getPositionX() == Fruit.getX() && MainPlayer.getPositionY() == Fruit.getY()) {
			MainPlayer.setSize(MainPlayer.getSize() + 1);
			// Place fruit in random position
			srand(time(NULL));
			Fruit.set(rand() % (MainConsole.getWidth() - 2) + 1, rand() % (MainConsole.getHeight() - 3) + 1);
		}

		if (MainPlayer.getSize() > 0) {
			for (int i = 0; i < MainPlayer.getSize(); i++) {
				if (MainPlayer.getSize() > 1) {
					lastX[i] = lastX[i-1];
					lastY[i] = lastY[i-1];
				}
				else {
					lastX[i] = MainPlayer.getPositionX();
					lastY[i] = MainPlayer.getPositionY();
				}
			}
		}

		// Draw SNAKE Title
		string Text = "S N A K E";
		for (int i = 0; i < Text.size(); i++)  {
			screenBuffer[0 * MainConsole.getWidth() + 1 + i].Char.UnicodeChar = Text[i];
			screenBuffer[0 * MainConsole.getWidth() + 1 + i].Attributes = FG_WHITE | BG_DARK_BLUE;
		}

		// Draw Fruit
		screenBuffer[Fruit.getY() * MainConsole.getWidth() + Fruit.getX()].Char.UnicodeChar = L'■';
		screenBuffer[Fruit.getY() * MainConsole.getWidth() + Fruit.getX()].Attributes = FG_RED | BG_DARK_BLUE;

		// Draw Player
		screenBuffer[MainPlayer.getPositionY() * MainConsole.getWidth() + MainPlayer.getPositionX()].Char.UnicodeChar = PIXEL_SOLID;
		screenBuffer[MainPlayer.getPositionY() * MainConsole.getWidth() + MainPlayer.getPositionX()].Attributes = FG_DARK_GREY;

		if (MainPlayer.getSize() > 0) {
			for (int i = 0; i < MainPlayer.getSize(); i++) {
				// Draw Player
				screenBuffer[lastY[i] * MainConsole.getWidth() + lastX[i]].Char.UnicodeChar = L'O';
				screenBuffer[lastY[i] * MainConsole.getWidth() + lastX[i]].Attributes = FG_DARK_GREY;
			}
		}

		// Update screen and console info
		wchar_t s[256];
		swprintf_s(s, 256, L"%s [Score: %i] [FPS: %3.2f] ", MainConsole.consoleName.c_str(), MainPlayer.getSize() * 10, 1.0f / timePassed);
		SetConsoleTitle(s);
		WriteConsoleOutput(hConsole, screenBuffer, { (short)MainConsole.getWidth(), (short)MainConsole.getHeight() }, { 0,0 }, &consoleInfo);

		Sleep(50);
	}
	return 0;
}
