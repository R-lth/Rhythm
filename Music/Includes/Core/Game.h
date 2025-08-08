#pragma once

#include "Utils.h"
#include <string>
// 입력
#include < conio.h >
// 음악 #include <Windows.h> 
#include <mmsystem.h>  
#pragma comment(lib, "winmm.lib")

class Game 
{
public : 
	Game() = default;
	~Game() = default;

public:
	void parse(int ix);
	void play();

private:
#pragma region for play
	void drawGrid();
	void baseNote();
	void drawNote(int x, int y, int row);
	void clearNote(int x, int y);
	char getInput(char oldinput);
#pragma endregion
	const int errorTime = 150; // 오차 시간
	int noteCount = 0;		   // 총 노트 개수

	std::vector<char>  notes;       
	std::vector<int>   noteLane; // 그리드 열
	std::vector<float> noteInterval;  
	std::vector<float> timing;   // (in ms)       
	std::vector<bool>  pressed;   
};