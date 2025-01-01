#pragma once

#include "InputListener.h"
#include <vector>

class InputManager
{
public:

	static InputManager* GetInstance();

	void OnEvent(SDL_Event* InEvent);
	void AddMouseListener(IInputListener* inputListener);
	void RemoveMouseListener(IInputListener* inputListener);
	InputManager(InputManager& other) = delete;

	void operator=(const InputManager&) = delete;
private:
	InputManager();
	virtual ~InputManager();
	static InputManager m_InputManager;
	std::vector<IInputListener*> m_InputListeners;
};

