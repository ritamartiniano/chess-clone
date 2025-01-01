#pragma once
#include <SDL.h>

class IInputListener
{
public:

	virtual ~IInputListener() {}
	virtual void OnMouseDown(SDL_Event* InEvent){}
	virtual void OnKeyUp(SDL_Scancode key_code){}
};

