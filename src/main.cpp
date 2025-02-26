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
		enemy[i].rec.width = 10;
		enemy[i].rec.height = 10;
		enemy[i].rec.x = GetRandomValue(screenWidth,screenWidth + 1000);
		enemy[i].rec.y = GetRandomValue(0, screenHeight - enemy[i].rec.height);
		enemy[i].speed.x = 5;
		enemy[i].speed.y = 5;
		enemy[i].active = true;
		enemy[i].color = BLACK;
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
	if (!gameOver || !victory) {
		if (IsKeyPressed('P')) { pause = !pause; }

		if (!pause) {
			
			//Player movement
			if (IsKeyDown('W')) { player.rec.y -= player.speed.y; }
			if (IsKeyDown('A')) { player.rec.x -= player.speed.x; }
			if (IsKeyDown('S')) { player.rec.y += player.speed.y; }
			if (IsKeyDown('D')) { player.rec.x += player.speed.x; }

			if (IsKeyDown(KEY_UP)) { player.rec.y -= player.speed.y; }
			if (IsKeyDown(KEY_LEFT)) { player.rec.x -= player.speed.x; }
			if (IsKeyDown(KEY_DOWN)) { player.rec.y += player.speed.y; }
			if (IsKeyDown(KEY_RIGHT)) { player.rec.x += player.speed.x; }

			//Colision With enemy
			for (int i = 0; i < activeEnemies; i++)
			{
				if (CheckCollisionRecs(player.rec, enemy[i].rec)) { gameOver = true; }
			}

			//Enemy
			for (int i = 0; i < activeEnemies; i++) {
				if (enemy[i].active) {
					enemy[i].rec.x -= enemy[i].speed.x;
				}
				if (enemy[i].rec.x < 0)
				{
					enemy[i].rec.x = GetRandomValue(screenWidth, screenWidth + 1000);
					enemy[i].rec.y = GetRandomValue(0, screenHeight - enemy[i].rec.height);
				}
			}

			//Walls
			if (player.rec.x <= 0) { player.rec.x = 0; }
			if (player.rec.x + player.rec.width >= screenWidth) { player.rec.x = screenWidth - player.rec.width; }
			if (player.rec.y <= 0) { player.rec.y = 0; }
			if (player.rec.y + player.rec.height >= screenHeight) { player.rec.y = screenHeight - player.rec.height; }

			//Shoots
			if (IsKeyDown(KEY_SPACE)) {

				shootRate += 5;

				for (int i = 0; i < 50; i++) {
					if (!shoot[i].active && shootRate % 20 == 0) {
						shoot[i].rec.x = player.rec.x;
						shoot[i].rec.y = player.rec.y + player.rec.height / 4;
						shoot[i].active = true;
						break;
					}
				}

			}

			for (int i = 0; i < 50; i++) {
				if (shoot[i].active) {
					shoot[i].rec.x += shoot[i].speed.x;

					//collision with enemy
					for (int j = 0; j < activeEnemies; j++) {
						if (enemy[j].active) {
							if (CheckCollisionRecs(shoot[i].rec, enemy[j].rec)) {
								shoot[i].active = false;
								enemy[j].rec.x = GetRandomValue(screenWidth, screenWidth + 1000);
								enemy[j].rec.y = GetRandomValue(0, screenHeight - enemy[j].rec.height);
								enemiesKill++;
								score += 100;
							}
						}

						if(shoot[i].rec.x + shoot[i].rec.width >= screenWidth){
							shoot[i].active = false;
							shootRate = 0;
						}
					}
				}
			}

			if (score >= 5000) { victory = true; }
		}
	}
	else {
		if (IsKeyPressed(KEY_ENTER)) {
			InitGame();
			gameOver = false;
			victory = false;
		}
	}
}

void DrawGame()
{
	BeginDrawing();
	ClearBackground(WHITE);

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


