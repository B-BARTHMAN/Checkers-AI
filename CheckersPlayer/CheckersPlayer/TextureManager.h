#pragma once

#include "SDL.h"
#include <map>

class TextureManager {

	std::map<const char*, SDL_Texture*> m_textures;
	SDL_Renderer* m_renderer;

public:
	TextureManager();
	~TextureManager();

	void SetRenderer(SDL_Renderer* renderer);

	SDL_Texture* CreateImage(const char* path);

};

inline void TextureManager::SetRenderer(SDL_Renderer* renderer) { m_renderer = renderer; }
