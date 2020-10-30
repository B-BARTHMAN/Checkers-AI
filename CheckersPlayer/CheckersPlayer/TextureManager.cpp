#include "TextureManager.h"
#include "SDL_image.h"


TextureManager::TextureManager()
{}

TextureManager::~TextureManager() {
	for (auto texture : m_textures) {
		SDL_DestroyTexture(texture.second);
	}

}

SDL_Texture* TextureManager::CreateImage(const char* path) {

	SDL_Texture* texture;
	if (m_textures.count(path) == 0) {
		auto* temp = IMG_Load(path);

		texture = SDL_CreateTextureFromSurface(m_renderer, temp);

		m_textures.emplace(std::make_pair(path, texture));

		SDL_FreeSurface(temp);
	}
	else {
		texture = m_textures.find(path)->second;
	}
	return texture;
}