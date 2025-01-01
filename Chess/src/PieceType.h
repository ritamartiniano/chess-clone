#pragma once
#include <array>
#include <unordered_map>

class Piece;

constexpr static int PIECE_SIZE = 60;
constexpr static int SCREEN_SIZE = 480;
constexpr int BOARD_SIZE = 8;
constexpr static int INVALID_COORDINATE = -8;
#define INVALID INVALID_COORDINATE;
struct PieceCoordinates
{
	PieceCoordinates():x(-1),y(-1){}
	PieceCoordinates(int x_, int y_) : x(x_), y(y_) {}
	int x = 0;
	int y = 0;
	PieceCoordinates operator+(const PieceCoordinates& coordinates) const
	{
		return {x + coordinates.x, y + coordinates.y};
	}
};

typedef std::array<std::array<Piece*, BOARD_SIZE>,BOARD_SIZE> BoardPieces;

enum class PieceType
{
   B_Bishop,
   B_Horse,
   B_King,
   B_Pawn,
   B_Queen,
   B_Turret,
   W_Bishop,
   W_Horse,
   W_King,
   W_Pawn,
   W_Queen,
   W_Turret,
   MAX,
};

enum class PieceTeam
{
	Black,
	White,
	Count
};

enum class SpecialMoveType
{
	EnPassant,
	Castling,
	PawnPromotion,
	None
};

enum class Direction
{
	Left,
	Right,
	Up,
	Down,
	TopRight,
	TopLeft,
	BottomRight,
	BottomLeft,
	Count
};

const static std::array direction_coordinates = {
						PieceCoordinates(0, -1), //left
						PieceCoordinates(0, 1), //right
						PieceCoordinates(-1, 0), //up
						PieceCoordinates(1, 0), //down
						PieceCoordinates(-1, 1), //top_right
						PieceCoordinates(-1, -1), //top_left
						PieceCoordinates(1, 1),//bottom_right
						PieceCoordinates(1,-1) //bottom_left
};	

struct Move
{
	Move(PieceCoordinates start, PieceCoordinates end_move, PieceTeam team)
		: m_StartMove(start), m_TargetMove(end_move), m_Team(team)
	{
	}
	Piece* m_OponentRemovedPiece{nullptr};
	PieceCoordinates m_StartMove{-1,-1};
	PieceCoordinates m_TargetMove{-1,-1};
	SpecialMoveType m_SpecialMoveType{SpecialMoveType::None};
	PieceTeam m_Team{PieceTeam::Count}; // team that performed the move
};

typedef std::unordered_map<PieceType, PieceCoordinates> AllPieceCoordinates;
typedef std::array<int, static_cast<size_t>(PieceType::MAX)> PieceTypeCountMap;