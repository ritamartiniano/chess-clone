#include "MoveEvaluation.h"

#include <iostream>

#include "ChessBoard.h"
#include "ChessHelpers.h"

int MoveEvaluation::EvaluateMove(const ChessBoard* chess_board, PieceTeam piece_team)
{
    const PieceTypeCountMap& piece_type_count_map = chess_board->GetPieceTypeCountMap();
    std::array<int, static_cast<size_t>(PieceTeam::Count)> teams_material;

    auto collect_team_material = [piece_type_count_map, &teams_material](PieceTeam piece_team)
    {
        const int queen_value = 900;
        const int horse_bishop_value = 300;
        const int turret_value = 500;
        const int pawn_value = 100;
        
        for(size_t i = 0; i < piece_type_count_map.size(); i++)
        {
            int piece_material = 0;
            const PieceType type = static_cast<PieceType>(i);
            if(piece_team == PieceTeam::White)
            {
                switch (type)
                {
                    case PieceType::W_Bishop:
                    case PieceType::W_Horse:
                    piece_material = piece_type_count_map[i] * horse_bishop_value;
                    break;
                    case PieceType::W_Pawn:
                        piece_material = piece_type_count_map[i] * pawn_value;
                    break;
                    case PieceType::W_Queen:
                        piece_material = piece_type_count_map[i] * queen_value;
                    break;
                    case PieceType::W_Turret:
                        piece_material = piece_type_count_map[i] * turret_value;
                    break;
                }
            }
            else if(piece_team == PieceTeam::Black)
            {
                switch (type)
                {
                case PieceType::B_Bishop:
                case PieceType::B_Horse:
                piece_material = piece_type_count_map[i] * horse_bishop_value;
                    break;
                case PieceType::B_Pawn:
                    piece_material = piece_type_count_map[i] * pawn_value;
                    break;
                case PieceType::B_Queen:
                    piece_material = piece_type_count_map[i] * queen_value;
                    break;
                case PieceType::B_Turret:
                    piece_material = piece_type_count_map[i] * turret_value;
                    break;
                }
            }
            teams_material[static_cast<size_t>(piece_team)] += piece_material;
        }
    };

    collect_team_material(PieceTeam::Black);
    collect_team_material(PieceTeam::White);
    int evaluation = teams_material[static_cast<size_t>(PieceTeam::Black)] - 
                    teams_material[static_cast<size_t>(PieceTeam::White)];
    std::cout << "Evaluation for back team: " << evaluation << std::endl;
    std::cout << "Evaluation for white team: " << evaluation * -1 << std::endl;
    return evaluation * (piece_team == PieceTeam::Black ? 1 : -1);
}
