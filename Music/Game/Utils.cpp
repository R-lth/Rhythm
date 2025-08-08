#include "Utils.h"

void Utils::SetConsole()
{
	// 콘솔 창 이름
	SetConsoleTitle(L"G- 11, I will be in your future");

	// 콘솔 창 크기 설정
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD newSize = { 80, 25 };
	SetConsoleScreenBufferSize(hConsole, newSize);

	// 콘솔 커서 끄기.
	CONSOLE_CURSOR_INFO info;
	info.bVisible = false;
	info.dwSize = 1;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);

	// UTF-8 인코딩 설정
	SetConsoleOutputCP(CP_UTF8);
}

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

void Utils::renderTitle(std::string filePath, std::vector<int> cursorPos)
{
	std::ifstream file;
	file.open(filePath);

	if (!file.is_open())
	{
		std::cout << "Failed to open file: " << filePath << std::endl;
		return;
	}

	std::stringstream stream;
	stream << file.rdbuf();
	file.close();

	std::string str = stream.str();
	std::vector<int> movePos = cursorPos;

	for (char ch : str)
	{
		if (ch == '\n')
		{
			movePos[0] = cursorPos[0];
			++movePos[1];
			continue;
		}

		static HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
		COORD coord;
		coord.X = static_cast<short>(movePos[0]);
		coord.Y = static_cast<short>(movePos[1]);
		SetConsoleCursorPosition(handle, coord);
		std::cout << ch;
		++movePos[0];
	}
}

void Utils::printStr(int color, int x, int y, std::string text)
{
	SetPos(x, y);
	SetColor(color);
	std::cout << text;

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

