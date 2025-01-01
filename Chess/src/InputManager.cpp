#include "InputManager.h"

#include <iostream>

InputManager InputManager::m_InputManager;

InputManager* InputManager::GetInstance()
{
	return &InputManager::m_InputManager;
}

void InputManager::OnEvent(SDL_Event* InEvent)
{
	if (InEvent->type == SDL_MOUSEBUTTONDOWN)
	{
		for (const auto listener : m_InputListeners)
		{
			listener->OnMouseDown(InEvent);
		}
	}
	else if(InEvent->type == SDL_KEYUP)
	{
		for(const auto listener : m_InputListeners)
		{
			std::cout <<SDL_GetKeyName(InEvent->key.keysym.sym) << std::endl;
			listener->OnKeyUp(InEvent->key.keysym.scancode);
		}
	}
}

void InputManager::AddMouseListener(IInputListener* inputListener)
{
	if (inputListener)
		m_InputListeners.push_back(inputListener);
}

void InputManager::RemoveMouseListener(IInputListener* inputListener)
{
	const std::vector<IInputListener*>::iterator position = std::find(m_InputListeners.begin(), m_InputListeners.end(), inputListener);

	if (position != m_InputListeners.end())
		m_InputListeners.erase(position);
}

InputManager::InputManager()
{
}

InputManager::~InputManager()
{
	m_InputListeners.clear();
}
