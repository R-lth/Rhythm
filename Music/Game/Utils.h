#pragma once

#include <Windows.h>
#include <iostream>

namespace Utils
{
	void SetConsoleSize(int width, int height);
	void SetPos(int x, int y);
	void SetColor(int color);
	void printStr(int color, int x, int y, std::string text);
	void PrintCenter(int width, int color, std::string text);
}