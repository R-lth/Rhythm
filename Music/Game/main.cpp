
#include "Game.h"
#include <Windows.h>
#include <iostream>
#include <vector>
#include <string>

/*
TODO. #include <conio.h>�� Sleep(200) �����غ���
*/

#pragma region Utils
void SetConsole()
{
    // �ܼ� â �̸�
    SetConsoleTitle(L"G- 11, I will be in your future");

    // �ܼ� â ũ�� ����
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD newSize = { 80, 25 };
    SetConsoleScreenBufferSize(hConsole, newSize);

    // �ܼ� Ŀ�� ����.
    CONSOLE_CURSOR_INFO info;
    info.bVisible = false;
    info.dwSize = 1;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);

    // UTF-8 ���ڵ� ����
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
    int size = 0; // actor��, str�� 
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
    // �⺻ ��� �� �۾� �� ����
    system("Color f9");
}
#pragma endregion

#pragma region Menu
void miniMenu()
{
    // �⺻ ����
    SetConsoleSize(20, 10);
    system("Color E9");

    // ���� ���
    SetMiddlePosition(0);
    std::cout << "1. ���� ����" << std::endl;
    //SetMiddlePosition();
    //std::cout << "2. " << std::endl;
    SetMiddlePosition(1);
    std::cout << "0. �޴� ����" << std::endl;
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
    
    // 2. Game ����
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