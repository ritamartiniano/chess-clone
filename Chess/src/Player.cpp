#include "Player.h"

#include "ChessBoard.h"

Player::Player(PieceTeam player_team)
    :m_PieceTeam(player_team)
{
    if(m_PieceTeam == PieceTeam::White)
    {
        m_MovesDrawColor = moves_draw_color_white;
    }
    else
    {
        m_MovesDrawColor = moves_draw_color_black;
    }
}

void Player::SetChessBoard(ChessBoard* chessBoard)
{
    m_Board = chessBoard;
}

