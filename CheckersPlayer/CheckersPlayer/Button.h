#pragma once
#include "SDL.h"


class Scene;

class Button
{
	//Function to Execute
	void (*func)(Scene* scene);
	
	//Position
	SDL_Rect Position;
	SDL_Texture* texture;

public:

	Button(SDL_Rect pos, SDL_Texture* tex, void (*function)(Scene* scene));
	~Button();

	void Render(SDL_Renderer* renderer);

	bool Execute(int mouse_x, int mouse_y, Scene* scene);
};

