#pragma once
#include "Piece.h"
struct PieceCoordinates;

class Pawn : public Piece
{
public:
	Pawn(PieceTeam team, PieceType type, PieceCoordinates start_pos);
	bool FindPossibleMoves(const BoardPieces& all_pieces, const PieceCoordinates& piece_coordinates) override;
	void SetPos(int x_pos, int y_pos) override;
	void CheckIsSpecialMove(::Move& piece_move, const BoardPieces& all_pieces) override;
	bool CheckCanEnPassant() const;
	bool CheckCanBeCapturedEnPassant() const;
private:
	void SetWhitePawnMoves();
	std::vector<PieceCoordinates> m_PawnMoves;
	std::vector<PieceCoordinates> m_DiagonalMoves;
	int m_StartXPos{0};
	bool b_IsFirstMove{true};
	bool b_CanCaptureEnPassant{false};
	bool b_CanBeCapturedEnPassant{false};
	
};

