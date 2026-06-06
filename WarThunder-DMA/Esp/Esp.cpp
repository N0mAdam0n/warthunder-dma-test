//Esp.cpp
#include "pch.h"
#include "Esp.h"
#include "ConfigInstance.h"

#include <GUI_main_menu.h>
#include "../handlers/unit_handler/unit_handler.h"



//void Esp::draw_damage_model(ImDrawList* draw_list,
//    DamageModelContainer& damage_model_container,
//    CUnit& unit,
//    const std::uintptr_t& damage_model_transform_ptr,
//    const Matrix3x3& unit_rotation,
//    const Vector3& unit_position,
//    const ViewMatrix& view_matrix,
//    const ImU32 color)
//{
//    static std::vector<ImVec2> vertices_2d;
//    vertices_2d.clear();
//
//    const auto damage_model_transform = unit.dm_transform(damage_model_transform_ptr, damage_model_container.transform_index << 6);
//
//    for (const auto& vertex : damage_model_container.vertices) {
//        const Vector3 damage_model_position = (vertex * damage_model_transform).ToVector3() * unit_rotation + unit_position;
//
//        Vector2 damage_model_position_2d = CCamera::world_to_screen(damage_model_position, view_matrix);
//            vertices_2d.push_back(ImVec2{ damage_model_position_2d.x, damage_model_position_2d.y });
//        
//    }
//
//    if (vertices_2d.size() == damage_model_container.vertices.size()) {
//        const auto& indices = damage_model_container.indices;
//        const size_t num_indices = indices.size();
//
//        for (size_t i = 0; i < num_indices; i += 3) {
//            const uint16_t index1 = indices[i];
//            const uint16_t index2 = indices[i + 1];
//            const uint16_t index3 = indices[i + 2];
//
//            if (index1 < vertices_2d.size() && index2 < vertices_2d.size() && index3 < vertices_2d.size()) {
//                const ImVec2& pos1 = vertices_2d[index1];
//                const ImVec2& pos2 = vertices_2d[index2];
//                const ImVec2& pos3 = vertices_2d[index3];
//
//                draw_list->AddTriangleFilled(pos1, pos2, pos3, color);
//            }
//        }
//    }
//}

void Esp::DrawPlayerEsp(ImDrawList* draw_list, CCamera camera, UnitHandler *unit_handler,CUnit local_unit)
{
    ImFont* font = ImGui::GetFont();
    if (ConfigInstance.Player_ESP.Enable)
    {
        unit_handler->complete_units_mutex.lock();
        std::vector<CUnit> temp_units = unit_handler->complete_units;
        unit_handler->complete_units_mutex.unlock();

        for (auto& unit : temp_units)
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