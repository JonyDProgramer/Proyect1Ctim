#include "raylib.h"
#include <iostream>
using namespace std;

// defines

#define screenWidth 840             
#define screenHeight 1080         

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
	

	//Init Player

	player.rec.x = 420;            
	player.rec.y = 950;            
	player.rec.width = 30;
	player.rec.height = 35;
	player.speed.x = 5;
	player.speed.y = 5;
	player.color = RED;
	
	
	// Init Enemie
	for (int i = 0; i < 10; i++) {
		enemy[i].rec.width = 20;
		enemy[i].rec.height = 20;
		enemy[i].rec.x = GetRandomValue(0 , screenWidth);
		enemy[i].rec.y = GetRandomValue(0 , screenHeight/2);
		enemy[i].speed.x = 0;
		enemy[i].speed.y = 0;
		enemy[i].active = true;
		enemy[i].color = BLACK;
	}

	
	// Initialize shoots
	for (int i = 0; i < 50; i++)
	{
		shoot[i].rec.x = player.rec.x + player.rec.width / 2;
		shoot[i].rec.y = player.rec.y;
		shoot[i].rec.width = 5;
		shoot[i].rec.height = 10;
		shoot[i].speed.x = 0;
		shoot[i].speed.y = -10;
		shoot[i].active = false;
		shoot[i].color = BLUE;
	}
}



void UpdateGame()
{
	if (!gameOver && !victory) {
		if (IsKeyPressed('P')) { pause = !pause; }

		if (!pause) {
			
			//Player movement
			
			if (IsKeyDown('A') || IsKeyDown(KEY_LEFT)) { player.rec.x -= player.speed.x; }
			if (IsKeyDown('D') || IsKeyDown(KEY_RIGHT)) { player.rec.x += player.speed.x; }


			//Colision With enemy
			for (int i = 0; i < activeEnemies; i++)
			{
				if (CheckCollisionRecs(player.rec, enemy[i].rec)) { gameOver = true; }
			}

			//Enemy
			for (int i = 0; i < activeEnemies; i++) {
				if (enemy[i].active) {
					enemy[i].rec.y += enemy[i].speed.y;
				}
				if (enemy[i].rec.x < 0)
				{
					enemy[i].rec.x = GetRandomValue(0, screenWidth);
					enemy[i].rec.y = GetRandomValue(0, screenHeight / 2);
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
						shoot[i].rec.x = player.rec.x + player.rec.width / 2;
						shoot[i].rec.y = player.rec.y;
						shoot[i].active = true;
						break;
					}
				}

			}

			for (int i = 0; i < 50; i++) {
				if (shoot[i].active) {
					shoot[i].rec.y += shoot[i].speed.y;

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

			if (score >= 1000) { victory = true; }
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

	if (!gameOver && !victory) {
		DrawRectangleRec(player.rec, player.color);
		

		for (int i = 0; i < activeEnemies; i++) {
			if (enemy[i].active) DrawRectangleRec(enemy[i].rec, enemy[i].color);
		}
		for (int i = 0; i < 50; i++)
		{
			if (shoot[i].active) DrawRectangleRec(shoot[i].rec, shoot[i].color);
		}
		DrawText(TextFormat("%04i", score), 20, 20, 40, BLACK);

		
		if (pause) DrawText("GAME PAUSED", screenWidth / 2 - MeasureText("GAME PAUSED", 40) / 2, screenHeight / 2 - 40, 40, GRAY);
	}
	else if (victory) {
		DrawText("YOU WIN", screenWidth / 2 - MeasureText("YOU WIN", 40) / 2, screenHeight / 2 - 40, 40, BLACK);
		DrawText("PRESS [ENTER] TO PLAY AGAIN", GetScreenWidth() / 2 - MeasureText("PRESS [ENTER] TO PLAY AGAIN", 20) / 2, GetScreenHeight() / 2, 20, GRAY);
		 
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


