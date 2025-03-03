#include <raylib.h>
#include <iostream>
using namespace std;

//defines

#define screenWidth 840
#define screenHeight 1080

// structs

struct Player {
	Rectangle rec;
	Vector2 speed;
	Color color;
};

// functions

void InitGame();
void UpdateGame();
void DrawGame();
void UnloadGame();
void UpdateDrawFrame();

// gloval variables

bool gameOver = false;
bool pause = false;
bool victory = false;

int score = 0;
int shootRate = 0;
int activeEnemies = 0;
int enemieskill = 0;

Player player = { 0 };


// global textures

Texture2D background;





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
	//global variables
	
	pause = false;
	gameOver = false;
	victory = false;
	score = 0;

	//Player
	player.rec.x = 420;
	player.rec.y = 950;
	player.rec.width = 30;
	player.rec.height = 35;
	player.speed.x = 5;
	player.speed.y = 5;
	player.color = RED;

	// load textures 
	background = LoadTexture("textures/background.png");
}
void UpdateGame() {
	if (!gameOver && !victory) {
		if (IsKeyPressed('P')) { pause = !pause; }
		if (!pause) {

			//Player movement

			if (IsKeyDown('A') || IsKeyDown(KEY_LEFT)) { player.rec.x -= player.speed.x; }
			if (IsKeyDown('D') || IsKeyDown(KEY_RIGHT)) { player.rec.x += player.speed.x; }

			

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
void DrawGame() {
	BeginDrawing();
	ClearBackground(WHITE);
	// draw background 
	float scaleX = (float)screenWidth / background.width;
	float scaleY = (float)screenHeight / background.height;
	DrawTextureEx(background, { 0, 0 }, 0.0f, (scaleX, scaleY), WHITE);

	if (!gameOver && !victory) {
		DrawRectangleRec(player.rec, player.color);







		if (pause) DrawText("GAME PAUSED", screenWidth / 2 - MeasureText("GAME PAUSED", 40) / 2, screenHeight / 2 - 40, 40, RED);
	}
	else if (victory) {
		DrawText("YOU WIN", screenWidth / 2 - MeasureText("YOU WIN", 40) / 2, screenHeight / 2 - 40, 40, BLACK);
		DrawText("PRESS [ENTER] TO PLAY AGAIN", screenWidth / 2 - MeasureText("PRESS [ENTER] TO PLAY AGAIN WIN", 20) / 2, screenHeight / 2 - 50, 20, RED);
	}
	else {
		DrawText("PRESS [ENTER] TO PLAY AGAIN", screenWidth / 2 - MeasureText("PRESS [ENTER] TO PLAY AGAIN WIN", 20) / 2, screenHeight / 2 - 50, 20, RED);
	}

	


	EndDrawing();
}
void UnloadGame() {
	UnloadTexture(background);
}
void UpdateDrawFrame() {
	UpdateGame();
	DrawGame();
}
