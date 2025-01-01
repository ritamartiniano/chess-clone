#pragma once
#include <array>
#include <stack>

#include "Piece.h"
#include "InputListener.h"

class ChessBoard
{
public:
	ChessBoard();
	~ChessBoard();
	void Update();
	void Draw(SDL_Renderer* renderer);
	const BoardPieces& GetPieces() const;
	Piece* GetPieceClickedOn(int x, int y) const;
	void MovePiece(Move& move);
	void SimulateMove(Move& move);
	void UnDoSimulateMove(const Move& move);
	void UndoLastMove();
	const PieceTypeCountMap& GetPieceTypeCountMap() const;
	const AllPieceCoordinates& GetAllPiecesCoordinates() const;
	void EndTurn();
	bool IsWhiteTeamTurn() const;
	bool IsBlackTeamTurn() const;
	bool IsKingInCheck(PieceTeam piece_team, const PieceCoordinates& king_coordinates) const;
	bool IsKingInCheckMate(PieceTeam piece_team, const PieceCoordinates& king_coordinates);
	
private:
	void UndoMovePiece(const Move& move);
	void SetupPieces();
	void CachePiecesInfo();
	void HandleEnPassant(Move& move);
	void HandleCastling(Move& move);
	void UndoCasltingMove(const Move& move);
	void UndoEnPassant(const Move& move);
	bool CheckSlidingPiecesCheckingKing(PieceTeam piece_team, const PieceCoordinates& king_coordinates) const;
	BoardPieces m_Pieces;
	std::stack<Move> m_MovesStack;
	PieceTypeCountMap m_PieceTypeCountMap;
	AllPieceCoordinates m_AllPieceCoordinates;
	bool b_IsWhiteTeamTurn{true};
	bool b_IsWhiteKingInCheckMate{false};
	bool b_IsBlackKingInCheckMate{false};
};

