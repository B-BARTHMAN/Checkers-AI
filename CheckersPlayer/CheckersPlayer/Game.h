#pragma once
#include "SDL.h"
#include "TextureManager.h"

class Scene;

class Game
{

public:

	//SDL stuff
	SDL_Window* window;
	SDL_Renderer* renderer;

	float xScale, yScale;

	bool isRunning = true;

	//Scene
	Scene* activeScene;

	//Textures
	TextureManager textures;

	void Run();

	Game();
	~Game();
};

