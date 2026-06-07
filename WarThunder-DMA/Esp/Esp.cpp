//Esp.cpp
#include "pch.h"
#include "Esp.h"
#include "ConfigInstance.h"

#include <GUI_main_menu.h>
#include "../handlers/unit_handler/unit_handler.h"



void Esp::DrawPlayerEsp(ImDrawList* draw_list, CCamera camera, UnitHandler *unit_handler,CUnit local_unit)
{
    ImFont* font = ImGui::GetFont();
    if (ConfigInstance.Player_ESP.Enable)
    {
        unit_handler->complete_units_mutex.lock();
        std::vector<CUnit> units = unit_handler->complete_units;
        unit_handler->complete_units_mutex.unlock();

        for (auto& unit : units)
        {
            if (unit.position.IsZero())
            {
                continue;
            }
            Vector2 screenpos = CCamera::world_to_screen(unit.position,camera.view_matrix);
            if (screenpos.IsZero()) {
                continue;
            }
            int distance = Vector3::Distance(unit.position, local_unit.position);
            if (distance > ConfigInstance.Player_ESP.Max_distance && (ConfigInstance.Player_ESP.Max_distance != 0))
            {
                continue;
            }
            std::string displayText;
            if (ConfigInstance.Player_ESP.Show_unit_name)
            {
                displayText +=  unit.unit_info.vehicle_info.vehicle_name;
            }
            if (ConfigInstance.Player_ESP .Show_distanse) {
                displayText += " " + std::to_string(static_cast<int>(distance)) + " m";
            }
            if (ConfigInstance.Player_ESP.show_invul_state)
            {
                if (unit.invul_state)
                {
                    displayText += "\n invul!";
                }
            }
            ImVec2 text_size = ImGui::CalcTextSize(displayText.c_str());

            ImVec2 text_pos = ImVec2(screenpos.x - text_size.x / 2.0f, screenpos.y /*- text_size.y / 2.0f*/);

            
            draw_list->AddText(font, ConfigInstance.Player_ESP.font_size, text_pos, ConfigInstance.Player_ESP.text_color, displayText.c_str());

            if (ConfigInstance.Player_ESP.Enable_simple_box)
            {

                Vector3 worldMin = unit.position + unit.boundsmin;
                Vector3 worldMax = unit.position + unit.boundsmax;


                std::vector<Vector3> worldCorners = {
                    Vector3(worldMin.x, worldMin.y, worldMin.z),
                    Vector3(worldMax.x, worldMin.y, worldMin.z),
                    Vector3(worldMin.x, worldMax.y, worldMin.z),
                    Vector3(worldMax.x, worldMax.y, worldMin.z),
                    Vector3(worldMin.x, worldMin.y, worldMax.z),
                    Vector3(worldMax.x, worldMin.y, worldMax.z),
                    Vector3(worldMin.x, worldMax.y, worldMax.z),
                    Vector3(worldMax.x, worldMax.y, worldMax.z)
                };


                Vector2 screenMin = Vector2(FLT_MAX, FLT_MAX);
                Vector2 screenMax = Vector2(-FLT_MAX, -FLT_MAX);
                bool anyVisible = false;


                for (Vector3 corner : worldCorners)
                {
                    Vector2 screenPos = CCamera::world_to_screen(corner, camera.view_matrix);
                    if (screenPos.IsZero()) continue; 

                    anyVisible = true;


                    screenMin.x = std::min(screenMin.x, screenPos.x);
                    screenMin.y = std::min(screenMin.y, screenPos.y);
                    screenMax.x = std::max(screenMax.x, screenPos.x);
                    screenMax.y = std::max(screenMax.y, screenPos.y);
                }

  
                if (anyVisible) {
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
    }
    
   





}