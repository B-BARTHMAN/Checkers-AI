//#include "SDL.h"
#include "Game.h"
#include "Singleton.h"

int main(int argc, char* argv[]) {

	SDL_Init(SDL_INIT_VIDEO);

	Game game;
	s_game = &game;

	game.Run();
	
	return 0;
}