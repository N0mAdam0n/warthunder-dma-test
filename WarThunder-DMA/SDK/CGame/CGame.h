#pragma once
#include <cstdint>
#include "../CCamera/CCamera.h"
#include "../CUnitList/CUnitList.h"

class CGame
{
public:
	auto set_instance() -> const bool;
	auto get_unit_list3() -> CUnitList;
	auto get_unit_count3() -> const std::uint32_t;
	void set_local_camera();

	CCamera camera;

private:
	std::uintptr_t base_address = 0;
};