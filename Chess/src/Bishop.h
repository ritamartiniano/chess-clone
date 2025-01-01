#pragma once
#include "Piece.h"
class Bishop : public Piece
{
public:
	Bishop(PieceTeam team, PieceType type, PieceCoordinates start_pos);
	bool FindPossibleMoves(const BoardPieces& all_pieces, const PieceCoordinates& piece_coordinates)  override;
	void CheckIsSpecialMove(::Move& piece_move, const BoardPieces& all_pieces) override{};
};

