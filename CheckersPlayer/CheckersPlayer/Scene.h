#pragma once
#include "SDL.h"
#include "Game.h"
#include "Button.h"

#include <vector>

class Scene
{
	std::vector<Button> buttons;

protected:

	void DrawImage(SDL_Texture* texture, SDL_Rect* pos);
	void RegisterButton(SDL_Texture* texture, SDL_Rect Position, void (*function)(Scene*));

public:
	Game* game;

	Scene(Game* activeGame);

	void SwitchScene(Scene* newScene);


	virtual ~Scene();
	
	//To be overridden
	virtual void PollEvent(SDL_Event* event) = 0;
	virtual void Update();
	virtual void Render();
};

