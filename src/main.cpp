#include <raylib.h>
#include <iostream>
using namespace std;

//defines

#define screenWidth 840
#define screenHeight 1080
#define NUM_SHOOTS 50

// structs

struct Player {
	Rectangle rec;
	Vector2 speed;
	Color color;
};

struct Enemy {
	Rectangle rec;
	Vector2 speed;
	bool active;
	Color color;
};

struct Shoots {
	Rectangle rec;
	Vector2 speed;
	bool active;
	Color color;
};

// functions

void InitGame();
void UpdateGame();
void DrawGame();
void UnloadGame();
void UpdateDrawFrame();

// global variables

bool gameOver = false;
bool pause = false;
bool victory = false;

int score = 0;
int shootRate = 0;
int activeEnemies = 0;
int enemieskill = 0;

Player player = { 0 };
Shoots shoot[NUM_SHOOTS] = { 0 };

// global textures

Texture2D background;
Texture2D player_sprite;
Texture2D shoot_sprite;



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
	player.rec.width = 50;
	player.rec.height = 55;
	player.speed.x = 5;
	player.speed.y = 5;
	player.color = RED;

	//Shoot
	for (int i = 0; i < NUM_SHOOTS; i++) {
		shoot[i].rec.x = player.rec.x + player.rec.width/2;
		shoot[i].rec.y = player.rec.y;
		shoot[i].rec.width = 5;
		shoot[i].rec.height = 10;
		shoot[i].speed.x = 0;
		shoot[i].speed.y = -10;
		shoot[i].active = false;
		shoot[i].color = BLUE;
	}


	// load textures 
	background = LoadTexture("Textures/level-background/stage1.png");
	player_sprite = LoadTexture("Textures/entities/player/Fighter.png");
	shoot_sprite = LoadTexture("Textures/entities/enemies/projectiles/disparoPlayer.png");
}
void UpdateGame() {
	if (!gameOver && !victory) {
		if (IsKeyPressed('P')) { pause = !pause; }
		if (!pause) {

			//Player movement

			if (IsKeyDown('A') || IsKeyDown(KEY_LEFT)) { player.rec.x -= player.speed.x; }
			if (IsKeyDown('D') || IsKeyDown(KEY_RIGHT)) { player.rec.x += player.speed.x; }

			// collisions Wall

			if (player.rec.x <= 0) { player.rec.x = 0; }
			if (player.rec.x + player.rec.width >= screenWidth) { player.rec.x = screenWidth - player.rec.width; }


			//Shoot

			if (IsKeyDown(KEY_SPACE)) {
				shootRate += 5;

				for (int i = 0; i < NUM_SHOOTS; i++) {
					if (!shoot[i].active && shootRate % 35 == 0) {
						shoot[i].rec.x = player.rec.x + player.rec.width / 2;
						shoot[i].rec.y = player.rec.y;
						shoot[i].active = true;
						break;
					}
				}
			}

			for (int i = 0; i < NUM_SHOOTS; i++) {
				if (shoot[i].active) {
					shoot[i].rec.y += shoot[i].speed.y;
				}
				if (shoot[i].rec.y < 0) {
					shoot[i].active = false;
				}
			}


			

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

	//draw Player

	DrawTextureEx(player_sprite, { player.rec.x, player.rec.y}, 0.0f, (player.rec.width /player_sprite.width, player.rec.height / player_sprite.height), WHITE);

	
	

	if (!gameOver && !victory) {
		/*DrawRectangleRec(player.rec, player.color);*/

		//draw Shoots
		 
		for (int i = 0; i < NUM_SHOOTS; i++) {
			if (shoot[i].active) {

				DrawTextureEx(shoot_sprite, { shoot[i].rec.x, shoot[i].rec.y}, 0.0f, (shoot[i].rec.width / shoot_sprite.width, shoot[i].rec.height / shoot_sprite.height), WHITE);

				/*DrawRectangleRec(shoot[i].rec, shoot[i].color);*/
			}
		}





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
	UnloadTexture(player_sprite);
	UnloadTexture(shoot_sprite);
}
void UpdateDrawFrame() {
	UpdateGame();
	DrawGame();
}
