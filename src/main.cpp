#include "raylib.h"
#include <iostream>
using namespace std;



void InitGame() {};
void UpdateGame() {};
void DrawGame() {};
void UnloadGame() {};
void UpdateDrawFrame() {};

struct {
	int val;
};

int main() {

	InitWindow(800, 400, "Galaga88");

	InitGame();

	SetTargetFPS(60);

	while (!WindowShouldClose()) { UpdateDrawFrame(); }

	UnloadGame();

	CloseWindow();
	return 0;
}
