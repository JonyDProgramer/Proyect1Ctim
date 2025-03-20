#include <raylib.h>
#include <iostream>
#include "Entities.h"

using namespace std;

//defines

#define screenWidth 840
#define screenHeight 1080
#define NUM_SHOOTS 50
#define NUM_ENEMY_SHOOTS 50


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
	bool move;
};

struct Enemy_Shoots {
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
int highscore = 0;
int shootRate = 0;
int shootRate2 = 0;
int activeEnemies = 0;
int enemieskill = 0;


Player player = { 0 };
Shoots shoot[NUM_SHOOTS] = { 0 };
Enemy enemy[10] = { 0 };
Enemy_Shoots e_shoot[NUM_ENEMY_SHOOTS] = { 0 };

// global textures

Texture2D background;
Texture2D player_sprite;
Texture2D shoot_sprite;
Texture2D level_sprite;
Texture2D zako_enemy_sprite;
Texture2D win_screen;
Texture2D main_menu;
Texture2D explosion;


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
	highscore = 0;
	activeEnemies = 10;
	enemieskill = 0;
	shootRate = 0;
    shootRate2 = 0;
	


	//Player

	player.rec.x = 420;
	player.rec.y = 900;
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

	//Enemies
	for (int i = 0; i < 10; i++) {
		enemy[i].rec.width = 50;
		enemy[i].rec.height = 55;
		enemy[i].rec.x = GetRandomValue(0 + enemy[i].rec.width, 840 - enemy[i].rec.width);
		enemy[i].rec.y = GetRandomValue(55 + enemy[i].rec.height, 400);
		enemy[i].speed.x = 0.3;
		enemy[i].speed.y = 0.3;
		enemy[i].active = true;
		enemy[i].color = RED;
		enemy[i].move = 1;
	}

	//Enemy Shoot

	for (int i = 0; i < NUM_ENEMY_SHOOTS; i++) {
		for (int j = 0; j < activeEnemies; j++) {
			e_shoot[i].rec.x = enemy[j].rec.x + enemy[j].rec.width / 2;
			e_shoot[i].rec.y = enemy[j].rec.y + enemy[j].rec.height;
			e_shoot[i].rec.width = 5;
			e_shoot[i].rec.height = 10;
			e_shoot[i].speed.x = 0;
			e_shoot[i].speed.y = +10;
			e_shoot[i].active = false;
			e_shoot[i].color = RED;
		}
	}
	
	

	// load textures 
	background = LoadTexture("Textures/level-background/stage1.png");
	player_sprite = LoadTexture("Textures/entities/player/Fighter.png");
	shoot_sprite = LoadTexture("Textures/entities/enemies/projectiles/disparoPlayer.png");
	level_sprite = LoadTexture("Textures/items/stage_indicator.png");
	zako_enemy_sprite = LoadTexture("Textures/entities/enemies/zako_dim1.png");
	win_screen = LoadTexture("Textures/UI/win_condition.png");
	explosion = LoadTexture("Textures/entities/player/explosion.png");

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
						shoot[i].rec.x = (player.rec.x + player.rec.width / 2) - 3;
						shoot[i].rec.y = player.rec.y;
						shoot[i].active = true;
						break;
					}
				}
			}

			//Shoot behavior

			for (int i = 0; i < NUM_SHOOTS; i++) {
				if (shoot[i].active) {
					shoot[i].rec.y += shoot[i].speed.y;
				}

				//collision with enemy

				for (int j = 0; j < activeEnemies; j++) {
					if (enemy[j].active) {
						if (CheckCollisionRecs(shoot[i].rec, enemy[j].rec)) {
							shoot[i].active = false;
							enemy[j].active = false;
							enemieskill++;
							score += 100;
						}
					}
				}


				if (shoot[i].rec.y < 0) {
					shoot[i].active = false;
				}
			}

			//Enemy on Enemy

			for (int i = 0; i < activeEnemies; i++) {
				for (int j = 0; j < activeEnemies; j++) {
					if (CheckCollisionRecs(enemy[i].rec, enemy[j].rec) && (i != j)) {
						
						enemy[i].rec.x = GetRandomValue(0 + enemy[i].rec.width, 840 - enemy[i].rec.width);
						enemy[i].rec.y = GetRandomValue(55 + enemy[i].rec.height, 400);

					}
				}
			}

			//Enemy IA		1 = right && 0 = left.
			for (int i = 0; i < activeEnemies; ++i) {
					if (enemy[i].move == 1 && enemy[i].rec.x < screenWidth - 72) {
						enemy[i].rec.x += enemy[i].speed.x;
					}
					else if (enemy[i].move == 1 && enemy[i].rec.x >= screenWidth - 72) {
						enemy[i].move = 0;
					}
					else if (enemy[i].move == 0 && enemy[i].rec.x > 0 + 16) {
						enemy[i].rec.x -= enemy[i].speed.x;
					}
					else if (enemy[i].move == 0 && enemy[i].rec.x <= 0 + 16) {
						enemy[i].move = 1;
					}
			}

			//Enemy Shoot
			for (int i = 0; i < activeEnemies; i++) {
				if (enemy[i].active) {
					shootRate2 += 5;
					

					for (int j = 0; j < NUM_ENEMY_SHOOTS; j++) {
						if (!e_shoot[i].active && shootRate2 % 55 == 0) {
							e_shoot[i].rec.x = enemy[i].rec.x + enemy[i].rec.width / 2;
							e_shoot[i].rec.y = enemy[i].rec.y + enemy[i].rec.height;
							e_shoot[i].active = true;
							break;
						}
					}
				}
			}

			//Enemy Shoot behavior
			
			for (int i = 0; i < NUM_ENEMY_SHOOTS; i++) {
				if (e_shoot[i].active) {
					e_shoot[i].rec.y += e_shoot[i].speed.y;
				}

				//collision with player

				if (CheckCollisionRecs(e_shoot[i].rec, player.rec)) {
					e_shoot[i].active = false;
					gameOver = true;
				}

				//shoot out of the screen

				if (e_shoot[i].rec.y > screenHeight) {
					e_shoot[i].active = false;
				}
			}
			

			//victory condition

			if (score == 1000) {
				victory = true;
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

	//draw background 

	float scaleX = (float)screenWidth / background.width;
	float scaleY = (float)screenHeight / background.height;
	DrawTextureEx(background, { 0, 0 }, 0.0f, (scaleX, scaleY), WHITE);

	//draw score
	DrawText("1UP ", 50 , 55, 30, YELLOW);
	DrawText(TextFormat("%04i ", score), 50,80,30, WHITE);

	DrawText("HIGH SCORE ", 340, 55, 30, RED);
	DrawText(TextFormat("%04i ", highscore), 400, 80, 30, WHITE);

	//draw Player

	DrawTextureEx(player_sprite, { player.rec.x, player.rec.y}, 0.0f, (player.rec.width/player_sprite.width, player.rec.height/player_sprite.height), WHITE);

	if (!gameOver && !victory) {
		/*DrawRectangleRec(player.rec, player.color);*/

		//draw Enemies

		for (int i = 0; i < activeEnemies; i++) {
			if (enemy[i].active) {
//				DrawRectangleRec(enemy[i].rec, enemy[i].color);
				DrawTextureEx(zako_enemy_sprite, { enemy[i].rec.x, enemy[i].rec.y }, 0.0f, (enemy[i].rec.width / zako_enemy_sprite.width, enemy[i].rec.height / ((zako_enemy_sprite.height) / 2)), WHITE);
			}
			
		}

		//draw Shoots
		 
		for (int i = 0; i < NUM_SHOOTS; i++) {
			if (shoot[i].active) {

				DrawTextureEx(shoot_sprite, { (shoot[i].rec.x)-15, (shoot[i].rec.y)-20}, 0.0f, ((shoot[i].rec.width / shoot_sprite.width)*4, (shoot[i].rec.height*2 / shoot_sprite.height)*4), WHITE);

				/*DrawRectangleRec(shoot[i].rec, shoot[i].color);*/
			}
		}

		//draw enemy shoots

		for (int i = 0; i < NUM_ENEMY_SHOOTS; i++) {
			if (e_shoot[i].active) {
				DrawRectangleRec(e_shoot[i].rec, e_shoot[i].color);
			}
		}

		

		// draw life

		DrawTextureEx(player_sprite, { 55,990 }, 0.0f, (player.rec.width / player_sprite.width, player.rec.height / player_sprite.height), WHITE);
		DrawTextureEx(player_sprite, { 0,990 }, 0.0f, (player.rec.width / player_sprite.width, player.rec.height / player_sprite.height), WHITE);

		// draw level indicator

		DrawTextureEx(level_sprite, { 800,1000 }, 0.0f, (50 / level_sprite.width, 55/ level_sprite.height), WHITE);

		// pause 

		if (pause) DrawText("GAME PAUSED", screenWidth / 2 - MeasureText("GAME PAUSED", 40) / 2, screenHeight / 2 - 40, 40, RED);
	}
	else if (victory) {
		DrawTextureEx(win_screen, { 0, 0 }, 0.0f, (scaleX, scaleY), WHITE);
		DrawText("PRESS [ENTER] TO PLAY AGAIN", (screenWidth / 2 - MeasureText("PRESS [ENTER] TO PLAY AGAIN WIN", 20) / 2)+15 , screenHeight - 100, 20, RED);
	}
	else {
		DrawText("GAME OVER", screenWidth / 2 - MeasureText("GAME OVER", 40) / 2, 430, 40, RED);
		DrawText("PRESS [ENTER] TO PLAY AGAIN", (screenWidth / 2 - MeasureText("PRESS [ENTER] TO PLAY AGAIN WIN", 20) / 2) + 15, screenHeight / 2 - 50, 20, RED);
	}

	


	EndDrawing();
}
void UnloadGame() {
	UnloadTexture(background);
	UnloadTexture(player_sprite);
	UnloadTexture(shoot_sprite);
	UnloadTexture(level_sprite);
	UnloadTexture(zako_enemy_sprite);
	UnloadTexture(win_screen);
	UnloadTexture(main_menu);
	UnloadTexture(explosion);
}
void UpdateDrawFrame() {
	UpdateGame();
	DrawGame();
}
