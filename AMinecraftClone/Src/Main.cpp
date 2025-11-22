#include "Game.h"

int main(int argc, char** argv) {
	Game game;
	game.Init();
	game.GameLoop();
	game.Terminate();
	return 0;
}