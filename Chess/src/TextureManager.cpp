#include "TextureManager.h"
#include <SDL_image.h>
#include <iostream>

SDL_Texture* TextureManager::LoadTexture(std::string InTexturePath, SDL_Renderer* Renderer)
{
    SDL_Surface* loadedSurface = IMG_Load(InTexturePath.c_str());

    SDL_Texture* newTexture = SDL_CreateTextureFromSurface(Renderer, loadedSurface);
    if (newTexture == nullptr)
    {
        std::cout << IMG_GetError() << std::endl;

    }

    SDL_FreeSurface(loadedSurface);

    return newTexture;

}

void TextureManager::Draw(SDL_Texture* text, SDL_Rect src, SDL_Rect dest, SDL_Renderer* renderer)
{
    SDL_RenderCopy(renderer, text, &src, &dest);
}
