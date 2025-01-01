#pragma once
#include "Piece.h"
#include "PieceType.h"
class Queen : public Piece
{
public:
	Queen(PieceTeam team, PieceType type, PieceCoordinates coordinates);
	bool FindPossibleMoves(const BoardPieces& all_pieces, const PieceCoordinates& piece_coordinates) override;
	void CheckIsSpecialMove(::Move& piece_move, const BoardPieces& all_pieces) override{};
};

