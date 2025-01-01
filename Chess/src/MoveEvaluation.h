#pragma once
#include "PieceType.h"

class ChessBoard;

class MoveEvaluation
{
public:
    int EvaluateMove(const ChessBoard* chess_board, PieceTeam piece_team);
    
};
