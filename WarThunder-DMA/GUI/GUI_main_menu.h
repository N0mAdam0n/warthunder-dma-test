#pragma once

#include <Windows.h>
#include "Vector.h"

class Warthunder;
class UnitHandler;

enum class WindowRunResult
{
	Success,
	UserExit,
	InitFailed
};

class Window
{
public:
	static int screenWidth;
	static int screenHeight;
	static Vector2 window_center;
	static WindowRunResult CreateAndRunWindow(HINSTANCE hInstance, Warthunder* wt, UnitHandler* unit_handler);
};