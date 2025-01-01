#include "Game.h"
#include <iostream>
#include "TextureManager.h"
#include "PieceTextureFactory.h"
#include "InputManager.h"

Game::Game()
	:m_Player1(PieceTeam::White), m_Player2(PieceTeam::Black)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		std::cout << "Subsystems initialized" << std::endl;

		m_Window = SDL_CreateWindow("Chess", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 480, 480, SDL_WINDOW_SHOWN);
		m_Renderer = SDL_CreateRenderer(m_Window, -1, 0);
		if (m_Window)
		{
			std::cout << "window created" << std::endl;
		}
		if (m_Renderer)
		{
			std::cout << "renderer created" << std::endl;
		}

		PieceTextureFactory::LoadPieces(m_Renderer);
		m_BoardTexture = TextureManager::LoadTexture("Assets/board.png", m_Renderer);
		m_ChessBoard = std::make_unique<ChessBoard>();
		
		m_Player1.SetChessBoard(m_ChessBoard.get());
		m_Player2.SetChessBoard(m_ChessBoard.get());
		m_Running = true;
		b_IsHumanPlayerTurn = true;

	}
	else
	{
		m_Running = false;
	}
}

Game::~Game()
{
	CleanUp();
}

void Game::Update()
{
	while(IsRunning())
	{
		m_ChessBoard->Update();
		HandleEvents();
		Render();
	}
}

bool Game::IsRunning()
{
	return m_Running;
}

void Game::CleanUp()
{
	PieceTextureFactory::DestroyTextures();
	SDL_DestroyTexture(m_BoardTexture);
	SDL_DestroyRenderer(m_Renderer);
	SDL_DestroyWindow(m_Window);
	m_Renderer = nullptr;
	m_Window = nullptr;
}

void Game::Render()
{
	SDL_RenderClear(m_Renderer);
	SDL_Rect rect;
	rect.x = 0;
	rect.y = 0;
	rect.w = 480;
	rect.h = 480;
	SDL_RenderCopy(m_Renderer, m_BoardTexture, nullptr, &rect);
	m_Player1.Draw(m_Renderer);
	m_Player2.Draw(m_Renderer);
	m_ChessBoard->Draw(m_Renderer);
	SDL_RenderPresent(m_Renderer);
}

void Game::HandleEvents()
{	
	SDL_Event event;
	SDL_PollEvent(&event);
	switch (event.type)
	{
	case SDL_QUIT:
		m_Running = false;
		break;
	default:
		InputManager::GetInstance()->OnEvent(&event);
		break;
	}
}
