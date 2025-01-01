#include "PieceTextureFactory.h"
#include "TextureManager.h"

std::string PieceTextureFactory::texture_path[(int)PieceType::MAX] = {
    "Assets/black_bishop.png",
    "Assets/black_horse.png",
    "Assets/black_king.png",
    "Assets/black_pawn.png",
    "Assets/black_queen.png",
    "Assets/black_turret.png",
    "Assets/white_bishop.png",
    "Assets/white_horse.png",
    "Assets/white_king.png",
    "Assets/white_pawn.png",
    "Assets/white_queen.png",
    "Assets/white_turret.png",
};

SDL_Texture* PieceTextureFactory::pieces_textures[(int)PieceType::MAX] = { nullptr };

void PieceTextureFactory::LoadPieces(SDL_Renderer* renderer)
{
    for (int i = 0; i < (int)PieceType::MAX; i++)
    {
        pieces_textures[i] = TextureManager::LoadTexture(texture_path[i], renderer);
    }
}

SDL_Texture* PieceTextureFactory::GetTextureForType(PieceType type)
{
    return pieces_textures[(int)type];
}

void PieceTextureFactory::DestroyTextures()
{
    for (int i = 0; i < (int)PieceType::MAX; i++)
    {
        SDL_DestroyTexture(pieces_textures[i]);
        pieces_textures[i] = nullptr;
    }
}