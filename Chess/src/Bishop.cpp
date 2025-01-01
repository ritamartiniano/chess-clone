#include "Bishop.h"
#include <array>

Bishop::Bishop(PieceTeam team, PieceType type, PieceCoordinates start_pos)
	:Piece(team, type, start_pos)
{
}

bool Bishop::FindPossibleMoves(const BoardPieces& all_pieces, const PieceCoordinates& piece_coordinates)
{
	m_PossibleMoves.clear();
	const std::array bishop_directions = {
		direction_coordinates[static_cast<int>(Direction::TopRight)],
		direction_coordinates[static_cast<int>(Direction::TopLeft)],
		direction_coordinates[static_cast<int>(Direction::BottomRight)],
		direction_coordinates[static_cast<int>(Direction::BottomLeft)]
	};
	
	AddSlidingPieceMoves(bishop_directions, all_pieces);
	
	return m_PossibleMoves.empty() == false;
}

