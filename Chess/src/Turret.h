#pragma once
#include "Piece.h"
class Turret : public Piece
{
public:
	Turret(PieceTeam team, PieceType type,PieceCoordinates start_pos);
	bool FindPossibleMoves(const BoardPieces& all_pieces, const PieceCoordinates& piece_coordinates) override;
	void CheckIsSpecialMove(::Move& piece_move,const BoardPieces& all_pieces) override;
};
