#include <raylib.h>
#include <iostream>


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
	int move;
	int cnt;
	int frameCounter;
	int currentFrame;
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
bool main_menu = true;
bool credits = false;

int score = 0;
int highscore;
int shootRate = 0;
int shootRate2 = 0;
int activeEnemies = 0;
int enemieskill = 0;
int currentMusic = 0;

const char* main_menu_text_start = "TO START PRESS < ENTER >!";

Player player = { 0 };
Shoots shoot[NUM_SHOOTS] = { 0 };
Enemy enemy[10] = { 0 };
Enemy_Shoots e_shoot[NUM_ENEMY_SHOOTS] = { 0 };

//variables pruebas

int parpadeo = 0;

// global textures

Texture2D background;
Texture2D player_sprite;
Texture2D shoot_sprite;
Texture2D level_sprite;
Texture2D zako_enemy_sprite;
Texture2D zako1, zako2;

Texture2D win_screen;
Texture2D main_menu_background;
Texture2D main_menu_logo;
Texture2D main_menu_namco;
Texture2D main_menu_enemy;
Texture2D credits_screen;

Font customFont;

Sound fighter_shoot;
Sound fighter_killed;
Sound enemy_shoot;
Sound enemy_killed;

Music main_menu_music;
Music game_over_music;
Music victory_music;

int main() {

	InitWindow(screenWidth, screenHeight, "Galaga88");
	InitAudioDevice();
	InitGame();
	SetTargetFPS(60);
	while (!WindowShouldClose()) { UpdateDrawFrame(); }
	CloseAudioDevice();
	UnloadGame();
	CloseWindow();
	return 0;
}

void InitGame() {

	//global variables

	pause = false;
	gameOver = false;
	victory = false;
	main_menu = true;
	credits = false;
	score = 0;
	highscore;
	activeEnemies = 10;
	enemieskill = 0;
	shootRate = 0;
	shootRate2 = 0;

	// variables pruebas

	parpadeo = 0;


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
		shoot[i].rec.x = player.rec.x + player.rec.width / 2;
		shoot[i].rec.y = player.rec.y;
		shoot[i].rec.width = 5;
		shoot[i].rec.height = 10;
		shoot[i].speed.x = 0;
		shoot[i].speed.y = -10;
		shoot[i].active = false;
		shoot[i].color = BLUE;
	}

	//Enemies
	for (int i = 0; i < activeEnemies; i++) {
		enemy[i].rec.width = 32;
		enemy[i].rec.height = 32;
		enemy[i].rec.x = GetRandomValue(0 + enemy[i].rec.width, 840 - enemy[i].rec.width);
		enemy[i].rec.y = GetRandomValue(55 + enemy[i].rec.height, 400);
		enemy[i].speed.x = 1;
		enemy[i].speed.y = 0;
		enemy[i].active = true;
		enemy[i].color = RED;
		enemy[i].move = GetRandomValue(0, 1);
		enemy[i].cnt = 0;
		enemy[i].currentFrame = 0;
		enemy[i].frameCounter = 0;
	}

	//Enemy Shoot

	for (int i = 0; i < NUM_ENEMY_SHOOTS; i++) {
		for (int j = 0; j < activeEnemies; j++) {
			e_shoot[i].rec.x = enemy[j].rec.x + enemy[j].rec.width / 2 + 12;
			e_shoot[i].rec.y = enemy[j].rec.y + enemy[j].rec.height + 20;
			e_shoot[i].rec.width = 5;
			e_shoot[i].rec.height = 10;
			e_shoot[i].speed.x = 0;
			e_shoot[i].speed.y = +10;
			e_shoot[i].active = false;
			e_shoot[i].color = RED;
		}
	}



	// load textures 
	background = LoadTexture("resources/Textures/level-background/stage1.png");
	player_sprite = LoadTexture("resources/Textures/entities/player/fighter.png");
	shoot_sprite = LoadTexture("resources/Textures/entities/enemies/projectiles/disparoplayer.png");
	level_sprite = LoadTexture("resources/Textures/items/stage_indicator.png");
	zako_enemy_sprite = LoadTexture("resources/Textures/entities/enemies/zako_dim1.png");
	zako1 = LoadTexture("resources/Textures/entities/enemies/zako_dim1_1.png");
	zako2 = LoadTexture("resources/Textures/entities/enemies/zako_dim1_2.png");

	win_screen = LoadTexture("resources/Textures/UI/win_condition.png");
	main_menu_background = LoadTexture("resources/Textures/level-background/menu.png");
	main_menu_logo = LoadTexture("resources/Textures/UI/logo.png");
	main_menu_namco = LoadTexture("resources/Textures/texts/logo_namco.png");
	main_menu_enemy = LoadTexture("resources/Textures/UI/enemy_title_screen.png");
	credits_screen = LoadTexture("resources/Textures/UI/credits.png");

	// load font
	customFont = LoadFont("resources/Textures/texts/font/font.png");
	customFont = LoadFontEx("resources/Textures/texts/font/font.png", 8, 0, 42);
	SetTextLineSpacing(16);

	// load sounds
	fighter_shoot = LoadSound("resources/audio/sfx/fighter_shoot.wav");
	fighter_killed = LoadSound("resources/audio/sfx/fighter_destroyed.wav");
	enemy_shoot = LoadSound("resources/audio/sfx/galaga_shot.wav");
	enemy_killed = LoadSound("resources/audio/sfx/galaga_destroyed.wav");

	// load music
	main_menu_music = LoadMusicStream("resources/audio/music/02_game_start.wav");
	PlayMusicStream(main_menu_music);
	game_over_music = LoadMusicStream("resources/audio/music/13_game_over.wav");
	victory_music = LoadMusicStream("resources/audio/music/12_ending.wav");

}
void UpdateGame() {
	if (main_menu == true) {

		UpdateMusicStream(main_menu_music);

		parpadeo++;
		if (parpadeo >= 90) {
			parpadeo = 0;
		}

		if (IsKeyPressed(KEY_ENTER)) {
			main_menu = false;
			credits = true;
		}
	}
	
	else if (credits == true) {
		main_menu = false;
		if (IsKeyPressed(KEY_ENTER)) {
			credits = false;
		}
	}
	
	else if (!gameOver && !victory && main_menu != true && credits != true) {

		StopMusicStream(main_menu_music);

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
						PlaySound(fighter_shoot);
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
					if (enemy[j].active && shoot[i].active) {
						if (CheckCollisionRecs(shoot[i].rec, enemy[j].rec)) {
							shoot[i].active = false;
							enemy[j].active = false;
							enemieskill++;
							PlaySound(enemy_killed);
							score += 100;

							// high score
							if (score > highscore) {
								highscore = score;
							}
						}
					}
				}


				if (shoot[i].rec.y < 0) {
					shoot[i].active = false;
				}
			}



			//Enemy IA
			// 1 = right && 0 = left.
			for (int i = 0; i < activeEnemies; i++) {
				if (enemy[i].active) {
					if (enemy[i].move == 1) {
						enemy[i].rec.x += enemy[i].speed.x;
					}
					else if (enemy[i].move == 0) {
						enemy[i].rec.x -= enemy[i].speed.x;
					}

					enemy[i].frameCounter++;
					if (enemy[i].frameCounter >= 30) { 
						enemy[i].currentFrame = (enemy[i].currentFrame + 1) % 2; 
						enemy[i].frameCounter = 0; 
					}

				}
			}

			//colission pared

			for (int i = 0; i < activeEnemies; i++) {
				if (enemy[i].active && enemy[i].rec.x < 0) {
					enemy[i].move = 1;
				}
				else if (enemy[i].active && enemy[i].rec.x > 800) {
					enemy[i].move = 0;
				}
			}

			//Enemy shoot

			for (int i = 0; i < activeEnemies; i++) {
				i = GetRandomValue(0, activeEnemies);
				if (enemy[i].active) {
					shootRate2 += 5;

					for (int j = 0; j < NUM_ENEMY_SHOOTS; j++) {
						if (!e_shoot[i].active && shootRate2 % 55 == 0) {
							e_shoot[i].rec.x = enemy[i].rec.x + enemy[i].rec.width / 2 + 12;
							e_shoot[i].rec.y = enemy[i].rec.y + enemy[i].rec.height + 20;
							e_shoot[i].active = true;
							PlaySound(enemy_shoot);
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
					PlaySound(fighter_killed);
					e_shoot[i].active = false;
					gameOver = true;
					currentMusic = 1;
					PlayMusicStream(game_over_music);
				}

				//shoot out of the screen

				if (e_shoot[i].rec.y > screenHeight) {
					e_shoot[i].active = false;
				}
			}


			//victory condition

			if (score == 1000) {
				victory = true;
				PlayMusicStream(victory_music);
				currentMusic = 1;
			}

		}
	}
	else {
		if (IsKeyPressed(KEY_ENTER)) {
			InitGame();
			gameOver = false;
			victory = false;
			main_menu = true;
		}
	}

	if (currentMusic == 1) {
		if (victory == true) {
			UpdateMusicStream(victory_music);
		}
		else if (gameOver == true) {
			UpdateMusicStream(game_over_music);
		}
	}
	else if (IsMusicStreamPlaying(victory_music) == 1 && currentMusic != 1 || IsMusicStreamPlaying(game_over_music) == 1 && currentMusic != 1) {
		StopMusicStream(victory_music);
		StopMusicStream(game_over_music);
	}

}
void DrawGame() {

	BeginDrawing();
	ClearBackground(WHITE);

	//draw main menu
	float scaleX = (float)screenWidth / background.width;
	float scaleY = (float)screenHeight / background.height;
	DrawTextureEx(main_menu_background, { 0, 0 }, 0.0f, (scaleX, scaleY), WHITE);
	DrawTextureEx(main_menu_logo, { screenWidth / 10, screenHeight / 10 }, 0.0f, (scaleX / 1.425, scaleY / 1.425), WHITE);

	DrawText("TO START PRESS [ENTER]!", (screenWidth / 2 - MeasureText("TO START PRESS [ENTER]!", 20) / 2) + 15, screenHeight / 2 - 50, 20, GREEN);

	if(parpadeo >= 0 && parpadeo <= 40){ DrawText("INSERT  COIN", screenWidth / 2 - 120, screenHeight / 2, 30, GREEN); }

	DrawText("© 1981 1987 NAMCO", 270, 820, 30, WHITE);
	DrawText("ALL RIGHTS RESERVED", 230, 860, 30, WHITE);
	DrawTextureEx(main_menu_namco, { 277, 960 }, 0.0f, (main_menu_namco.width / 32, main_menu_namco.height / 32), WHITE);

	//DrawTextEx(customFont, main_menu_text_start,{ screenWidth / 2, screenHeight / 2 },customFont.baseSize, 2, BLACK);

	//draw score
	DrawText("1UP ", 50, 55, 30, YELLOW);
	DrawText(TextFormat("%04i ", score), 50, 80, 30, WHITE);

	DrawText("HIGH SCORE ", 340, 55, 30, RED);
	DrawText(TextFormat("%04i ", highscore), 400, 80, 30, WHITE);

	if (credits == true) {
		DrawTextureEx(credits_screen, { 0, 0 }, 0.0f, (scaleX, scaleY), WHITE);
		DrawText("PRESS [ENTER] TO CONTINUE!", (screenWidth / 2 - MeasureText("PRESS [ENTER] TO CONTINUE!", 20) / 2) + 15, screenHeight / 4 - 50, 20, GREEN);
	}

	if (main_menu != true && credits != true) { 
		//draw background 
		DrawTextureEx(background, { 0, 0 }, 0.0f, (scaleX, scaleY), WHITE); 

		//draw score
		DrawText("1UP ", 50, 55, 30, YELLOW);
		DrawText(TextFormat("%04i ", score), 50, 80, 30, WHITE);

		DrawText("HIGH SCORE ", 340, 55, 30, RED);
		DrawText(TextFormat("%04i ", highscore), 400, 80, 30, WHITE);

		//draw Player

		DrawTextureEx(player_sprite, { player.rec.x, player.rec.y }, 0.0f, (player.rec.width / player_sprite.width, player.rec.height / player_sprite.height), WHITE);

		if (!gameOver && !victory) {
			/*DrawRectangleRec(player.rec, player.color);*/

			//draw Enemies

			for (int i = 0; i < activeEnemies; i++) {
				if (enemy[i].active) {
					/*DrawRectangleRec(enemy[i].rec, enemy[i].color);*/
					
					if (enemy[i].currentFrame == 0) {
						DrawTextureEx(zako1, { enemy[i].rec.x + enemy[i].rec.width + 8, enemy[i].rec.y + enemy[i].rec.height + 4 }, 180.f, (3, 3), WHITE);
					}
					else {
						DrawTextureEx(zako2, { enemy[i].rec.x - 8, enemy[i].rec.y - 10 }, 0.f, (3, 3), WHITE);
					}
				  
					
				}

			}

			//draw Shoots

			for (int i = 0; i < NUM_SHOOTS; i++) {
				if (shoot[i].active) {

					DrawTextureEx(shoot_sprite, { (shoot[i].rec.x) - 15, (shoot[i].rec.y) - 20 }, 0.0f, ((shoot[i].rec.width / shoot_sprite.width) * 4, (shoot[i].rec.height * 2 / shoot_sprite.height) * 4), WHITE);

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

			DrawTextureEx(level_sprite, { 800,1000 }, 0.0f, (50 / level_sprite.width, 55 / level_sprite.height), WHITE);

			// pause 

			if (pause) DrawText("GAME PAUSED", screenWidth / 2 - MeasureText("GAME PAUSED", 40) / 2, screenHeight / 2 - 40, 40, RED);
		}
		else if (victory) {
			DrawTextureEx(win_screen, { 0, 0 }, 0.0f, (scaleX, scaleY), WHITE);
			DrawText("PRESS [ENTER] TO PLAY AGAIN", (screenWidth / 2 - MeasureText("PRESS [ENTER] TO PLAY AGAIN WIN", 20) / 2) + 15, screenHeight - 100, 20, RED);
		}
		else {
			DrawText("GAME OVER", screenWidth / 2 - MeasureText("GAME OVER", 40) / 2, 430, 40, RED);
			DrawText("PRESS [ENTER] TO PLAY AGAIN", (screenWidth / 2 - MeasureText("PRESS [ENTER] TO PLAY AGAIN WIN", 20) / 2) + 15, screenHeight / 2 - 50, 20, RED);
		}
	}



	EndDrawing();
}
void UnloadGame() {
	UnloadTexture(background);
	UnloadTexture(player_sprite);
	UnloadTexture(shoot_sprite);
	UnloadTexture(level_sprite);
	UnloadTexture(win_screen);
	UnloadTexture(zako_enemy_sprite);
	UnloadTexture(zako1);
	UnloadTexture(zako2);
	UnloadTexture(main_menu_background);
	UnloadTexture(main_menu_logo);
	UnloadTexture(main_menu_namco);
	UnloadTexture(main_menu_enemy);
	UnloadTexture(credits_screen);

	UnloadFont(customFont);

	UnloadSound(fighter_shoot);
	UnloadSound(fighter_killed);
	UnloadSound(enemy_shoot);
	UnloadSound(enemy_killed);

	UnloadMusicStream(main_menu_music);
	UnloadMusicStream(game_over_music);
	UnloadMusicStream(victory_music);
}
void UpdateDrawFrame() {
	UpdateGame();
	DrawGame();
}


/*

MIT License

	Copyright (c) 2025 Repeaters Studio - Galaga 88.

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.

*/
