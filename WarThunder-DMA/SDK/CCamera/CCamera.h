#pragma once
#include <cstdint>

class CCamera
{
public:
	CCamera(const std::uintptr_t base_address = 0) : base_address(base_address) {}

	const std::uintptr_t operator=(const std::uintptr_t addr) {
		return this->base_address = addr;
	}

	ViewMatrix view_matrix;

	auto is_valid() -> const bool;
	void set_view_matrix();

	static Vector2 world_to_screen(const Vector3& position, const ViewMatrix& view_matrix, int screen_w, int screen_h);

private:
	std::uintptr_t base_address = 0;
};