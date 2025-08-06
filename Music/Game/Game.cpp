#include "Game.h"

void Game::prase(int ix)
{
#pragma region Info
	// 음악 정보(노드/bmp 등) 받아오기
	std::vector<std::string> info = {
		"../Asset/badapple.txt",
		"../Asset/ievanpolkka.txt"
	};

	std::string filePath1 = info[ix];

	std::ifstream file;
	file.open(filePath1);

	if (!file.is_open())
	{
		std::cout << "Failed to open file: " << filePath1 << std::endl;
		return;
	}

	std::string title;
	getline(file, title);

	float delay;
	int bpm;
	int nodeCount;

	file >> delay >> bpm >> nodeCount;

	// 배열 공간 할당
	notes.resize(nodeCount); // 액터
	noteRow.resize(nodeCount); // 그리드 위치
	noteLength.resize(nodeCount); // 시간 간격, Time between note and next note

	timing.resize(nodeCount);
	pressed.resize(nodeCount, false);

	// 액터 생성. W, A, S, D, K
	for (int i = 0, tmp; i < nodeCount; i++)
	{
		file >> tmp >> noteRow.at(i) >> noteLength.at(i);

		if (noteRow.at(i) == 1)
		{
			notes.at(i) = 'W'; // [] 연산자 대신 안전하게 접근
		}
		else if (noteRow.at(i) == 2)
		{
			notes.at(i) = 'A';
		}
		else if (noteRow.at(i) == 3)
		{
			notes.at(i) = 'S';
		}
		else if (noteRow.at(i) == 4)
		{
			notes.at(i) = 'D';
		}
		else if (noteRow.at(i) == 5)
		{
			notes.at(i) = 'K';
		}
	}
	file.close();

	// 타이밍 계산 (delay/Time between note and next note)
	// 1. 첫 번째 노트의 타이밍은 delay
	timing.at(0) = delay;

	for (int i = 1; i < nodeCount; i++)
	{
		// 2. 이후 노트의 타이밍은 이전 노트의 타이밍 + 이전 노트의 시간 간격
		timing.at(i) =
			timing.at(i - 1) + noteLength.at(i - 1);
	}
#pragma endregion

	// 음악 파일 받아오기
	std::vector<std::string> musicCMD = {
		"open Asset\\badapple.mp3 alias music",
		"open Asset\\ievanpolkka.mp3 alias music"
	};

	// 음악 재생 (↔ playground)
	(ix == 1) ?
		mciSendString(L"open Asset\\badapple.mp3 alias music", 0, 0, 0) :
		mciSendString(L"open Asset\\ievanpolkka.mp3 alias music", 0, 0, 0);

	// bgm
	mciSendString(L"open SFX\\hit.wav alias hit", 0, 0, 0);
	mciSendString(L"open SFX\\miss.mp3 alias miss", 0, 0, 0);
}

void Game::play()
{
	
}

/// <summary>
/// for play
/// </summay>

void Game::drawGrid()
{
	system("cls");
	system("Color 17");
	///////////////////
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SMALL_RECT windowSize = { 0, 0, 28, 30 };
	SetConsoleWindowInfo(hConsole, TRUE, &windowSize);
	///////////////////

#pragma region Grid
	// 상
	COORD pos1 = { 1, 0 };
	SetConsoleCursorPosition(hConsole, pos1);
	WORD word1 = 7;
	SetConsoleTextAttribute(hConsole, word1);
	std::cout << "\x00c9\x00cd\x00cd\x00cd\x00cd\x00d1\x00cd\x00cd\x00cd\x00cd\x00d1\x00cd\x00cd"
		"\x00cd\x00cd\x00d1\x00cd\x00cd\x00cd\x00cd\x00d1\x00cd\x00cd\x00cd\x00cd\x00bb" << std::endl; // 16진수 값으로 Grid 그리기
	
	// 하
	COORD pos2 = { 1, 25};
	SetConsoleCursorPosition(hConsole, pos2);
	WORD word2 = 7;
	SetConsoleTextAttribute(hConsole, word2);
	std::cout << "\x00c8\x00cd\x00cd\x00cd\x00cd\x00cf\x00cd\x00cd\x00cd\x00cd\x00cf\x00cd\x00cd"
		"\x00cd\x00cd\x00cf\x00cd\x00cd\x00cd\x00cd\x00cf\x00cd\x00cd\x00cd\x00cd\x00bc" << std::endl;

	// 좌우
	for (int x = 0; x < 6; x++)
	{
		for (int y = 1; y < 25; y++)
		{
			std::vector<int> pos = { x * 5 + 1, y };
			COORD coord;
			coord.X = pos[0];
			coord.Y = pos[1];
			SetConsoleCursorPosition(hConsole, coord);

			if (x == 0 || x == 5)
			{
				std::cout << "\x00ba";
			}
			else
			{
				std::cout << "\x00b3";
			}
		}
	}
#pragma endregion
}

char Game::getInput(char input)
{
	// 입력 대기
	if (_kbhit()) 
	{
		char ch = getchar();

		if (input != ch) 
		{
			return ch;
		}
	}
	// 입력 대기(x)나 입력이 같으면!!
	return '\0'; 
}

void Game::drawNote(int x, int y, int row)
{
}

void Game::clearNote(int x, int y)
{
}
