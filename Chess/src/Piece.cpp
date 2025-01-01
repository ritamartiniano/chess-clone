#include "Piece.h"

#include <array>

#include "PieceTextureFactory.h"

Piece::Piece(PieceTeam team,PieceType type, PieceCoordinates start_pos)
	: m_Team(team), m_Type(type), m_PiecePosition(start_pos)
{
}

void Piece::SetPos(int x_pos, int y_pos)
{
	m_PiecePosition.x = x_pos;
	m_PiecePosition.y = y_pos;
	m_HasMoved = true;
}

const std::vector<PieceCoordinates>& Piece::GetPossibleMoves() const
{
	return m_PossibleMoves;
}

void Piece::Draw(SDL_Renderer* renderer)
{
	SDL_Rect rect;
	rect.x = m_PiecePosition.y * PIECE_SIZE;
	rect.y = m_PiecePosition.x * PIECE_SIZE;
	rect.w = 60;
	rect.h = 60;
	SDL_RenderCopy(renderer, PieceTextureFactory::GetTextureForType(m_Type), nullptr, &rect);
}

bool Piece::IsPossibleMove(PieceCoordinates coordinates)
{
	auto find_pred = [coordinates](const PieceCoordinates& piece_coordinates)
	{
		return coordinates.x == piece_coordinates.x && coordinates.y == piece_coordinates.y;
	};
	auto piece_coordinates_iterator = std::find_if(m_PossibleMoves.begin(), m_PossibleMoves.end(), find_pred);
	return piece_coordinates_iterator != m_PossibleMoves.end();
}

PieceTeam Piece::GetTeam() const
{
	return m_Team;
}

bool Piece::IsOpponentTeam(PieceTeam team) const
{
	return m_Team != team;
}

PieceType Piece::GetType() const
{
	return m_Type;
}

bool Piece::HasMoved() const
{
	return m_HasMoved;
}

const PieceCoordinates& Piece::GetCoordinates() const
{
	return m_PiecePosition;
}

bool Piece::IsBlockedByFriendly(const Piece* friendly_piece) const
{
	if(friendly_piece)
	{
		//don't look in this direction further if move is blocked by same piece
		if(!IsOpponentTeam(friendly_piece->GetTeam()))
			return true;
	}
	return false;
}

bool Piece::BoardBoundsCheck(int x_coor, int y_coor) const
{
	if(x_coor < 0 || x_coor >= BOARD_SIZE || y_coor < 0 || y_coor >= BOARD_SIZE)
		return false;
	return true;
}

void Piece::AddPossibleMove(const BoardPieces& all_pieces, const PieceCoordinates& possible_move)
{
	if(!BoardBoundsCheck(possible_move.x, possible_move.y))
		return;
	if(Piece* existing_piece = all_pieces[possible_move.x][possible_move.y])
	{
		if(existing_piece->m_Team == m_Team)
		{
			return;
		}
	}
	
	m_PossibleMoves.emplace_back(possible_move);
}


