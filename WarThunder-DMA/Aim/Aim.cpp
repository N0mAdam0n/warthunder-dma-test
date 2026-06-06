#include "Pch.h"
#include "Aim.h"
#include "ConfigInstance.h"
#include "GUI_main_menu.h"
#include "kmboxNet.h"


#define _USE_MATH_DEFINES
#include <math.h>
bone_pos Aim::closest_point_to_center;


void Aim::UpdateThread(Aim* instanse)
{
    printf("[Aim] Thread started\n");
    while (true)
    {
        //if (ConfigInstance.Aim.kmbox_aim_enable == true)
        //{
        //    
        //    
        //    do_kmbox_aim();

        //}
    }
}

void Aim::do_kmbox_aim()
{

    //https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes

}

void Aim::DrawAim(ImDrawList* draw_list)
{

    //draw_list->AddCircle(ImVec2(Window::window_center.x, Window::window_center.y), ConfigInstance.Aim.aim_circle_radius, 
    //    ConfigInstance.Aim.circle_color, 0, 2.0f);
    //draw_list->AddCircleFilled(ImVec2(closest_point_to_center.screen_pos.x,closest_point_to_center.screen_pos.y), 2.0f, IM_COL32(255, 0, 0, 255));
}

bool Aim::is_point_between_circle_edges(const Vector2& point, const Vector2& center, float radius, float leftOffset, float rightOffset)
{

    return (point.x >= center.x - radius) && (point.x <= center.x + radius);
}

bool Aim::is_point_inside_circle(const Vector2& point, const Vector2& center, float radius)
{
   
    float distanceSquared = (point.x - center.x) * (point.x - center.x) +
        (point.y - center.y) * (point.y - center.y);


    return distanceSquared <= radius * radius;
}

bone_pos Aim::find_closest_point_to_center(const std::vector<bone_pos>& bones, const Vector2& center)
{
    if (bones.empty()) {
      
        return bone_pos{ Vector2{0, 0}, Vector3{0, 0, 0} };
    }


    bone_pos closestPoint = bones.at(0);
    float minDistanceSquared = std::pow(bones.at(0).screen_pos.x - center.x, 2) +
        std::pow(bones.at(0).screen_pos.y - center.y, 2);


    for (size_t i = 1; i < bones.size(); ++i) {
        float distanceSquared = std::pow(bones.at(i).screen_pos.x - center.x, 2) +
            std::pow(bones.at(i).screen_pos.y - center.y, 2);

        if (distanceSquared < minDistanceSquared) {
            minDistanceSquared = distanceSquared;
            closestPoint = bones.at(i);
        }
    }

    return closestPoint;
}
void Aim::move_mouse_from_center_to_point(Vector2 point)
{
    int x_offset = point.x - Window::window_center.x;
    int y_offset = point.y - Window::window_center.y;
    kmNet_enc_mouse_move_auto(x_offset, y_offset, 100);
}
