#include "raylib.h"
#include <iostream>
using namespace std;

// defines

#define screenWidth 800
#define screenHeight 450

// structs

typedef  struct Player {
	Rectangle rec;
	Vector2 speed;
	Color color;
}Player;

 struct Enemy {
	Rectangle rec;
	Vector2 speed;
	bool active;
	Color color;
};


struct Shoot {
	Rectangle rec;
	Vector2 speed;
	bool active;
	Color color;
}shoot[50];

// Basic Functions

void InitGame();
void UpdateGame();
void DrawGame();
void UnloadGame();
void UpdateDrawFrame();

// Global variables

bool gameOver = false;
bool pause = false;
int score = 0;
bool victory = false;

static Player player = { 0 };
static Enemy enemy[10] = { 0 };

int shootRate = 0;
float alpha = 0.0f;

int activeEnemies = 0;
int enemiesKill = 0;
bool smooth = false;

// main 

int main() {

	InitWindow(screenWidth, screenHeight, "Galaga88");

	InitGame();

	SetTargetFPS(60);

	while (!WindowShouldClose()) { UpdateDrawFrame(); }

	UnloadGame();
	
	CloseWindow();
	return 0;
}


void InitGame() {

	//init game variables

	pause = false;
	gameOver = false;
	victory = false;
	smooth = false;
	activeEnemies = 10;
	enemiesKill = 0;
	score = 0;
	alpha = 0;

	//Init Player

	player.rec.x = 20;
	player.rec.y = 50;
	player.rec.width = 20;
	player.rec.height = 50;
	player.speed.x = 5;
	player.speed.y = 5;
	player.color = RED;
	
	
	// Init Enemie
	for (int i = 0; i < 10; i++) {
		enemy[i].rec.x = 10;
		enemy[i].rec.y = 10;
		enemy[i].rec.x = GetRandomValue(screenWidth,screenWidth + 1000);
		enemy[i].rec.y = GetRandomValue(0, screenHeight - enemy[i].rec.height);
		enemy[i].speed.x = 5;
		enemy[i].speed.y = 5;
		enemy[i].active = true;
		enemy[i].color = YELLOW;
	}

	
	// Initialize shoots
	for (int i = 0; i < 50; i++)
	{
		shoot[i].rec.x = player.rec.x;
		shoot[i].rec.y = player.rec.y + player.rec.height / 4;
		shoot[i].rec.width = 10;
		shoot[i].rec.height = 5;
		shoot[i].speed.x = 7;
		shoot[i].speed.y = 0;
		shoot[i].active = false;
		shoot[i].color = BLUE;
	}
}



void UpdateGame()
{

}

void DrawGame()
{
	BeginDrawing();
	ClearBackground(SKYBLUE);

	if (!gameOver) {
		DrawRectangleRec(player.rec, player.color);

		for (int i = 0; i < activeEnemies; i++) {
			if (enemy[i].active) DrawRectangleRec(enemy[i].rec, enemy[i].color);
		}
		for (int i = 0; i < 50; i++)
		{
			if (shoot[i].active) DrawRectangleRec(shoot[i].rec, shoot[i].color);
		}
		DrawText(TextFormat("%04i", score), 20, 20, 40, BLACK);

		if (victory) DrawText("YOU WIN", screenWidth / 2 - MeasureText("YOU WIN", 40) / 2, screenHeight / 2 - 40, 40, BLACK);
		if (pause) DrawText("GAME PAUSED", screenWidth / 2 - MeasureText("GAME PAUSED", 40) / 2, screenHeight / 2 - 40, 40, GRAY);
	}
	else DrawText("PRESS [ENTER] TO PLAY AGAIN", GetScreenWidth() / 2 - MeasureText("PRESS [ENTER] TO PLAY AGAIN", 20) / 2, GetScreenHeight() / 2 - 50, 20, GRAY);

	EndDrawing();
}

void UnloadGame() {}
void UpdateDrawFrame()
{
	UpdateGame();
	DrawGame();
}


