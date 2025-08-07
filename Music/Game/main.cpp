
#include "Game.h"
#include "Color.h"
#include "Utils.h"
#include <Windows.h>
#include <iostream>
#include <vector>
#include <string>
#include <io.h>
#include <fcntl.h>

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

void SetWindowSize(int width, int height)
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
    {
        // Make sure the new size isn't too big
        if (height > csbi.dwSize.Y) height = csbi.dwSize.Y;
        if (width > csbi.dwSize.X) width = csbi.dwSize.X;

        // Adjust window origin if necessary
        if ((csbi.srWindow.Top + height) > csbi.dwSize.Y) csbi.srWindow.Top = csbi.dwSize.Y - height - 1;
        if ((csbi.srWindow.Left + width) > csbi.dwSize.Y) csbi.srWindow.Left = csbi.dwSize.X - width - 1;

        // Calculate new size
        csbi.srWindow.Bottom = csbi.srWindow.Top + height - 1;
        csbi.srWindow.Right = csbi.srWindow.Left + width - 1;

        SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), true, &csbi.srWindow);
    }
}

void SetCursorPosition(int x, int y) 
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void RenderText(std::string filePath, std::vector<int> cursorPos)
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
#pragma endregion

void musicMenu() 
{
	// 플레이리스트
	std::vector<std::string> playList = {
		"0. bad apple",
		"1. good apple"
	};

	SetCursorPosition(20, 9);
	// TODO. 한글 깨짐 해결하기
	std::cout << "Play Music!" << std::endl;

	for ( int i = 0; i < playList.size( ); ++i ) {
		SetCursorPosition(20, 10 + i);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14); // Yellow color
		std::cout << playList[ i ] << std::endl;
	}
}

#pragma region Level
void Title()
{
	RenderText("../Asset/Title.txt", { 0, 0 });

    for (int i = 0; i < 10; i++)
    {
        Sleep(100);
        system("Color 06");
        Sleep(100);
        system("Color 09");
        Sleep(100);
        system("Color 0D");

        // TODO. 제목이 깜빡이는 동안, 게임 세계관 출력
    }

    // 부 제목
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 20, 10 });
    std::cout << "Star Wars..." << std::endl;

    // 메뉴 출력
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 20, 14 });
    std::cout << "0. Game Start" << std::endl;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 20, 15 });
    std::cout << "1. Exit Game" << std::endl;
}

void PlayMusic( )
{
	system("cls");
	Utils::SetConsoleSize(80, 25);
	musicMenu( );
	
	char ch;

	do {
		ch = _getch( );
		PlaySound(NULL, NULL, 0);

		Game game;
		int ix = ( ch == '0' || ch == '1' ) ? ( ch - '0' ) : 0;

		game.parse(ix);   
		game.play( );

		musicMenu( );
	} while ( ch == '0' || ch == '1' );

	// exit back to main menu
}
#pragma endregion

int main() {

    // TODO. 콘솔 창 설정
    SetConsole();
	// TODO. 16으로 
    //SetWindowSize(80, 25);
	SetConsoleOutputCP(CP_UTF8);
	//_setmode(_fileno(stdout), _O_U8TEXT);
    

    PlaySound(TEXT("../Asset/Imtr.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);

	// TODO. 정렬 설정
	// 1) Title
    Title();

    char ch;
    do 
    {
        ch = getchar();
        std::cin.ignore();

        if (ch == '0') 
        {
            PlayMusic();
        }
        else 
        {
            system("cls");
            system("Color 0D");
            std::cout << "Good Bye! " << std::endl;
			std::cout << "Press any Key to exit..." << std::endl;

            _getch();
        }
    } while (ch == '0');

    return 0;
}