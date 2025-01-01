#include "MoveGenerator.h"

#include <iostream>

#include "ChessBoard.h"
#include "ChessHelpers.h"
#include "MoveEvaluation.h"
#include "Piece.h"

void MoveGenerator::GeneratePsudoLegalMoves(std::vector<Move>& moves, const BoardPieces& all_pieces, PieceTeam piece_team)
{
    for (int x = 0; x < BOARD_SIZE; x++)
    {
        for (int y = 0; y < BOARD_SIZE; y++)
        {
            if(Piece* current_piece = all_pieces[x][y])
            {
                if(current_piece->GetTeam() == piece_team)
                {
                    if(current_piece->FindPossibleMoves(all_pieces, {x,y}))
                    {
                        const std::vector<PieceCoordinates>& possible_moves = current_piece->GetPossibleMoves();
                        moves.reserve(possible_moves.size());
                        for (const auto& possible_move : possible_moves)
                        {
                            moves.emplace_back(PieceCoordinates(x,y),possible_move, piece_team);
                        }
                    }
                }
            }
        }
    }
}

void MoveGenerator::ParseLegalMoves(ChessBoard* board, std::vector<Move>& moves, const BoardPieces& all_pieces, PieceTeam piece_team)
{
    std::vector<Move> pseudo_legal_moves;
    GeneratePsudoLegalMoves(pseudo_legal_moves, all_pieces, piece_team);
    for(auto& pseudo_legal_move : pseudo_legal_moves)
    {
        board->SimulateMove(pseudo_legal_move);
        if(IsValidMove(pseudo_legal_move,board, all_pieces, piece_team))
        {
            moves.emplace_back(pseudo_legal_move);
        }
        board->UnDoSimulateMove(pseudo_legal_move);
    }
}

Move MoveGenerator::GenerateRandomMove(ChessBoard* board, const BoardPieces& all_pieces, PieceTeam piece_team)
{
    std::vector<Move> legal_moves;
    ParseLegalMoves(board, legal_moves, all_pieces, piece_team);
    int best_score = 0;
    int best_move_index = 0;
    int index =0;
    for(auto& pseudo_legal_move : legal_moves)
    {
        board->SimulateMove(pseudo_legal_move);
        int score = AlphaBetaMax(board,all_pieces, PieceTeam::White, INT_MAX, INT_MIN, 1);
        if(best_score >best_move_index)
        {
            best_score = score;
            best_move_index = index;
        }
        board->UnDoSimulateMove(pseudo_legal_move);
        index++;
    }
    //int random_move = rand() % legal_moves.size();
    return legal_moves[best_move_index];
}

bool MoveGenerator::IsValidMove(const Move& move,const ChessBoard* board, const BoardPieces& pieces, PieceTeam piece_team)
{
    PieceType king_piece = piece_team == PieceTeam::Black ? PieceType::B_King : PieceType::W_Pawn;
    PieceCoordinates king_coordinates = board->GetAllPiecesCoordinates().at(king_piece);
    return board->IsKingInCheck(piece_team, king_coordinates) == false;
}

int MoveGenerator::AlphaBetaMax(ChessBoard* board, const BoardPieces& board_pieces, PieceTeam piece_team, int alpha, int beta, int depth)
{
    MoveEvaluation evaluation;
    if (depth == 0)
    {
        return evaluation.EvaluateMove(board, piece_team);
    }

    int best_value = -INT_MAX;
    std::vector<Move> possible_moves;
    piece_team = ReverseTeam(piece_team);
    ParseLegalMoves(board, possible_moves, board_pieces, piece_team);

    for (auto& move : possible_moves)
    {
        board->SimulateMove(move);
        int score = AlphaBetaMin(board, board_pieces, piece_team, alpha, beta, depth - 1);
        board->UnDoSimulateMove(move);

        if (score > best_value)
        {
            best_value = score;
        }
        if (score > alpha)
        {
            alpha = score;
        }
        if (score >= beta)
        {
            return score;
        }
    }

    return best_value;
}

int MoveGenerator::AlphaBetaMin(ChessBoard* board, const BoardPieces& board_pieces, PieceTeam piece_team, int alpha, int beta, int depth)
{
    MoveEvaluation evaluation;
    if (depth == 0)
    {
        return evaluation.EvaluateMove(board, ReverseTeam(piece_team));
    }

    int best_value = INT_MAX;
    std::vector<Move> possible_moves;
    piece_team = ReverseTeam(piece_team);
    ParseLegalMoves(board, possible_moves, board_pieces, piece_team);

    for (auto& move : possible_moves)
    {
        board->SimulateMove(move);
        int score = AlphaBetaMax(board, board_pieces, piece_team, alpha, beta, depth - 1);
        board->UnDoSimulateMove(move);

        if (score < best_value)
        {
            best_value = score;
        }
        if (score < beta)
        {
            beta = score;
        }
        if (score <= alpha)
        {
            return score;
        }
    }

    return best_value;
}
