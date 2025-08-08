
#include "Game.h"
#include "Color.h"
#include "Utils.h"
#include <string>
#include <io.h>
#include <fcntl.h>

/*
// 전체 개발 프로세스 
TODO. ★ 1. Game 구현
TODO. ★ 2. Game 구현한 것을 Engine로 분리
*/

void musicMenu() 
{
	// 플레이리스트
	std::vector<std::string> playList = {
		"0. bad apple",
		"1. good apple"
	};

	Utils::SetPos(20, 9);
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

void BeginPlay() 
{
}

void Tick(float deltaTime) 
{
}

void Render() 
{
}

int main() {

    //Utils::SetConsole();
    //PlaySound(TEXT("../Asset/Imtr.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	/* 
    Title();

    char ch;
    do 
    {
        ch = _getch();

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
    } while (ch == '0');*/

    // fPS
	float frames = 60.0f; 
    float fps = 1/ frames;

    // delta time
	LARGE_INTEGER frequency, prevTime, curTime;
	QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&prevTime);
	curTime = prevTime;

    // 게임 루프 생성
    while (true) 
    {
        // 게임 루프 종료 조건
        if (true) 
        {
        }

        // 매 프레임마다 실행되는 코드
        QueryPerformanceCounter(&curTime);
		float deltaTime = 
            (curTime.QuadPart - prevTime.QuadPart) / (float)frequency.QuadPart;

        if (deltaTime > fps) 
        {
            // TODO. 게임 루프에 현재 게임 로직 반영하기
            BeginPlay();
			Tick(deltaTime);
            Render();
        }
    }
    
    return 0;
}