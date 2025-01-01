#include "Turret.h"

#include <array>

#include "King.h"

Turret::Turret(PieceTeam team, PieceType type, PieceCoordinates start_pos)
	:Piece(team, type, start_pos)
{
}

bool Turret::FindPossibleMoves(const BoardPieces& all_pieces,
	const PieceCoordinates& piece_coordinates) 
{
	m_PossibleMoves.clear();
	
	const std::array turret_directions = {
		direction_coordinates[static_cast<int>(Direction::Up)],
		direction_coordinates[static_cast<int>(Direction::Down)],
		direction_coordinates[static_cast<int>(Direction::Left)],
		direction_coordinates[static_cast<int>(Direction::Right)],
		};
	
	if(!HasMoved())
	{
		const std::array castle_directions = {
			direction_coordinates[static_cast<int>(Direction::Left)],
			direction_coordinates[static_cast<int>(Direction::Right)],
		};
		for(const auto& castle_direction: castle_directions)
		{
			for (int i = 1; i <= BOARD_SIZE; ++i)
			{
				const int new_x = m_PiecePosition.x + castle_direction.x * i;
				const int new_y = m_PiecePosition.y + castle_direction.y * i;
				if(!BoardBoundsCheck(new_x, new_y))
					break;
				if(const Piece* move_piece = all_pieces[new_x][new_y])
				{
					if(IsBlockedByFriendly(move_piece))
					{
						const PieceType king = m_Team == PieceTeam::White ? PieceType::W_King : PieceType::B_King;
						if(move_piece->GetType() == king && !move_piece->HasMoved())
						{
							m_PossibleMoves.emplace_back(new_x, new_y);
						}
						else
						{
							break;
						}
					}
				}
			}
		}
	}
	AddSlidingPieceMoves(turret_directions, all_pieces);
	return m_PossibleMoves.empty() == false;
}

void Turret::CheckIsSpecialMove(::Move& piece_move, const BoardPieces& all_pieces)
{
	if(Piece* piece = all_pieces[piece_move.m_TargetMove.x][piece_move.m_TargetMove.y])
	{
		if(piece->GetTeam() == m_Team && !piece->HasMoved() && !HasMoved())
		{
			if(piece->GetType() == PieceType::B_King || piece->GetType() == PieceType::W_King)
			{
				piece_move.m_SpecialMoveType = SpecialMoveType::Castling;	
			}
		}
	}
}
