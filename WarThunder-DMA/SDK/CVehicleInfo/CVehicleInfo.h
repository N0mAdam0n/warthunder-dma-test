#pragma once
#include "pch.h"
#include "../../memory/Memory.h"
#include "../offsets.h"
#include "../globals/globals.h"
class CVehicleInfo
{
public:
    std::uintptr_t base_address = 0;
    std::string vehicle_name;

    CVehicleInfo(const std::uintptr_t base_address = 0)
        : base_address(base_address)
    {
    }

    void set_vehicle_name() {
        vehicle_name = globals::ReadString(base_address + offsets::_CVehiceInfo::vehicle_name, 64);
    }

    bool is_valid() const {
        return base_address != 0;
    }
};
