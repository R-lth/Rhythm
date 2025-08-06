#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
// ют╥б
#include < conio.h >
// ю╫╬г
#include <Windows.h> 
#include <mmsystem.h>  
#pragma comment(lib, "winmm.lib")

class Game 
{
public : 
	Game() = default;
	~Game() = default;

public:
	void prase(int ix);
	void play();

private:
#pragma region for play
	void drawGrid();
	char getInput(char oldinput);
	void drawNote(int x, int y, int row);
	void clearNote(int x, int y);
#pragma endregion

	std::vector<char>  notes;       
	std::vector<int>   noteRow;     
	std::vector<float> noteLength;  
	std::vector<float> timing;       
	std::vector<bool>  pressed;   
};