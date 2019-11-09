#pragma once
#include "../rendering/Window.h"

class Input
{
public:
	Input(Window& t_window) : m_window(t_window){}

	Window& m_window;
};
