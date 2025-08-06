
#include "Game.h"
#include <Windows.h>
#include <iostream>
#include <vector>
#include <string>

/*
TODO. #include <conio.h>의 Sleep(200) 실험해보기
*/

#pragma region Utils
void SetConsole()
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

std::vector<int> GetConsoleSize()
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    int width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    return { width, height };
}

std::vector<int> GetMiddlePosition()
{
    std::vector<int> consoleSize = GetConsoleSize();
    int size = 0; // actor든, str든 
    int x = (consoleSize[0] - size) / 2;
    int y = consoleSize[1] / 2;
    return { x, y };
}

void SetMiddlePosition(int y)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    std::vector<int> mPos = GetMiddlePosition();

    COORD pos;
    pos.X = mPos[0];
    pos.Y = mPos[1] + y;

    SetConsoleCursorPosition(hConsole, pos);
}

void SetMiddlePosition(int x, int y)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    std::vector<int> mPos = GetMiddlePosition();

    COORD pos;
    pos.X = mPos[0] + x;
    pos.Y = mPos[1] + y;

    SetConsoleCursorPosition(hConsole, pos);
}

void SetConsoleSize(int width, int height)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	int w = static_cast<int>(width);
	int h = static_cast<int>(height);
    SMALL_RECT windowSize = { 0, 0, w, h };
    SetConsoleWindowInfo(hConsole, TRUE, &windowSize);
}

#pragma endregion

#pragma region Level
void Intro()
{
    SetConsoleSize(45, 20);
    // 기본 배경 및 글씨 색 설정
    system("Color f9");
}
#pragma endregion

#pragma region Menu
void miniMenu()
{
    // 기본 설정
    SetConsoleSize(20, 10);
    system("Color E9");

    // 문구 출력
    SetMiddlePosition(0);
    std::cout << "1. 게임 시작" << std::endl;
    //SetMiddlePosition();
    //std::cout << "2. " << std::endl;
    SetMiddlePosition(1);
    std::cout << "0. 메뉴 끄기" << std::endl;
}
#pragma endregion

void PlayMusic() 
{
    std::vector<std::string> playList = {
        "1. bad apple",
        "2. good apple"
    };

    for (int i = 0; i < playList.size(); i++)
    {
        SetMiddlePosition(i);
        std::cout << playList[i] << std::endl;
	}   

    char ch;
    ch = getchar();

	int musicIndex = ch - '0';
	musicIndex = ((musicIndex < 1) || (musicIndex > 2)) ? 1 : musicIndex;
    
    // 2. Game 시작
    ///////////////////////////////////
    Game game;
    game.prase(musicIndex);
}

int main() {

    SetConsole();

    // 1) intro
    Intro();

    char ch;
    do 
    {
        // 2) miniMenu
        miniMenu();
        ch = getchar();

#pragma region reset
        system("cls");
        system("Color f9");
#pragma endregion

        if (ch == '1') 
        {
            PlayMusic();
        }
        /*
        else 
        {
        }
        */

    } while (ch != '1');


    std::cin.get();
    return 0;
}