#include "Game.h"

void Game::parse(int ix)
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

	float delay;   // 첫 노트 전까지의 delay (in ms)
	int bpm;

	file >> delay >> bpm >> noteCount;

	// 배열 공간 할당
	notes.resize(noteCount); // 액터
	noteLane.resize(noteCount); // 그리드 위치 (행)
	noteInterval.resize(noteCount); // 시간 간격, Time between note and next note

	timing.resize(noteCount);
	pressed.resize(noteCount, false);

	// 그리드 열에 맞게 액터 초기화
	for (int i = 0, tmp; i < noteCount; i++)
	{
		file >> tmp >> noteLane.at(i) >> noteInterval.at(i);

		if (noteLane.at(i) == 1)
		{
			notes.at(i) = 'd'; // [] 연산자 대신 안전하게 접근
		}
		else if (noteLane.at(i) == 2)
		{
			notes.at(i) = 'f';
		}
		else if (noteLane.at(i) == 3)
		{
			notes.at(i) = ' ';
		}
		else if (noteLane.at(i) == 4)
		{
			notes.at(i) = 'j';
		}
		else if (noteLane.at(i) == 5)
		{
			notes.at(i) = 'k';
		}
	}
	file.close();

	// 타이밍 계산 (delay/Time between note and next note)
	// 1. 첫 번째 노트의 타이밍은 delay
	timing.at(0) = delay;

	for (int i = 1; i < noteCount; i++)
	{
		// 2. 이후 노트의 타이밍은 이전 노트의 타이밍 + 이전 노트의 시간 간격
		timing.at(i) =
			timing.at(i - 1) + noteInterval.at(i - 1);
	}
#pragma endregion

	// 음악 재생 초기화 (↔ playground)
	if (ix == 0)
	{
		MCIERROR err = mciSendStringW(
			L"open \"..\\Asset\\drift phonk.mp3\" type mpegvideo alias music",
			nullptr, 0, nullptr);
		if (err) {
			wchar_t buf[128];
			mciGetErrorStringW(err, buf, 128);
			std::wcerr << L"MCI open error: " << buf << std::endl;
			return;
		}
	}
	else
	{
		MCIERROR err = mciSendStringW(
			L"open \"..\\Asset\\ievanpolkka.mp3\" type mpegvideo alias music",
			nullptr, 0, nullptr);
		if (err) {
			wchar_t buf[128];
			mciGetErrorStringW(err, buf, 128);
			std::wcerr << L"MCI open error: " << buf << std::endl;
			return;
		}
	}

	// hit 사운드 열기
	MCIERROR err = mciSendStringW(
		L"open \"..\\Asset\\SFX\\hit.wav\" type waveaudio alias hit",
		nullptr, 0, nullptr);
	if (err) {
		wchar_t errBuf[128];
		mciGetErrorStringW(err, errBuf, 128);
		std::wcerr << L"Failed to open hit.wav: " << errBuf << std::endl;
	}

	// miss 사운드 열기
	err = mciSendStringW(
		L"open \"..\\Asset\\SFX\\miss.mp3\" type mpegvideo alias miss",
		nullptr, 0, nullptr);
	if (err) {
		wchar_t errBuf[128];
		mciGetErrorStringW(err, errBuf, 128);
		std::wcerr << L"Failed to open miss.mp3: " << errBuf << std::endl;
	}
}

void Game::play()
{
#pragma region UI
	drawGrid();
	baseNote();
#pragma endregion

#pragma region InGame
	unsigned long int currentTime = 0;
	char input = '\0';
	int frameIndex = 0; // 프레임당 요구하는 노트의 인덱스
	int nextFrameIndex = 0;
	int score = 0;

	mciSendString(L"play music", 0, 0, 0); // MCI 장치, command to play the music
	clock_t start = clock();			   // 현재 시작 → 딱 음악이 플레이됐을 때

	// TODO. 테스트로 30초 동안 플레이
	// while (currentTime < timing.back() + 2000) // 마지막 노드로부터 2초까지
	while (currentTime < 30000)
	{
		// 1. 업데이트
		// 1. 시계 택, 프로그램 시작 이후 흘러간 틱 수 : 현재 -  시작 간격
		// 2. 틱을 초 단위로 변환					   : 시간 간격 * 1000
		// 3. 초를 밀리초(ms) 단위로 변환			   
		currentTime = 
			static_cast<unsigned long>(1000 * (clock() - start) 
				/ (double)CLOCKS_PER_SEC);

		// 2. 인풋
		input = getInput(input);

		// 3. 플레이
		// 현재에서 노트 최대 50개만 플레이
		/*
			시간 복잡도 O(50)으로 프레임을 일정하기 유지
			프레임 1 에는 “0~49” (50개)
			프레임 2 에는 “10~59” (50개)
			프레임 3 에는 “50~99” (50개)
			프레임 4 이후에는 남은 노트가 40개면 40개만
		*/

		for (int i = frameIndex; i < min(frameIndex + 50, noteCount); ++i)
		{
			// 노트와 입력 횟수의 상관 관계 → 노트 하나 당 한번 누르게
			if (pressed.at(i))
			{
				continue;
			}

			// 1)노트 안 놓침
			if (currentTime < timing.at(i) + errorTime)
			{
				// 입력 처리
				/*
				(timing at(i) - errorTime)직전부터 (timing at(i) + errorTime)까지
				*/
				if (currentTime >= timing.at(i) - errorTime)
				{
					// 노트 액터와 같은 키를 누른 경우
					if (input && notes.at(i) == input)
					{
						mciSendString(L"stop hit", 0, 0, 0);
						mciSendString(L"play hit from 0", 0, 0, 0); //Play the hit sound

						score++;			  // 맞춘 개수
						pressed.at(i) = true; // 

						nextFrameIndex = max(0, i - 5); // 다음 프레임에 검사 시작 위치를 잘못 옮기면 안돼서,
						// 최대 5개 이전부터 검사 시작	+ pressed로 진짜 hit한 노트는 건너띔.
					}
				}

				// 노트 빠르게 그리기
				/*
					(timing at(i) - 1초)직전부터 (timing at(i) + errorTime)까지
				*/
				if (currentTime > timing.at(i) - 1000)
				{
					int x = noteLane.at(i) * 5 - 2; // 그리드 위치, 화면 가로(0~27) 좌표 중 3, 8, 13, 18, 23 지점이 각 레인의 중심

					int step = static_cast<int>((timing.at(i) - currentTime) / 50); // 정확한 시간까지 남은 시간 (in ms), 50ms마다 1픽셀씩 움직인다고 가정
					int y = 23 - max(step, 0);

					//////////////////////
					// ★ 프레임마다 갱신
					clearNote(x, y);
					drawNote(x, y, noteLane.at(i));
				}
			}
			// 2) 노트 놓침
			else
			{
				mciSendString(L"stop miss", 0, 0, 0);
				mciSendString(L"play miss from 0", 0, 0, 0);

				pressed.at(i) = true;

				nextFrameIndex = max(0, i - 5);
			}

			// 3) 하단 그리드 및 베이스 노트 갱신
			Utils::printStr(14, 1, 21, "==========================");
		}

		// 4. UI 갱신
		baseNote();
		Utils::SetPos(13, 27);
		std::string scoreText = "Score: " + std::to_string(score) + "  ";
		Utils::printStr(0x1e, 30, 1, scoreText);

		// 5. 프레임 인덱스 갱신
		frameIndex = nextFrameIndex;
	}
#pragma endregion

#pragma region TODO. GameOver
	system("cls");
	system("Color 3f");
	std::cout << std::endl; // 줄 바꿈 + 버퍼 비움

	Utils::PrintCenter(0x17, 28, "Score: " + std::to_string(score));
	Sleep(1000);

	if (score == noteCount)
	{
		Utils::PrintCenter(0x1f, 28, "Rank SSS");
	}
	else if (score == noteCount * 0.8)
	{
		Utils::PrintCenter(0x1e, 28, "Rank S");
	}
	else if (score == noteCount * 0.6)
	{
		Utils::PrintCenter(0x1a, 28, "Rank A");
	}
	else if (score == noteCount * 0.5)
	{
		Utils::PrintCenter(0x19, 28, "Rank B");
	}
	else
	{
		Utils::PrintCenter(0x15, 28, "Rank C");
	}

	// MCI 장치, command
	mciSendString(L"stop music", 0, 0, 0);
	mciSendString(L"close music", 0, 0, 0);
	mciSendString(L"close hit", 0, 0, 0);
	mciSendString(L"close miss", 0, 0, 0);

	// 아무키나 누르면 종료
	_getch();
#pragma endregion
}

/// <summary>
/// for play
/// </summay>

void Game::drawGrid()
{
	system("cls");

	// 상
	Utils::printStr(14, 1, 0, "==========================");

	// 하
	Utils::printStr(14, 1, 21, "==========================");

	// 좌우
	for (int x = 0; x < 6; x++)
	{
		for (int y = 1; y < 25; y++)
		{
			if ((x == 0) || (x == 5))
			{
				Utils::printStr(14, x * 5 + 1, y, u8"\u2551"); // ║ 외부
			}
			else
			{
				Utils::printStr(14, x * 5 + 1, y, u8"\u2502"); // │ 내부
			}
		}
	}
}

// 판정 노트 (하단 그리드 아래 노트)
void Game::baseNote() 
{
	for (int i = 0; i < 5; ++i)
	{
		int color = 0x07;
		if (i == 0 || i == 4)
			color = 0x1f; //White
		else if (i == 1 || i == 3)
			color = 0x17; //Light grey
		else if (i == 2)
			color = 0x18; //Dark grey

		Utils::printStr(8, 3 + i * 5, 22, "**");
		Utils::printStr(8, 2 + i * 5, 23, "****");
		Utils::printStr(8, 3 + i * 5, 24, "**");
	}
}

// ☆ 노트의 정 가운데 위치를 기준으로 블록 그리기
void Game::drawNote(int x, int y, int lane)
{
	int color = 6;
	if (lane == 1)
		color = 13; //Magenta
	else if (lane == 2)
		color = 3; //Cyan
	else if (lane == 3) 
		color = 4; //Red
	else if (lane == 4)
		color = 10; //Green
	else if (lane == 5)
		color = 15; //White
	
	// 판정 위치에 따라 강도 다르게
	if (y == 21)
	{
		Utils::printStr(color + 8, x, y - 1, "**");
		Utils::printStr(color + 8, x - 1, y, "****");
		Utils::printStr(color + 8, x, y + 1, "**");
		
		
	}
	else
	{
		Utils::printStr(color, x, y - 1, u8"\u2588\u2588");
		Utils::printStr(color, x - 1, y, u8"\u2588\u2588\u2588\u2588");
		Utils::printStr(color, x, y + 1, u8"\u2588\u2588");
	}
}

void Game::clearNote(int x, int y)
{
	// ☆ 맨 아래 노트의 위치를 기준으로, 아래에서 위로 블록 지우기
	Utils::printStr(0, x, y - 3, "  "); 
	Utils::printStr(0, x - 1, y - 2, "    ");
	Utils::printStr(0, x - 1, y - 1, "    ");
	Utils::printStr(0, x, y, "  ");
}

char Game::getInput(char input)
{
	// 입력 대기
	if (_kbhit())
	{
		char ch = _getch();

		if (input != ch)
		{
			return ch;
		}
	}
	// 입력 대기(x)나 입력이 같으면!!
	return '\0';
}