#include "pch.h"
#include "Esp.h"
#include "ConfigInstance.h"

void Esp::DrawPlayerEsp(ImDrawList* draw_list, const CCamera& camera, UnitHandler* unit_handler,
	const CUnit& local_unit, int screen_w, int screen_h)
{
	if (!ConfigInstance.Player_ESP.Enable)
		return;

	ImFont* font = ImGui::GetFont();

	unit_handler->complete_units_mutex.lock();
	std::vector<CUnit> units = unit_handler->complete_units;
	unit_handler->complete_units_mutex.unlock();

	for (const auto& unit : units)
	{
		if (unit.position.IsZero())
			continue;

		const Vector2 screenpos = CCamera::world_to_screen(unit.position, camera.view_matrix, screen_w, screen_h);
		if (screenpos.IsZero())
			continue;

		const int distance = static_cast<int>(Vector3::Distance(unit.position, local_unit.position));
		if (ConfigInstance.Player_ESP.Max_distance != 0 && distance > ConfigInstance.Player_ESP.Max_distance)
			continue;

		std::string displayText;
		if (ConfigInstance.Player_ESP.Show_unit_name)
			displayText += unit.unit_info.vehicle_info.vehicle_name;
		if (ConfigInstance.Player_ESP.Show_distanse)
			displayText += " " + std::to_string(distance) + " m";
		if (ConfigInstance.Player_ESP.show_invul_state && unit.invul_state)
			displayText += "\n invul!";

		const ImVec2 text_size = ImGui::CalcTextSize(displayText.c_str());
		const ImVec2 text_pos(screenpos.x - text_size.x * 0.5f, screenpos.y);
		draw_list->AddText(font, ConfigInstance.Player_ESP.font_size, text_pos,
			ConfigInstance.Player_ESP.text_color, displayText.c_str());

		if (!ConfigInstance.Player_ESP.Enable_simple_box)
			continue;

		const Vector3 worldMin = unit.position + unit.boundsmin;
		const Vector3 worldMax = unit.position + unit.boundsmax;
		const std::vector<Vector3> worldCorners = {
			Vector3(worldMin.x, worldMin.y, worldMin.z),
			Vector3(worldMax.x, worldMin.y, worldMin.z),
			Vector3(worldMin.x, worldMax.y, worldMin.z),
			Vector3(worldMax.x, worldMax.y, worldMin.z),
			Vector3(worldMin.x, worldMin.y, worldMax.z),
			Vector3(worldMax.x, worldMin.y, worldMax.z),
			Vector3(worldMin.x, worldMax.y, worldMax.z),
			Vector3(worldMax.x, worldMax.y, worldMax.z)
		};

		Vector2 screenMin(FLT_MAX, FLT_MAX);
		Vector2 screenMax(-FLT_MAX, -FLT_MAX);
		bool anyVisible = false;

		for (const Vector3& corner : worldCorners)
		{
			const Vector2 cornerScreen = CCamera::world_to_screen(corner, camera.view_matrix, screen_w, screen_h);
			if (cornerScreen.IsZero())
				continue;

			anyVisible = true;
			screenMin.x = std::min(screenMin.x, cornerScreen.x);
			screenMin.y = std::min(screenMin.y, cornerScreen.y);
			screenMax.x = std::max(screenMax.x, cornerScreen.x);
			screenMax.y = std::max(screenMax.y, cornerScreen.y);
		}

		if (anyVisible)
		{
			draw_list->AddRect(
				ImVec2(screenMin.x, screenMin.y),
				ImVec2(screenMax.x, screenMax.y),
				ConfigInstance.Player_ESP.box_color,
				0.0f, 0,
				ConfigInstance.Player_ESP.box_thickness
			);
		}
	}
}