#include "King.h"

#include <array>

King::King(PieceTeam team, PieceType type,PieceCoordinates start_pos)
	:Piece(team, type, start_pos)
{
}

bool King::FindPossibleMoves(const BoardPieces& all_pieces,
	const PieceCoordinates& piece_coordinates) 
{
	m_PossibleMoves.clear();

	for (const auto& possible_move : direction_coordinates)
	{
		AddPossibleMove(all_pieces,piece_coordinates + possible_move );
	}
	
	return m_PossibleMoves.empty() == false;
}