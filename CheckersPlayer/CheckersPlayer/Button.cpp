#include "Button.h"
#include "Singleton.h"

Button::Button(SDL_Rect pos, SDL_Texture* tex, void(*function)(Scene* scene))
	:func(function), Position(pos), texture(tex)
{
}

Button::~Button()
{
}

void Button::Render(SDL_Renderer* renderer)
{
	int w, h; SDL_GetRendererOutputSize(renderer, &w, &h);

	SDL_Rect pos;
	pos.x = w * Position.x / 1600;
	pos.y = h * Position.y / 900;
	pos.w = w * Position.w / 1600;
	pos.h = h * Position.h / 1600;

	SDL_RenderCopy(renderer, texture, NULL, &pos);
}

bool Button::Execute(int mouse_x, int mouse_y, Scene* scene)
{
	int w, h; SDL_GetRendererOutputSize(s_game->renderer, &w, &h);

	SDL_Rect pos;
	pos.x = w * Position.x / 1600;
	pos.y = h * Position.y / 900;
	pos.w = w * Position.w / 1600;
	pos.h = h * Position.h / 1600;

	if (mouse_x >= pos.x && mouse_x <= pos.x + pos.w
		&& mouse_y >= pos.y && mouse_y <= pos.y + pos.h) {
		func(scene);
		return true;
	}
	return false;
}
