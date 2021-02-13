#include <Game/Game.h>
#include "States/PlayingState.h"

int main() {

	Game game;
	game.create("Homebase", 1920, 1080);
	game.pushState(new PlayingState(&game));
	game.run();

	return 0;
}