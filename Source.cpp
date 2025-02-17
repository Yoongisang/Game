#include <stdio.h>
#include <Windows.h>
#include <stdlib.h>
#include <time.h>
#include <math.h> 

#pragma region define
#define speed 3
#define BufferWidth 500
#define BufferHeight 180
#define MAX_BULLETS 10
#define BULLET_SPEED 5
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

#pragma region OBJ

void PrintOBJ();

char Bullets[5][5] =
{
	{1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1}
};
char HPICON[5][5] =
{
	{0, 1, 0, 1, 0},
	{1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1},
	{0, 1, 1, 1, 0},
	{0, 0, 1, 0, 0}
};
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

char Enemy[1][16][16] =
{
	{

		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
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
	int HP;
	Dir dir;
	Color color;
	char (*shape)[16][16];
};
#pragma endregion

#pragma region Bullet
typedef struct 
{
	int x;
	int y;
	int dx;
	int dy;
	int active;
} Bullet;
#pragma endregion

#pragma region Main
int main()
{
	srand((unsigned)time(NULL));
	
	InitBuffer();

	PrintOBJ();

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
void PrintOBJ()
{

	Obj player;
	player.x = 125;
	player.y = 90;
	player.dir = RIGHT;
	player.color = WHITE;
	player.shape = character;
	player.HP = 10;
	
	const int numEnemies = 10;
	Obj enemies[numEnemies];
 

	Bullet bullets[MAX_BULLETS] = { 0 };
	for (int i = 0; i < MAX_BULLETS; i++)
	{
		bullets[i].active = 0;
	}

	for (int i = 0; i < numEnemies; i++)
	{
		enemies[i].HP = 5;
		enemies[i].color = WHITE;
		enemies[i].shape = Enemy;
		enemies[i].dir = LEFT;
		int valid = 0;
		while (!valid) 
		{
			int newX = rand() % 235;       
			int newY = (rand() % 163) + 2;   
			int dx = newX - player.x;
			int dy = newY - player.y;
			double d = sqrt(dx * dx + dy * dy);
			if (d >= 20) 
			{  
				enemies[i].x = newX;
				enemies[i].y = newY;
				valid = 1;
			}
		}
	}
	


	while (true)
	{



		if (GetAsyncKeyState('A') & 0x8000)
		{
			player.x -= speed;
			player.dir = LEFT;
		
		}
		if (GetAsyncKeyState('D') & 0x8000)
		{
			player.x += speed;
			player.dir = RIGHT;

		}
		if (GetAsyncKeyState('W') & 0x8000)
		{

			player.y -= speed;
		}
		if (GetAsyncKeyState('S') & 0x8000)
		{
			player.y += speed;
		}
		if (player.x < 0)
		{
			player.x += speed;
		}
		if (player.x > 234)
		{
			player.x -= speed;
		}
		if (player.y < 2)
		{
			player.y += speed;
		}
		if (player.y > 164)
		{
			player.y -= speed;
		}
		if (GetAsyncKeyState(VK_LEFT) & 0x8000) 
		{
			
			for (int i = 0; i < MAX_BULLETS; i++) 
			{
				if (!bullets[i].active) 
				{
					bullets[i].active = 1;
					bullets[i].x = player.x + 8 - 2; 
					bullets[i].y = player.y + 8 - 2;
					bullets[i].dx = -BULLET_SPEED;
					bullets[i].dy = 0;
					break;
				}
			}
		}

		if (GetAsyncKeyState(VK_RIGHT) & 0x8000) 
		{
			for (int i = 0; i < MAX_BULLETS; i++) 
			{
				if (!bullets[i].active) 
				{
					bullets[i].active = 1;
					bullets[i].x = player.x + 8 - 2;
					bullets[i].y = player.y + 8 - 2;
					bullets[i].dx = BULLET_SPEED;
					bullets[i].dy = 0;
					break;
				}
			}
		}
		if (GetAsyncKeyState(VK_UP) & 0x8000) 
		{
			for (int i = 0; i < MAX_BULLETS; i++) 
			{
				if (!bullets[i].active) 
				{
					bullets[i].active = 1;
					bullets[i].x = player.x + 8 - 2;
					bullets[i].y = player.y + 8 - 2;
					bullets[i].dx = 0;
					bullets[i].dy = -BULLET_SPEED;
					break;
				}
			}
		}
		if (GetAsyncKeyState(VK_DOWN) & 0x8000) 
		{
			for (int i = 0; i < MAX_BULLETS; i++) 
			{
				if (!bullets[i].active) 
				{
					bullets[i].active = 1;
					bullets[i].x = player.x + 8 - 2;
					bullets[i].y = player.y + 8 - 2;
					bullets[i].dx = 0;
					bullets[i].dy = BULLET_SPEED;
					break;
				}
			}
		}

		for (int i = 0; i < MAX_BULLETS; i++) 
		{
			if (bullets[i].active) 
			{
				bullets[i].x += bullets[i].dx;
				bullets[i].y += bullets[i].dy;
				
				if (bullets[i].x < 0 || bullets[i].x > 234 || bullets[i].y < 0 || bullets[i].y > 164)
					bullets[i].active = 0;
			}
		}

		for (int i = 0; i < MAX_BULLETS; i++) 
		{
			if (bullets[i].active) {
				for (int j = 0; j < numEnemies; j++) 
				{
					if (enemies[j].HP > 0) 
					{
						if (bullets[i].x < enemies[j].x + 16 &&
							bullets[i].x + 5 > enemies[j].x &&
							bullets[i].y < enemies[j].y + 16 &&
							bullets[i].y + 5 > enemies[j].y) 
						{

							enemies[j].HP -= 1;
							bullets[i].active = 0;
							break;
						}
					}
				}
			}
		}

		ClearBuffer();

		for (int i = 0; i < player.HP; i++) 
		{
			int iconStartX = i * (5 + 1);  

			for (int r = 0; r < 5; r++) 
			{
				for (int c = 0; c < 5; c++) 
				{
					if (HPICON[r][c] == 1) 
					{
						WriteBuffer(iconStartX + c, r, "бс", RED);
					}
				}
			}
		}

		for (int i = 0; i < MAX_BULLETS; i++) 
		{
			if (bullets[i].active) 
			{
				
				for (int r = 0; r < 5; r++) 
				{
					for (int c = 0; c < 5; c++) 
					{
						if (Bullets[r][c] == 1) 
						{
							WriteBuffer(bullets[i].x + c, bullets[i].y + r, "бс", YELLOW);
						}
					}
				}
			}
		}

		for (int y = 0; y < 16; y++)
		{
			for (int x = 0; x < 16; x++)
			{

				switch (character[player.dir][y][x])
				{
				case 1:
					WriteBuffer(player.x + x, player.y + y, "бс", RED);
					break;
				case 2:
					WriteBuffer(player.x + x, player.y + y, "бс", RED);
					break;
				case 3:
					WriteBuffer(player.x + x, player.y + y, "бс", YELLOW);
					break;
				case 4:
					WriteBuffer(player.x + x, player.y + y, "бс", LIGHTBLUE);
					break;
				case 5:
					WriteBuffer(player.x + x, player.y + y, "бс", LIGHTBLUE);
					break;
				default:
					break;
				}

			}
		}

		for (int i = 0; i < numEnemies; i++) 
		{
			if (enemies[i].HP > 0) 
			{
				double enemySpeed = 2.0;
				double exCenter = enemies[i].x + 8;
				double eyCenter = enemies[i].y + 8;
				double pxCenter = player.x + 8;
				double pyCenter = player.y + 8;
				double dx = pxCenter - exCenter;
				double dy = pyCenter - eyCenter;
				double dist = sqrt(dx * dx + dy * dy);
				if (dist != 0) 
				{
					enemies[i].x += (int)((dx / dist) * enemySpeed);
					enemies[i].y += (int)((dy / dist) * enemySpeed);
				}
			}
			if (enemies[i].HP > 0 &&
				player.x < enemies[i].x + 16 && player.x + 16 > enemies[i].x &&
				player.y < enemies[i].y + 16 && player.y + 16 > enemies[i].y) 
			{
				double playerCenterX = player.x + 8;
				double playerCenterY = player.y + 8;
				double enemyCenterX = enemies[i].x + 8;
				double enemyCenterY = enemies[i].y + 8;
				double dx = playerCenterX - enemyCenterX;
				double dy = playerCenterY - enemyCenterY;
				double distance = sqrt(dx * dx + dy * dy);
				if (distance == 0) 
				{
					player.x += 10;
				}
				else 
				{
					player.x += (int)((dx / distance) * 10);
					player.y += (int)((dy / distance) * 10);
					enemies[i].x -= (int)((dx / distance) * 10);
					enemies[i].y -= (int)((dy / distance) * 10);
				}
				
				player.HP -= 1;
				enemies[i].HP -= 1;
				if (player.HP <= 0) 
				{
					break; 
				}
			}

			if (enemies[i].HP <= 0) {
				Sleep(50); 
				enemies[i].HP = 5;
				
				int valid = 0;
				while (!valid) {
					int newX = rand() % 235;        
					int newY = (rand() % 163) + 2;    
					int dx = newX - player.x;
					int dy = newY - player.y;
					double d = sqrt(dx * dx + dy * dy);
					if (d >= 20) {
						enemies[i].x = newX;
						enemies[i].y = newY;
						valid = 1;
					}
				}
			}

		
			if (enemies[i].HP > 0) 
			{
				for (int y = 0; y < 16; y++) 
				{
					for (int x = 0; x < 16; x++) 
					{
						switch (Enemy[0][y][x]) 
						{
						case 3:
							WriteBuffer(enemies[i].x + x, enemies[i].y + y, "бс", LIGHTBLUE);
							break;
						case 4:
							WriteBuffer(enemies[i].x + x, enemies[i].y + y, "бс", RED);
							break;
						case 5:
							WriteBuffer(enemies[i].x + x, enemies[i].y + y, "бс", RED);
							break;
						default:
							break;
						}
					}
				}
			}
		
			if (enemies[i].x < 0) 
			{ 
				enemies[i].x += speed; 
			}
			if (enemies[i].x > 234) 
			{ 
				enemies[i].x -= speed; 
			}
			if (enemies[i].y < 2) 
			{ 
				enemies[i].y += speed;
			}
			if (enemies[i].y > 164) 
			{ 
				enemies[i].y -= speed;
			}
		}

		FlipBuffer();

		Sleep(10);
	}

}
#pragma endregion

