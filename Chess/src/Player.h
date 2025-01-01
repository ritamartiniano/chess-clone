#pragma once
#include "PieceType.h"
#include "SDL/SDL_render.h"

constexpr SDL_Color moves_draw_color_white(134, 139, 255);
constexpr SDL_Color moves_draw_color_black(150, 0,255 );
class ChessBoard;
class Player
{
public:
    Player(PieceTeam player_team);
    void SetChessBoard(ChessBoard* chessBoard);
    virtual void Draw(SDL_Renderer* renderer){};
protected:
    ChessBoard* m_Board{nullptr};
    PieceTeam m_PieceTeam{PieceTeam::Count};
    SDL_Color m_MovesDrawColor;
};
