#pragma once
#include <cstdint>
#include <string>
#include "../../Memory/Memory.h"
#include "../offsets.h"
#include "../globals/globals.h"

class CVehicleInfo
{
public:
	std::uintptr_t base_address = 0;
	std::string vehicle_name;

	explicit CVehicleInfo(const std::uintptr_t addr = 0) : base_address(addr) {}

	void set_vehicle_name() {
		vehicle_name = globals::ReadString(base_address + offsets::_CVehiceInfo::vehicle_name, 64);
	}

	bool is_valid() const { return base_address != 0; }
};