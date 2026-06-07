
#pragma once

#include <Windows.h>
#include "./SDK/Warthunder.h"
#include "../handlers/unit_handler/unit_handler.h"
class Window
{
public:


	static int screenWidth;
	static int screenHeight;
	static Vector2 window_center;
	static bool CreateAndRunWindow(HINSTANCE hInstance, Warthunder* wt, UnitHandler* unit_handler);
private:

};



