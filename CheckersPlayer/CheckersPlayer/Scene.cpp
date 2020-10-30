#include "Scene.h"
#include "Game.h"


Scene::Scene(Game* activeGame)
	:game(activeGame)
{
}

void Scene::SwitchScene(Scene* newScene) {
	game->activeScene = newScene;
	delete this;
}

void Scene::DrawImage(SDL_Texture* texture, SDL_Rect* pos)
{
	SDL_Rect truePos;
	truePos.x = pos->x * game->xScale;
	truePos.y = pos->y * game->yScale;
	truePos.w = pos->w * game->xScale;
	truePos.h = pos->h * game->yScale;
	SDL_RenderCopy(game->renderer, texture, NULL, &truePos);
}

void Scene::RegisterButton(SDL_Texture* texture, SDL_Rect Position, void(*function)(Scene*))
{
	buttons.emplace_back(Position, texture, function);
}


Scene::~Scene()
{
}

void Scene::Update()
{
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type) {
	case SDL_QUIT:
		game->isRunning = false;
		break;
	case SDL_KEYDOWN:
		if (event.key.keysym.sym == SDLK_ESCAPE) {
			game->isRunning = false;
		}
	case SDL_MOUSEBUTTONUP:
	{
		int mouse_x, mouse_y; SDL_GetMouseState(&mouse_x, &mouse_y);
		for (Button& button : buttons) {
			if (button.Execute(mouse_x, mouse_y, this))
				return;
		}
	}
	default:
		PollEvent(&event);
	}
}

void Scene::Render()
{
	for (Button& button : buttons) {
		button.Render(game->renderer);
	}
}

