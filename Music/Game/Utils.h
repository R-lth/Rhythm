#pragma once

#include <Windows.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

namespace Utils
{
	void SetConsole();
	void SetConsoleSize(int width, int height);
	void SetPos(int x, int y);
	void SetColor(int color);
	void renderTitle(std::string filePath, std::vector<int> cursorPos);
	void printStr(int color, int x, int y, std::string text);
	void PrintCenter(int width, int color, std::string text);
}