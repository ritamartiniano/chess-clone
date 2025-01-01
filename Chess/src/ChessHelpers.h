#pragma once
#include "PieceType.h"

namespace ChessHelpers
{
    inline bool BoardBoundsCheck(int x, int y)
    {
        if(x < 0 || x >= BOARD_SIZE || y < 0 || y >= BOARD_SIZE)
            return false;
        return true; 
    }

    inline PieceTeam GetPieceTeam(PieceType type)
    {
        const PieceTeam team = (type >= PieceType::B_Bishop && type <= PieceType::B_Turret) 
                       ? PieceTeam::Black 
                       : PieceTeam::White;
        return team;
    }
}
