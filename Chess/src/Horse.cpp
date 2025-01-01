#include "Horse.h"

#include <array>

Horse::Horse(PieceTeam team, PieceType type, PieceCoordinates start_pos)
	:Piece(team, type, start_pos)
{
}

bool Horse::FindPossibleMoves(const BoardPieces& all_pieces,
	const PieceCoordinates& piece_coordinates) 
{
	m_PossibleMoves.clear();
	const std::array horse_moves =
	{
		PieceCoordinates{piece_coordinates.x - 1, piece_coordinates.y - 2}, // 2 squares up, 1 left
		PieceCoordinates{piece_coordinates.x + 1, piece_coordinates.y - 2},  // 2 squares up, 1 right
		PieceCoordinates{piece_coordinates.x - 1, piece_coordinates.y + 2}, // 2 squares down, 1 left
		PieceCoordinates{piece_coordinates.x + 1, piece_coordinates.y + 2}, // 2 squares down, 1 right
		PieceCoordinates{piece_coordinates.x - 2, piece_coordinates.y - 1},// 1 square up, 2 squares left
		PieceCoordinates{piece_coordinates.x + 2, piece_coordinates.y - 1}, // 1 square up, 2 squares right
		PieceCoordinates{piece_coordinates.x - 2, piece_coordinates.y + 1}, // 1 square down, 2 squares left
		PieceCoordinates{piece_coordinates.x + 2, piece_coordinates.y + 1} // 1 square down, 2 squares right
	};
	
	for (const auto& horse_move : horse_moves)
	{
		AddPossibleMove(all_pieces, horse_move);
	}
	return m_PossibleMoves.empty() == false;
}

