#include "Pawn.h"

#include <algorithm>
#include <array>
#include <filesystem>
#include <vector>

Pawn::Pawn(PieceTeam team, PieceType type,PieceCoordinates start_pos)
:Piece(team, type, start_pos)
{
	m_PawnMoves.emplace_back(1,0);
	m_PawnMoves.emplace_back(2,0);

	m_DiagonalMoves.reserve(2);
	m_DiagonalMoves.emplace_back( 1, -1);
	m_DiagonalMoves.emplace_back(1,1);

	if(m_Team == PieceTeam::White)
	{
		SetWhitePawnMoves();
	}
	m_StartXPos = m_PiecePosition.x;
}

bool Pawn::FindPossibleMoves(const BoardPieces& all_pieces, const PieceCoordinates& piece_coordinates) 
{
	m_PossibleMoves.clear();

	std::vector<PieceCoordinates> possible_pawn_moves;
	for (auto default_pawn_move : m_PawnMoves)
	{
		const int x = piece_coordinates.x + default_pawn_move.x;
		const int y = piece_coordinates.y + default_pawn_move.y;
		if(Piece* piece = all_pieces[x][y])
		{
			if(piece->IsOpponentTeam(m_Team))
				continue;
		}
		possible_pawn_moves.emplace_back(default_pawn_move);
	}
	
	for (const auto& diagonal_move : m_DiagonalMoves)
	{
		const int move_x = piece_coordinates.x + diagonal_move.x;
		const int move_y = piece_coordinates.y + diagonal_move.y;
		if(const Piece* other_team_piece = all_pieces[move_x][move_y])
		{
			if(IsOpponentTeam(other_team_piece->GetTeam()))
			{
				possible_pawn_moves.emplace_back(diagonal_move.x, diagonal_move.y);	
			}
		}
	}

	if (b_CanCaptureEnPassant)
	{
		std::array<PieceCoordinates, 2> en_passant_opponent_pawns =
		{
			direction_coordinates[static_cast<int>(Direction::Right)],
			direction_coordinates[static_cast<int>(Direction::Left)]
		};
		
		std::array<PieceCoordinates, 2> en_passant_capture_placement;
		if(m_Team == PieceTeam::White)
		{
			en_passant_capture_placement = {direction_coordinates[static_cast<int>(Direction::TopRight)],
			direction_coordinates[static_cast<int>(Direction::TopLeft)]};
		}
		else
		{
			en_passant_capture_placement =  {direction_coordinates[static_cast<int>(Direction::BottomRight)],
			direction_coordinates[static_cast<int>(Direction::BottomLeft)]};
		}
		
		for (int i = 0; i < en_passant_opponent_pawns.size(); ++i)
		{
			const auto& en_passant_move = piece_coordinates + en_passant_opponent_pawns[i];
			const auto& capture_position = piece_coordinates + en_passant_capture_placement[i];
			
			if (!BoardBoundsCheck(en_passant_move.x, en_passant_move.y))
				continue;
			
			if (!BoardBoundsCheck(capture_position.x, capture_position.y))
				continue;
			
			
			if (Piece* piece = all_pieces[en_passant_move.x][en_passant_move.y])
			{
				if (piece->IsOpponentTeam(m_Team))
				{
					// Ensure the piece is a pawn
					if (piece->GetType() == PieceType::B_Pawn || piece->GetType() == PieceType::W_Pawn)
					{
						if (Pawn* pawn = dynamic_cast<Pawn*>(piece))
						{
							if (pawn->CheckCanBeCapturedEnPassant())
							{
								m_PossibleMoves.emplace_back(capture_position);
							}
						}
					}
				}
			}
		}
	}

	for (const auto& element : possible_pawn_moves)
	{
		AddPossibleMove(all_pieces, {piece_coordinates.x + element.x, piece_coordinates.y + element.y});
	}
	
	return m_PossibleMoves.empty() == false;
}

bool Pawn::CheckCanEnPassant() const
{
	return b_CanCaptureEnPassant;
}

bool Pawn::CheckCanBeCapturedEnPassant() const
{
	return b_CanBeCapturedEnPassant;
}

void Pawn::SetWhitePawnMoves()
{
	auto for_each_moves = [&](std::vector<PieceCoordinates>& pawn_moves)
	{
		for (PieceCoordinates& move : pawn_moves)
		{
			move.x *= -1;
		}
	};

	for_each_moves(m_PawnMoves);
	for_each_moves(m_DiagonalMoves);
}

void Pawn::SetPos(int x_pos, int y_pos)
{
	const int old_x = m_PiecePosition.x;
	Piece::SetPos(x_pos, y_pos);
	
	//pawn that can be capture en passant has to have moved forward 3 ranks
	b_CanBeCapturedEnPassant = std::abs(m_StartXPos-x_pos) == 3;
	
	if(b_IsFirstMove && b_CanCaptureEnPassant == false)
	{
		//pawn that is captured en passant has to have move forward 2 ranks in one go
		if(std::abs(old_x - x_pos) == 2)
		{
			b_CanCaptureEnPassant = true;
		}
	}
	if(!b_IsFirstMove)
	{
		b_CanCaptureEnPassant = false;
		return;
	}
	
	m_PawnMoves.clear();
	m_PawnMoves.emplace_back(1,0);
	
	if(m_Team == PieceTeam::White)
	{
		for (PieceCoordinates& move : m_PawnMoves)
		{
			move.x *= -1;
		}
	}
	Piece::SetPos(x_pos, y_pos);
	b_IsFirstMove = false;
}

void Pawn::CheckIsSpecialMove(::Move& piece_move, const BoardPieces& all_pieces) 
{
	// Determine the direction of movement based on the team
	std::array<PieceCoordinates, 2> pawn_moves;
	if(m_Team == PieceTeam::White)
	{
		pawn_moves = {{direction_coordinates[static_cast<int>(Direction::TopLeft)],
						direction_coordinates[static_cast<int>(Direction::TopRight)]}};
	}
	else
	{
		pawn_moves = {{direction_coordinates[static_cast<int>(Direction::BottomLeft)],
						direction_coordinates[static_cast<int>(Direction::BottomRight)]}};
	}
	PieceCoordinates diagonal_move(piece_move.m_TargetMove.x - piece_move.m_StartMove.x, piece_move.m_TargetMove.y - piece_move.m_StartMove.y);
	auto find_pred = [diagonal_move](const PieceCoordinates& piece_coordinates)
	{
		return diagonal_move.x == piece_coordinates.x && diagonal_move.y == piece_coordinates.y;
	};
	auto found =  std::find_if(pawn_moves.begin(),pawn_moves.end(), find_pred);
	if(found == pawn_moves.end())
		return;
	
	// Ensure the target square is empty (en passant target square)
	if (all_pieces[piece_move.m_TargetMove.x][piece_move.m_TargetMove.y] == nullptr)
	{
		// Check if there is a potential pawn to capture in the adjacent square
		Piece* potential_pawn = all_pieces[piece_move.m_StartMove.x][piece_move.m_TargetMove.y];
		const PieceType type = potential_pawn->GetType();
		if (potential_pawn && (type == PieceType::W_Pawn || type == PieceType::B_Pawn)  &&  IsOpponentTeam(potential_pawn->GetTeam()))
		{
			if (Pawn* pawn = dynamic_cast<Pawn*>(potential_pawn))
			{
				if (pawn->CheckCanBeCapturedEnPassant())
				{
					piece_move.m_SpecialMoveType = SpecialMoveType::EnPassant;
				}
			}
		}
	}
	
}

