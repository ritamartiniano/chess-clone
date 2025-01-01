#pragma once
#include <vector>
#include "PieceType.h"
#include <SDL/SDL_render.h>

class ChessBoard;

class Piece
{
public:
	Piece() = delete;
	Piece(PieceTeam team, PieceType type, PieceCoordinates start_pos);
	virtual ~Piece() = default;
	virtual void SetPos(int x_pos, int y_pos);
	const std::vector<PieceCoordinates>& GetPossibleMoves() const;
	virtual bool FindPossibleMoves(const BoardPieces& all_pieces, const PieceCoordinates& piece_coordinates) = 0;
	virtual void CheckIsSpecialMove(Move& piece_move, const BoardPieces& board_pieces) = 0;
	void Draw(SDL_Renderer* renderer);
	bool IsPossibleMove(PieceCoordinates coordinates);
	PieceTeam GetTeam() const;
	bool IsOpponentTeam(PieceTeam team) const;
	PieceType GetType() const;
	bool HasMoved() const;
	const PieceCoordinates& GetCoordinates() const;
protected:
	template<auto N>
	void AddSlidingPieceMoves(const std::array<PieceCoordinates, N>& sliding_piece_moves, const BoardPieces& all_pieces);
	bool IsBlockedByFriendly(const Piece* friendly_piece) const;
	bool BoardBoundsCheck(int x_coor, int y_coor) const;
	void AddPossibleMove(const BoardPieces& all_pieces, const PieceCoordinates& current_coordinates);
	
	PieceTeam m_Team;
	PieceType m_Type;
	std::vector<PieceCoordinates> m_PossibleMoves;
	PieceCoordinates m_PiecePosition;
	bool m_HasMoved{false};
};

template <auto N>
void Piece::AddSlidingPieceMoves(const std::array<PieceCoordinates, N>& sliding_piece_moves,
	const BoardPieces& all_pieces)
{
	static_assert(N != 0);
	for (const auto& [x_dir, y_dir] : sliding_piece_moves)
	{
		for (int i = 1; i <= BOARD_SIZE; ++i)
		{
			const int new_x = m_PiecePosition.x + x_dir * i;
			const int new_y = m_PiecePosition.y + y_dir * i;
			if(!BoardBoundsCheck(new_x, new_y))
			{
				break;
			}
			if(const Piece* move_piece = all_pieces[new_x][new_y])
			{
				if(IsBlockedByFriendly(move_piece))
					break;

				AddPossibleMove(all_pieces, {new_x, new_y});
				if(move_piece->GetTeam() != m_Team)
					break;
			}
			else
			{
				AddPossibleMove(all_pieces, {new_x, new_y});
			}
		}
	}
}

