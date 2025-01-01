#pragma once
#include <vector>

#include "PieceType.h"

class ChessBoard;

class MoveGenerator
{
public:
    void GeneratePsudoLegalMoves(std::vector<Move>& moves, const BoardPieces& pieces, PieceTeam piece_team);
    void ParseLegalMoves(ChessBoard* board, std::vector<Move>& moves, const BoardPieces& all_pieces, PieceTeam piece_team);
    Move GenerateRandomMove(ChessBoard* board,const BoardPieces& all_pieces, PieceTeam piece_team);
    bool IsValidMove(const Move& move, const ChessBoard* board, const BoardPieces& pieces, PieceTeam piece_team);
    //Assumes move is already simulated on the board
    int EvaluateMove(const ChessBoard* chess_board,PieceTeam piece_team);
    int AlphaBetaMax(ChessBoard* board, const BoardPieces& board_pieces, PieceTeam piece_team, int alpha, int beta, int depth);
    int AlphaBetaMin(ChessBoard* board, const BoardPieces& board_pieces, PieceTeam piece_team, int alpha, int beta, int depth);

    inline PieceTeam ReverseTeam(PieceTeam team)
    {
        if(team == PieceTeam::White)
        {
            return PieceTeam::Black;
        }
        if(team == PieceTeam::White)
        {
            return PieceTeam::Black;
        }

        return PieceTeam::Count;
    }

};
