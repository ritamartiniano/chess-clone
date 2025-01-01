#pragma once

#include <SDL.h>
#include <string>
class TextureManager
{
public:
	static SDL_Texture* LoadTexture(std::string InTexturePath, SDL_Renderer* renderer);

	static void Draw(SDL_Texture* text, SDL_Rect src, SDL_Rect dest, SDL_Renderer* renderer);
};

