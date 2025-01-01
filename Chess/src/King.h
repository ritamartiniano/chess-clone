#pragma once
#include "Piece.h"
struct PieceCoordinates;

class King : public Piece
{
public: 
	King(PieceTeam team, PieceType type, PieceCoordinates start_pos);
	bool FindPossibleMoves(const BoardPieces& all_pieces, const PieceCoordinates& piece_coordinates) override;
	void CheckIsSpecialMove(::Move& piece_move,const BoardPieces& all_pieces) override{};
	//void DrawPossibleMoves(const ChessBoard* chess_board, const PieceCoordinates& piece_coordinates ) const override;
};

