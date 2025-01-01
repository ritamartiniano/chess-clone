#pragma once
#include "PieceType.h"
#include <SDL.h>
#include <string> 

enum class PieceType;

class PieceTextureFactory
{
public:
    static void LoadPieces(SDL_Renderer* renderer);
    static SDL_Texture* GetTextureForType(PieceType type);
    static void DestroyTextures();
private:

    static SDL_Texture* pieces_textures[(int)PieceType::MAX];
    static std::string texture_path[(int)PieceType::MAX];
};

