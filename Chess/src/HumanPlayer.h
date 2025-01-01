#pragma once
#include "ChessBoard.h"
#include "InputListener.h"
#include "Player.h"

class HumanPlayer : public Player, public IInputListener
{
public:
    HumanPlayer(PieceTeam piece_team);
    void OnMouseDown(SDL_Event* InEvent) override;
    void OnKeyUp(SDL_Scancode key_code) override;
    void MakeMove();
    void Draw(SDL_Renderer* renderer) override;
private:
    Piece* m_PieceClickedOn{nullptr};
    PieceCoordinates m_CoordinatesClickedOn;
    PieceCoordinates m_CoordinatesToPlacePiece;
};
