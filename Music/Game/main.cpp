
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

void musicMenu() 
{
	// 플레이리스트
	std::vector<std::string> playList = {
		"0. bad apple",
		"1. good apple"
	};

	Utils::SetPos(20, 9);
	// TODO. 한글 깨짐 해결하기
	std::cout << "Play Music!" << std::endl;

	for ( int i = 0; i < playList.size( ); ++i ) {
        Utils::SetPos(20, 10 + i);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14); // Yellow color
		std::cout << playList[ i ] << std::endl;
	}
}

#pragma region Level
void Title()
{
	Utils::renderTitle("../Asset/Title.txt", { 0, 0 });

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

    Utils::SetConsole();
    PlaySound(TEXT("../Asset/Imtr.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);

	// TODO. 정렬 설정
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