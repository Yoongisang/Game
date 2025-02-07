#include <stdio.h>
#include <Windows.h>
#include <stdlib.h>
#include <time.h>

#pragma region define
#define speed 3
#define BufferWidth 500
#pragma endregion

#pragma region DoubleBuffer
#define BufferHeight 180

HANDLE hBuffer[2];
int screenIndex;

void InitBuffer();
void FlipBuffer();
void ClearBuffer();
void WriteBuffer(int x, int y, const char* shape, int color);
void CloseBuffer();

#pragma endregion

#pragma region Enum
enum Color
{
	BLACK,
	BLUE,
	GREEN,
	CYAN,
	RED,
	MAGENTA,
	BROWN,
	LIGHTGRAY,
	DARKGRAY,
	LIGHTBLUE,
	LIGHTGREEN,
	LIGHTCYAN,
	LIGHTRED,
	LIGHTMAGENTA,
	YELLOW,
	WHITE,
};
enum Dir
{
	RIGHT,
	LEFT
	
};
#pragma endregion

#pragma region Windows
void SetPosition(int x, int y);
void setColor(int color);
#pragma endregion

#pragma region Charactor

void PrintCharactor();

char character[2][16][16] =
{
	{
		//right
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0},
		{0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0},
		{0,0,0,0,1,2,2,2,2,2,2,1,0,0,0,0},
		{0,0,0,2,2,2,2,2,2,2,2,2,0,0,0,0},
		{0,0,2,2,2,2,2,2,2,2,2,2,2,0,0,0},
		{0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0},
		{0,0,3,3,3,3,3,3,3,3,3,3,3,3,0,0},
		{0,0,3,3,3,3,3,3,3,3,3,3,3,3,0,0},
		{0,0,4,4,4,4,4,4,4,4,4,4,4,4,0,0},
		{0,0,4,4,4,4,4,4,4,4,4,4,4,4,0,0},
		{0,0,4,4,4,4,4,4,4,4,4,4,4,4,0,0},
		{0,0,4,4,4,4,4,4,4,4,4,4,4,4,0,0},
		{0,0,5,5,5,5,0,0,0,0,5,5,5,5,0,0},
		{0,0,5,5,5,5,0,0,0,0,5,5,5,5,0,0}
	},

	{
		//left
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0},
		{0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0},
		{0,0,0,0,1,2,2,2,2,2,2,1,0,0,0,0},
		{0,0,0,0,2,2,2,2,2,2,2,2,2,0,0,0},
		{0,0,0,2,2,2,2,2,2,2,2,2,2,2,0,0},
		{0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0},
		{0,0,3,3,3,3,3,3,3,3,3,3,3,3,0,0},
		{0,0,3,3,3,3,3,3,3,3,3,3,3,3,0,0},
		{0,0,4,4,4,4,4,4,4,4,4,4,4,4,0,0},
		{0,0,4,4,4,4,4,4,4,4,4,4,4,4,0,0},
		{0,0,4,4,4,4,4,4,4,4,4,4,4,4,0,0},
		{0,0,4,4,4,4,4,4,4,4,4,4,4,4,0,0},
		{0,0,5,5,5,5,0,0,0,0,5,5,5,5,0,0},
		{0,0,5,5,5,5,0,0,0,0,5,5,5,5,0,0}
	},
};
#pragma endregion
#pragma region Struct
struct Obj
{
	int x;
	int y;
	Dir dir;
	Color color;
	char (*shape)[16][16];
};
#pragma endregion
#pragma region Main
int main()
{
	InitBuffer();

	PrintCharactor();

	return 0;
}
#pragma endregion
#pragma region DoubleBuffer
void InitBuffer()
{
	screenIndex = 0;

	COORD size = { BufferWidth, BufferHeight };

	SMALL_RECT rect = { 0,0,BufferWidth - 1, BufferHeight - 1 };

	hBuffer[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

	SetConsoleScreenBufferSize(hBuffer[0], size);
	
	SetConsoleWindowInfo(hBuffer[0], TRUE, &rect);

	
	hBuffer[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	
	SetConsoleScreenBufferSize(hBuffer[1], size);
	
	SetConsoleWindowInfo(hBuffer[1], TRUE, &rect);

	CONSOLE_CURSOR_INFO info;	
	info.dwSize = 1;			
	info.bVisible = FALSE;		

	
	SetConsoleCursorInfo(hBuffer[0], &info);
	SetConsoleCursorInfo(hBuffer[1], &info);


}

void FlipBuffer()
{
	
	SetConsoleActiveScreenBuffer(hBuffer[screenIndex]);

	screenIndex = !screenIndex;



}

void ClearBuffer()
{
	COORD pos = { 0,0 };
	DWORD dw;

	FillConsoleOutputCharacter(hBuffer[screenIndex], ' ', BufferWidth * BufferHeight, pos, &dw);

}

void WriteBuffer(int x, int y, const char* shape, int color)
{
	COORD pos = { x * 2, y };

	SetConsoleCursorPosition(hBuffer[screenIndex], pos);

	SetConsoleTextAttribute(hBuffer[screenIndex], color);

	DWORD dw;

	WriteFile(hBuffer[screenIndex], shape, strlen(shape), &dw, NULL);
}

void CloseBuffer()
{
	CloseHandle(hBuffer[0]);
	CloseHandle(hBuffer[1]);
}

#pragma endregion

#pragma region Windows
void SetPosition(int x, int y)
{
	COORD pos;
	pos.X = x * 2;
	pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void setColor(int color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

#pragma endregion
#pragma region Charactor
void PrintCharactor()
{

	Obj player;
	player.x = 125;
	player.y = 90;
	player.dir = RIGHT;
	player.color = WHITE;
	player.shape = character;

	while (true)
	{



		if (GetAsyncKeyState(VK_LEFT))
		{
			player.x -= speed;
			player.dir = LEFT;

		}
		if (GetAsyncKeyState(VK_RIGHT))
		{
			player.x += speed;
			player.dir = RIGHT;

		}
		if (GetAsyncKeyState(VK_UP))
		{

			player.y -= speed;
		}
		if (GetAsyncKeyState(VK_DOWN))
		{
			player.y += speed;
		}

		ClearBuffer();

		for (int y = 0; y < 16; y++)
		{
			for (int x = 0; x < 16; x++)
			{

				switch (character[player.dir][y][x])
				{
				case 1:
					WriteBuffer(player.x + x, player.y + y, "��", RED);
					break;
				case 2:
					WriteBuffer(player.x + x, player.y + y, "��", RED);
					break;
				case 3:
					WriteBuffer(player.x + x, player.y + y, "��", YELLOW);
					break;
				case 4:
					WriteBuffer(player.x + x, player.y + y, "��", LIGHTBLUE);
					break;
				case 5:
					WriteBuffer(player.x + x, player.y + y, "��", LIGHTBLUE);
					break;
				default:
					break;
				}

			}
		}
		FlipBuffer();

		Sleep(50);
	}

}
#pragma endregion


