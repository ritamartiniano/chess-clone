#include "ChessBoard.h"
#include "Pawn.h"
#include "Turret.h"
#include "King.h"
#include "Queen.h"
#include "Bishop.h"
#include "Horse.h"
#include <SDL.h>
#include <algorithm>
#include <iostream>
#include <ostream>

#include "ChessHelpers.h"
#include "PieceType.h"

#include "InputManager.h"
#include "MoveGenerator.h"

static const int board_setup[8][8] = {
	{5,1,0,4,2,0,1,5},
	{3,3,3,3,3,3,3,3},
	{-1,-1,-1,-1,-1,-1,-1,-1},
	{-1,-1,-1,-1,-1,-1,-1,-1 },
	{-1,-1,-1,-1,-1,-1,-1,-1 },
	{-1,-1,-1,-1,-1,-1,-1,-1},
	{9,9,9,9,9,9,9,9},
	{11,7,6,10,8,6,7,11}
};

ChessBoard::ChessBoard()
{
	SetupPieces();
}

ChessBoard::~ChessBoard()
{
	for(int x = 0; x < BOARD_SIZE; x++)
	{
		for(int y = 0; y < BOARD_SIZE; y++)
		{
			delete m_Pieces[x][y];
		}
	}

	while(m_MovesStack.empty() == false)
	{
		auto move = m_MovesStack.top();
		delete move.m_OponentRemovedPiece;
		m_MovesStack.pop();
	}
}

void ChessBoard::Update()
{
	/* if(!b_IsWhiteTeamTurn)
	 {
	 	MoveGenerator move_generator;
	 	Move new_ai_move = move_generator.GenerateRandomMove(this, m_Pieces, PieceTeam::Black);
	 	MovePiece(new_ai_move);
	 	b_IsWhiteTeamTurn = !b_IsWhiteTeamTurn;
	 	CachePiecesInfo();
	 }*/
}

void ChessBoard::Draw(SDL_Renderer* renderer)
{
	for (int x = 0; x < BOARD_SIZE; x++)
	{
		for (int y = 0; y < BOARD_SIZE; y++)
		{
			if (m_Pieces[x][y] != nullptr)
			{
				m_Pieces[x][y]->Draw(renderer);
			}
		}
	}
}

const BoardPieces& ChessBoard::GetPieces() const
{
	return m_Pieces;
}

void ChessBoard::SetupPieces()
{
	for (int x = 0; x < 8; x++)
	{
		for (int y = 0; y < 8; y++)
		{
			if (board_setup[x][y] != -1)
			{
				const PieceType type = (PieceType)board_setup[x][y];
				const PieceTeam team = (type >= PieceType::B_Bishop && type <= PieceType::B_Turret) 
					   ? PieceTeam::Black 
					   : PieceTeam::White;
				switch (type)
				{
				case PieceType::B_Pawn:
				case PieceType::W_Pawn:
					m_Pieces[x][y] = new Pawn(team, type, PieceCoordinates(x,y));
					break;
				case PieceType::B_Horse:
				case PieceType::W_Horse:
					m_Pieces[x][y] = new Horse(team, type, PieceCoordinates(x,y));
					break;
				case PieceType::B_Queen:
				case PieceType::W_Queen:
					m_Pieces[x][y] =  new Queen(team, type, PieceCoordinates(x,y));
					break;
				case PieceType::B_King:
				case PieceType::W_King:
					m_Pieces[x][y] =  new King(team, type, PieceCoordinates(x,y));
					break;
				case PieceType::B_Turret:
				case PieceType::W_Turret:
					m_Pieces[x][y] =  new Turret(team, type, PieceCoordinates(x,y));
					break;
				case PieceType::B_Bishop:
				case PieceType::W_Bishop:
					m_Pieces[x][y] =  new Bishop(team, type, PieceCoordinates(x,y));
					break;
				}
			}
			else
			{
				m_Pieces[x][y] = nullptr;
			}
			
		}
	}
}

Piece* ChessBoard::GetPieceClickedOn(int x, int y) const
{
	return m_Pieces[x][y];
}

void ChessBoard::MovePiece(Move& move)
{
	if(move.m_SpecialMoveType == SpecialMoveType::EnPassant)
	{
		HandleEnPassant(move);
	}
	else if(move.m_SpecialMoveType == SpecialMoveType::Castling)
	{
		HandleCastling(move);
	}
	else
	{
		move.m_OponentRemovedPiece = m_Pieces[move.m_TargetMove.x][move.m_TargetMove.y];
		m_Pieces[move.m_TargetMove.x][move.m_TargetMove.y] = nullptr;
		m_Pieces[move.m_TargetMove.x][move.m_TargetMove.y] = m_Pieces[move.m_StartMove.x][move.m_StartMove.y];
		m_Pieces[move.m_StartMove.x][move.m_StartMove.y] = nullptr;
		m_Pieces[move.m_TargetMove.x][move.m_TargetMove.y]->SetPos(move.m_TargetMove.x, move.m_TargetMove.y);
	}
	
	m_MovesStack.push(move);
	CachePiecesInfo();
}

void ChessBoard::UndoMovePiece(const Move& move)
{
	if(move.m_SpecialMoveType == SpecialMoveType::Castling)
	{
		UndoCasltingMove(move);
		CachePiecesInfo();
		return;
	}
	if(move.m_SpecialMoveType == SpecialMoveType::EnPassant)
	{
		UndoEnPassant(move);
		CachePiecesInfo();
		return;
	}
	
	m_Pieces[move.m_StartMove.x][move.m_StartMove.y] = m_Pieces[move.m_TargetMove.x][move.m_TargetMove.y];
	m_Pieces[move.m_TargetMove.x][move.m_TargetMove.y] = move.m_OponentRemovedPiece;
	if(Piece* piece = m_Pieces[move.m_StartMove.x][move.m_StartMove.y])
	{
		piece->SetPos(move.m_StartMove.x, move.m_StartMove.y );
	}
	if(Piece* piece = m_Pieces[move.m_TargetMove.x][move.m_TargetMove.y])
	{
		piece->SetPos(move.m_TargetMove.x, move.m_TargetMove.y);
	}
	CachePiecesInfo();
}

void ChessBoard::SimulateMove(Move& move)
{
	move.m_OponentRemovedPiece = m_Pieces[move.m_TargetMove.x][move.m_TargetMove.y];
	m_Pieces[move.m_TargetMove.x][move.m_TargetMove.y] = m_Pieces[move.m_StartMove.x][move.m_StartMove.y];
	m_Pieces[move.m_StartMove.x][move.m_StartMove.y] = nullptr;
	CachePiecesInfo();
}

void ChessBoard::UnDoSimulateMove(const Move& move)
{
	m_Pieces[move.m_StartMove.x][move.m_StartMove.y] = m_Pieces[move.m_TargetMove.x][move.m_TargetMove.y];
	m_Pieces[move.m_TargetMove.x][move.m_TargetMove.y] = move.m_OponentRemovedPiece;
	CachePiecesInfo();
}

void ChessBoard::UndoLastMove()
{
	if(m_MovesStack.empty())
		return;
	
	auto player_move = m_MovesStack.top();
	UndoMovePiece(player_move);
	m_MovesStack.pop();

	//auto human_move = m_MovesStack.top();
	//UndoMovePiece(human_move);
	//m_MovesStack.pop();// undo Human Player move
}

const PieceTypeCountMap& ChessBoard::GetPieceTypeCountMap() const
{
	return m_PieceTypeCountMap;
}

const AllPieceCoordinates& ChessBoard::GetAllPiecesCoordinates() const
{
	return m_AllPieceCoordinates;
}

void ChessBoard::EndTurn()
{
	CachePiecesInfo();
	const auto king_checkmate = [this](PieceTeam team)-> bool
	{
		const PieceType type = team == PieceTeam::White ? PieceType::W_King : PieceType::B_King;
		return IsKingInCheckMate(team, m_AllPieceCoordinates.at(type));
	};
	
	b_IsWhiteKingInCheckMate = king_checkmate(PieceTeam::White);
	b_IsBlackKingInCheckMate = king_checkmate(PieceTeam::Black);
	b_IsWhiteTeamTurn = !b_IsWhiteTeamTurn;

	if(b_IsBlackKingInCheckMate)
	{
		std::cout << "White wins" << std::endl;
	}

	if(b_IsWhiteKingInCheckMate)
	{
		std::cout << "Black wins" << std::endl;
	}
}

bool ChessBoard::IsWhiteTeamTurn() const
{
	return b_IsWhiteTeamTurn;
}

bool ChessBoard::IsBlackTeamTurn() const
{
	return b_IsWhiteTeamTurn == false;
}

bool ChessBoard::IsKingInCheck(PieceTeam piece_team, const PieceCoordinates& king_coordinates) const
{
	if(CheckSlidingPiecesCheckingKing(piece_team,king_coordinates))
		return true;
	
	auto verify_piece_is_checking_king = [king_coordinates, this, piece_team](const std::array<PieceCoordinates, 8>& piece_moves, PieceType type) -> bool
	{
		int x, y = 0;
		for (const auto& horse_move : piece_moves)
		{
			x = king_coordinates.x + horse_move.x;
			y = king_coordinates.y + horse_move.y;
            
			if (ChessHelpers::BoardBoundsCheck(x, y))
			{
				if (Piece* piece = m_Pieces[x][y])
				{
					if (piece->GetTeam() != piece_team && piece->GetType() == type)
					{
						return true;
					}
				}
			}
		}
		return false;
	};
    const std::array horse_moves = {
        PieceCoordinates{2, 1}, PieceCoordinates{2, -1}, PieceCoordinates{-2, 1}, PieceCoordinates{-2, -1},
        PieceCoordinates{1, 2}, PieceCoordinates{1, -2}, PieceCoordinates{-1, 2}, PieceCoordinates{-1, -2}
    };

	const PieceType horse = piece_team == PieceTeam::White ? PieceType::B_Horse : PieceType::W_Horse;
    if(verify_piece_is_checking_king(horse_moves, horse))
    {
    	std::cout << "horse is checking the king" << std::endl;
	    return true;
    }
	
	const std::array<PieceCoordinates, 2> pawn_attack_directions = (piece_team == PieceTeam::White) ?
	std::array<PieceCoordinates, 2>{PieceCoordinates{-1, 1}, PieceCoordinates{1, 1}} :
	std::array<PieceCoordinates, 2>{PieceCoordinates{-1, -1}, PieceCoordinates{1, -1}};

	const PieceType pawn = piece_team == PieceTeam::White ? PieceType::B_Pawn : PieceType::W_Pawn;
    for (const auto& dir : pawn_attack_directions)
    {
        int x = king_coordinates.x + dir.x;
        int y = king_coordinates.y + dir.y;

        if (ChessHelpers::BoardBoundsCheck(x, y))
        {
            if (Piece* piece = m_Pieces[x][y])
            {
                if (piece->GetTeam() != piece_team && piece->GetType() == pawn)
                {
                	std::cout << "pawn piece is checking" << std::endl;
                    return true; // There is a Pawn threatening the King.
                }
            }
        }
    }

    const std::array king_moves = {
        PieceCoordinates{-1, -1}, PieceCoordinates{-1, 0}, PieceCoordinates{-1, 1},
        PieceCoordinates{0, -1}, PieceCoordinates{0, 1},
        PieceCoordinates{1, -1}, PieceCoordinates{1, 0}, PieceCoordinates{1, 1}
    };

	const PieceType king = piece_team == PieceTeam::White ? PieceType::B_King : PieceType::W_King;
    if(verify_piece_is_checking_king(king_moves, king))
    {
    	std::cout << "king piece is checking" << std::endl;
    	return true;
    }
    
    return false;
}

bool ChessBoard::CheckSlidingPiecesCheckingKing(PieceTeam piece_team, const PieceCoordinates& king_coordinates) const 
{
	auto loop_directions = [king_coordinates, this, piece_team](int start_index, int end_index, PieceType first_type, PieceType second_type)-> bool
	{
		int x,y = 0;
		for (int i = start_index; i <= end_index; i++)
		{
			x = king_coordinates.x + direction_coordinates[i].x;
			y = king_coordinates.y + direction_coordinates[i].y;
        
			while(ChessHelpers::BoardBoundsCheck(x, y))
			{
				if(Piece* piece = m_Pieces[x][y])
				{
					//is blocked by friendly, don't go further in the axis
					if(piece->GetTeam() == piece_team)
						break;
					if(piece->GetType() == first_type || piece->GetType() == second_type)
						return true;
				}
				x+=direction_coordinates[i].x;
				y+=direction_coordinates[i].y;
			}
		}
		return false;
	};
	
	//check if king is checked on +x,-x,+y,-y axis by queen or turret
	const PieceType queen = piece_team == PieceTeam::White ? PieceType::B_Queen : PieceType::W_Queen;
	const PieceType turret = piece_team == PieceTeam::White ? PieceType::B_Turret : PieceType::W_Turret;
	if(loop_directions(0,3, queen, turret))
	{
		std::cout << "Queen or turret is checking the king"  << std::endl;
		return true;
	}

	const PieceType bishop = piece_team == PieceTeam::White ? PieceType::B_Bishop : PieceType::W_Bishop;
	if(loop_directions(4,7, queen, bishop))
	{
		std::cout << "queen or bishop is checking the king" << std::endl;
		return true;
	}

	return false;
}

bool ChessBoard::IsKingInCheckMate(PieceTeam piece_team, const PieceCoordinates& king_coordinates)
{
	if(IsKingInCheck(piece_team, king_coordinates) == false)
		return false;

	const PieceType king = piece_team == PieceTeam::White ? PieceType::W_King : PieceType::B_King;
	bool has_possible_move = false;
	for(int x = 0; x < BOARD_SIZE; x++)
	{
		for(int y = 0; y < BOARD_SIZE; y++)
		{
			if(Piece* piece = m_Pieces[x][y])
			{
				if(piece->GetTeam() == piece_team)
				{
					if(piece->FindPossibleMoves(m_Pieces, {x,y}))
					{
						const std::vector<PieceCoordinates>& piece_coordinates = piece->GetPossibleMoves();
						for(const auto& piece_move : piece_coordinates)
						{
							Move new_move({x,y}, piece_move, piece_team);
							//check is special move
							SimulateMove(new_move);
							const PieceCoordinates new_king_coordinates = m_AllPieceCoordinates.at(king);
							bool king_in_check = IsKingInCheck(piece_team, new_king_coordinates);
							UnDoSimulateMove(new_move);
							if(king_in_check == false)
							{
								has_possible_move = true;
							}

							if(has_possible_move)
							{
								std::cout << "Valid move to uncheck king: Start["
										  << new_move.m_StartMove.x << ", " << new_move.m_StartMove.y << "] -> Target["
										  << new_move.m_TargetMove.x << ", " << new_move.m_TargetMove.y << "]" 
										  << " for team " << (piece_team == PieceTeam::White ? "White" : "Black") << std::endl;
								return false;
							}
						}
					}
				}
			}	
		}
	}
	return true;
}

void ChessBoard::CachePiecesInfo()
{
	m_AllPieceCoordinates.clear();
	std::fill(m_PieceTypeCountMap.begin(), m_PieceTypeCountMap.end(), 0);
	for(int x = 0; x < BOARD_SIZE; x++)
	{
		for(int y = 0; y < BOARD_SIZE; y++)
		{
			if(Piece* piece = m_Pieces[x][y])
			{
				const PieceType piece_type = piece->GetType();
				m_PieceTypeCountMap[static_cast<int>(piece_type)]++;
				m_AllPieceCoordinates[piece_type] = PieceCoordinates(x, y);
			}
		}
	}
}

void ChessBoard::HandleEnPassant(Move& move)
{
	m_Pieces[move.m_TargetMove.x][move.m_TargetMove.y] = nullptr;
	m_Pieces[move.m_TargetMove.x][move.m_TargetMove.y] = m_Pieces[move.m_StartMove.x][move.m_StartMove.y];
	m_Pieces[move.m_StartMove.x][move.m_StartMove.y] = nullptr;
	m_Pieces[move.m_TargetMove.x][move.m_TargetMove.y]->SetPos(move.m_TargetMove.x, move.m_TargetMove.y);
	move.m_OponentRemovedPiece = m_Pieces[move.m_StartMove.x][move.m_TargetMove.y];
	m_Pieces[move.m_StartMove.x][move.m_TargetMove.y] = nullptr;
	
}

void ChessBoard::UndoEnPassant(const Move& move)
{
	m_Pieces[move.m_StartMove.x][move.m_StartMove.y] = m_Pieces[move.m_TargetMove.x][move.m_TargetMove.y];
	m_Pieces[move.m_TargetMove.x][move.m_TargetMove.y] = nullptr;
	m_Pieces[move.m_StartMove.x][move.m_StartMove.y]->SetPos(move.m_StartMove.x, move.m_StartMove.y);
	if(Piece* removed_piece = move.m_OponentRemovedPiece)
	{
		const PieceCoordinates& piece_coordinates = removed_piece->GetCoordinates();
		m_Pieces[piece_coordinates.x][piece_coordinates.y] = move.m_OponentRemovedPiece;
		removed_piece->SetPos(piece_coordinates.x, piece_coordinates.y);
	}
}

void ChessBoard::HandleCastling(Move& move)
{
	const bool castle_to_left = move.m_StartMove.y > move.m_TargetMove.y;
	const PieceType king_piece_type = move.m_Team == PieceTeam::White ? PieceType::W_King : PieceType::B_King;
	const PieceCoordinates king_coordinates = m_AllPieceCoordinates.at(king_piece_type);
	
	//move turret
	PieceCoordinates new_turret_coordinates = move.m_StartMove;

	new_turret_coordinates.y = castle_to_left ? new_turret_coordinates.y - 2 : new_turret_coordinates.y + 3;
	m_Pieces[new_turret_coordinates.x][new_turret_coordinates.y] = m_Pieces[move.m_StartMove.x][move.m_StartMove.y];
	m_Pieces[move.m_StartMove.x][move.m_StartMove.y] = nullptr;
	m_Pieces[new_turret_coordinates.x][new_turret_coordinates.y]->SetPos(new_turret_coordinates.x, new_turret_coordinates.y);

	move.m_TargetMove = new_turret_coordinates;

	//move king
	PieceCoordinates new_king_coordinates = king_coordinates;
	new_king_coordinates.y = castle_to_left ? king_coordinates.y + 2 : king_coordinates.y -2;
	m_Pieces[new_king_coordinates.x][new_king_coordinates.y] = m_Pieces[king_coordinates.x][king_coordinates.y];
	m_Pieces[king_coordinates.x][king_coordinates.y] = nullptr;
	m_Pieces[new_king_coordinates.x][new_king_coordinates.y]->SetPos(new_king_coordinates.x, new_king_coordinates.y);
	
}

void ChessBoard::UndoCasltingMove(const Move& move)
{
	const bool castle_to_left = move.m_StartMove.y > move.m_TargetMove.y;
	const PieceType king_piece_type = move.m_Team == PieceTeam::White ? PieceType::W_King : PieceType::B_King;
	const PieceCoordinates king_coordinates = m_AllPieceCoordinates.at(king_piece_type);
	
	m_Pieces[move.m_StartMove.x][move.m_StartMove.y] = m_Pieces[move.m_TargetMove.x][move.m_TargetMove.y];
	m_Pieces[move.m_TargetMove.x][move.m_TargetMove.y] = nullptr;
	m_Pieces[move.m_StartMove.x][move.m_StartMove.y]->SetPos(move.m_StartMove.x, move.m_StartMove.y);

	PieceCoordinates new_king_coordinates = king_coordinates;
	new_king_coordinates.y = castle_to_left ? king_coordinates.y - 2 : king_coordinates.y + 2;

	m_Pieces[new_king_coordinates.x][new_king_coordinates.y] = m_Pieces[king_coordinates.x][king_coordinates.y];
	m_Pieces[king_coordinates.x][king_coordinates.y] = nullptr;
	m_Pieces[new_king_coordinates.x][new_king_coordinates.y]->SetPos(new_king_coordinates.x, new_king_coordinates.y);

}