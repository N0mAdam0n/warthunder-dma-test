#include "pch.h"
#include "CCamera.h"
#include "../../memory/Memory.h"
#include "../offsets.h"
#include "GUI_main_menu.h"  
auto CCamera::is_valid() -> const bool
{
	return this->base_address != 0;
}

void CCamera::set_view_matrix()
{
	view_matrix =  mem.Read<ViewMatrix>(this->base_address + offsets::_CCamera::camera_viewmatrix);
}

void CCamera::set_position()
{
	position=  mem.Read<Vector3>(this->base_address + offsets::_CCamera::camera_position);
}

Vector2 CCamera::world_to_screen(const Vector3 Position, const ViewMatrix& view_matrix)
{

    float x = Position.x * view_matrix.matrix[0][0] + Position.y * view_matrix.matrix[1][0] + Position.z * view_matrix.matrix[2][0] + view_matrix.matrix[3][0];
    float y = Position.x * view_matrix.matrix[0][1] + Position.y * view_matrix.matrix[1][1] + Position.z * view_matrix.matrix[2][1] + view_matrix.matrix[3][1];
    float w = Position.x * view_matrix.matrix[0][3] + Position.y * view_matrix.matrix[1][3] + Position.z * view_matrix.matrix[2][3] + view_matrix.matrix[3][3];

    if (w < 0.0001f)
        return Vector2(); 

    float inv_w = 1.0f / w;

    float screen_x = (Window::screenWidth / 2.0f) + (x * inv_w) * (Window::screenWidth / 2.0f);
    float screen_y = (Window::screenHeight / 2.0f) - (y * inv_w) * (Window::screenHeight / 2.0f);

    return Vector2(screen_x, screen_y);
}
