#include "Queen.h"
#include <array>

Queen::Queen(PieceTeam team, PieceType type, PieceCoordinates start_pos)
	:Piece(team, type, start_pos)
{
}

bool Queen::FindPossibleMoves(const BoardPieces& all_pieces,
	const PieceCoordinates& piece_coordinates) 
{
	m_PossibleMoves.clear();

	const std::array queen_directions = {
	    direction_coordinates[static_cast<int>(Direction::Up)],
	    direction_coordinates[static_cast<int>(Direction::Down)],
	    direction_coordinates[static_cast<int>(Direction::Left)],
	    direction_coordinates[static_cast<int>(Direction::Right)],
	    direction_coordinates[static_cast<int>(Direction::TopRight)],
	    direction_coordinates[static_cast<int>(Direction::TopLeft)],
	    direction_coordinates[static_cast<int>(Direction::BottomRight)],
	    direction_coordinates[static_cast<int>(Direction::BottomLeft)]
	    };
	
	AddSlidingPieceMoves(queen_directions, all_pieces);
	
	return m_PossibleMoves.empty() == false;
}

