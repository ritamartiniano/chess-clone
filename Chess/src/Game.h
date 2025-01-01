#pragma once
#include "ChessBoard.h"
#include <memory>

#include "AIPlayer.h"
#include "HumanPlayer.h"

class Game
{
public:
	Game();
	~Game();
	void Update();
	void CleanUp();
private:
	bool IsRunning();
	void Render();
	void HandleEvents();

	std::unique_ptr<ChessBoard> m_ChessBoard;
	SDL_Window* m_Window;
	SDL_Renderer* m_Renderer;
	SDL_Texture* m_BoardTexture;
	HumanPlayer m_Player1;
	HumanPlayer m_Player2;
	bool b_IsHumanPlayerTurn{false};
	bool m_Running{false};
};

