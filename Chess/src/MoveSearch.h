#pragma once
#include <vector>

#include "PieceType.h"

class ChessBoard;

class MoveSearch
{
public:
    void StartSearch(std::vector<Move>& possible_moves, int depth, PieceTeam team);

private:
    int AlphaBetaMax(int alpha, int beta, int depth);
};
