#include "Game.h"
#include "SDL_image.h"

#include "TitleScene.h"

Game::Game()
{

	window = SDL_CreateWindow("Chess", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 450, SDL_WINDOW_RESIZABLE);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
	SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);

	textures.SetRenderer(renderer);

	activeScene = new TitleScene(this);


}

void Game::Run() {

	auto* Icon = IMG_Load("assets/CheckersLogo.png");
	SDL_SetWindowIcon(window, Icon);

	while (isRunning) {

		SDL_RenderClear(renderer);

		int x, y;
		SDL_GetRendererOutputSize(renderer, &x, &y);
		xScale = x / 1600.f; yScale = y / 900.f;

		//Handle Scene
		activeScene->Update();
		activeScene->Render();

		SDL_RenderPresent(renderer);

	}
	SDL_FreeSurface(Icon);
}


Game::~Game()
{
	delete activeScene;

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();
}
