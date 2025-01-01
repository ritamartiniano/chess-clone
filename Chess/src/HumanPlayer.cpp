#include "HumanPlayer.h"

#include "InputManager.h"


HumanPlayer::HumanPlayer(PieceTeam piece_team)
    :Player(piece_team), m_CoordinatesClickedOn({-1,-1}), m_CoordinatesToPlacePiece({-1,-1})
{
    InputManager::GetInstance()->AddMouseListener(this);
}

void HumanPlayer::OnMouseDown(SDL_Event* InEvent)
{
    if(m_PieceTeam == PieceTeam::White && m_Board->IsWhiteTeamTurn() == false)
        return;

    if(m_PieceTeam == PieceTeam::Black && m_Board->IsBlackTeamTurn() == false)
        return;
    
    const int x_coordinate = InEvent->button.y / PIECE_SIZE;
    const int y_coordinate = InEvent->button.x / PIECE_SIZE;
    if(m_PieceClickedOn)
    {
        if(m_PieceClickedOn->IsPossibleMove({x_coordinate, y_coordinate}))
        {
            m_CoordinatesToPlacePiece = {x_coordinate, y_coordinate};
            
            Move new_move(m_CoordinatesClickedOn, m_CoordinatesToPlacePiece, m_PieceTeam);
            m_PieceClickedOn->CheckIsSpecialMove(new_move, m_Board->GetPieces());
            
            m_Board->MovePiece(new_move);
            m_PieceClickedOn = nullptr;
            m_Board->EndTurn();
        }
        else
        {
            m_PieceClickedOn = nullptr;
        }
    }
    else
    {
        if (Piece* piece_clicked = m_Board->GetPieceClickedOn(x_coordinate, y_coordinate))
        {
            if(piece_clicked->GetTeam() != m_PieceTeam)
                return;
            if(piece_clicked->FindPossibleMoves(m_Board->GetPieces(), {x_coordinate, y_coordinate}))
            {
                m_PieceClickedOn = piece_clicked;
                m_CoordinatesClickedOn = {x_coordinate, y_coordinate};
            }
        }	
    }
}

void HumanPlayer::OnKeyUp(SDL_Scancode key_code)
{
    if(key_code == SDL_SCANCODE_BACKSPACE)
    {
        m_Board->UndoLastMove();
        m_CoordinatesClickedOn = {-1,-1};
        m_PieceClickedOn = nullptr;
    }
}

void HumanPlayer::Draw(SDL_Renderer* renderer)
{
    if(m_PieceClickedOn)
    {
        const std::vector<PieceCoordinates>& coordinates = m_PieceClickedOn->GetPossibleMoves();
        for(const PieceCoordinates& coordinate : coordinates)
        {
            SDL_Rect rect;
            rect.x = coordinate.y * PIECE_SIZE;
            rect.y = coordinate.x * PIECE_SIZE;
            rect.w = 60;
            rect.h = 60;
            SDL_SetRenderDrawColor(renderer,0, m_MovesDrawColor.r, m_MovesDrawColor.g, m_MovesDrawColor.b);
            SDL_RenderFillRect(renderer, &rect);
        }
    }
}
