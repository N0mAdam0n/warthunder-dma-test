#pragma once
#include <imgui.h>
#include "../SDK/CCamera/CCamera.h"
#include "handlers/unit_handler/unit_handler.h"
#include "../SDK/CUnit/CUnit.h"

class Esp
{
public:
	static void DrawPlayerEsp(ImDrawList* draw_list, const CCamera& camera, UnitHandler* unit_handler,
		const CUnit& local_unit, int screen_w, int screen_h);
};