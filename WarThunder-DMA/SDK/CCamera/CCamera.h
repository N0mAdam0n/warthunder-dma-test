#pragma once
#include <cstdint>


class CCamera
{
public:
	CCamera(const std::uintptr_t base_address = 0) : base_address(base_address) {}

	const std::uintptr_t operator=(const std::uintptr_t base_address) {
		return this->base_address = base_address;
	}
	ViewMatrix view_matrix;
	Vector3 position;
	auto is_valid() -> const bool;
	void set_view_matrix();
	void set_position();

	static Vector2 world_to_screen(const Vector3 Position, const ViewMatrix& view_matrix);

private:
	std::uintptr_t base_address;
};

