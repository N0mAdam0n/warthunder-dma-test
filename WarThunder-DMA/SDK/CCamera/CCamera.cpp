#include "pch.h"
#include "CCamera.h"
#include "../../Memory/Memory.h"
#include "../offsets.h"

auto CCamera::is_valid() -> const bool
{
	return this->base_address != 0;
}

void CCamera::set_view_matrix()
{
	view_matrix = mem.Read<ViewMatrix>(this->base_address + offsets::_CCamera::camera_viewmatrix);
}

Vector2 CCamera::world_to_screen(const Vector3& position, const ViewMatrix& view_matrix, int screen_w, int screen_h)
{
	const float x = position.x * view_matrix.matrix[0][0] + position.y * view_matrix.matrix[1][0]
		+ position.z * view_matrix.matrix[2][0] + view_matrix.matrix[3][0];
	const float y = position.x * view_matrix.matrix[0][1] + position.y * view_matrix.matrix[1][1]
		+ position.z * view_matrix.matrix[2][1] + view_matrix.matrix[3][1];
	const float w = position.x * view_matrix.matrix[0][3] + position.y * view_matrix.matrix[1][3]
		+ position.z * view_matrix.matrix[2][3] + view_matrix.matrix[3][3];

	if (w < 0.0001f)
		return Vector2();

	const float inv_w = 1.0f / w;
	const float half_w = screen_w * 0.5f;
	const float half_h = screen_h * 0.5f;

	return Vector2(
		half_w + (x * inv_w) * half_w,
		half_h - (y * inv_w) * half_h
	);
}