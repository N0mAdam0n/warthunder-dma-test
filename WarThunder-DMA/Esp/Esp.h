#pragma once

#include <imgui.h>
#include "../SDK/CCamera/CCamera.h"
#include "handlers/unit_handler/unit_handler.h"
#include "../SDK/Warthunder.h"
class Esp
{
public:

	/*void draw_damage_model(ImDrawList* draw_list, DamageModelContainer& damage_model_container, CUnit& unit, const std::uintptr_t& damage_model_transform_ptr, const Matrix3x3& unit_rotation, const Vector3& unit_position, const ViewMatrix& view_matrix, const ImU32 color);*/

	static void DrawPlayerEsp(ImDrawList* draw_list, CCamera camera, UnitHandler* unit_handler, CUnit local_unit);
	//static void DrawLootEsp(ImDrawList* draw_list, const Camera& camera);
	//static void DrawMiscEsp(ImDrawList* draw_list, const Camera& camera);
	static const ImU32 white = IM_COL32(255, 255, 255, 255);
	static const ImU32 blue = IM_COL32(0, 128, 255, 255);
	static const ImU32 red = IM_COL32(255, 0, 0, 255);
private:
	//const float font_size = 20.0f;

};


