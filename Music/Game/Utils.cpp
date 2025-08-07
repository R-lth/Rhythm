#include "Utils.h"

void Utils::SetConsoleSize(int width, int height)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	short w = static_cast<short>(width);
	short h = static_cast<short>(height);
	SMALL_RECT windowSize = { 0, 0, w, h };
	SetConsoleWindowInfo(hConsole, TRUE, &windowSize);
}

void Utils::SetPos(int x, int y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void Utils::SetColor(int color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void Utils::printStr(int color, int x, int y, std::string text)
{
	SetPos(x, y);
	SetColor(color);
	std::cout << text;

	// TODO. 리셋을 해야 지워지는 이유
	SetColor(7); 
}

void Utils::PrintCenter(int width, int color, std::string text)
{
	CONSOLE_SCREEN_BUFFER_INFO SBInfo;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &SBInfo);

	text += "\n";
	int len = static_cast<int>(text.length());

	COORD coord;
	coord.X = (width - len) / 2;
	coord.Y = SBInfo.dwCursorPosition.Y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
	std::cout << text;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}

