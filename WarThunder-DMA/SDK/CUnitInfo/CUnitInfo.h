#pragma once
#include <cstdint>
#include <string>
#include "../CVehicleInfo/CVehicleInfo.h"
#include "../../memory/Memory.h"
#include "../offsets.h"
#include "../globals/globals.h"

class CUnitInfo
{
public:
    std::uintptr_t base_address = 0;
    bool bombsight_local_enabled = false;
    std::string unit_type_str;
    CVehicleInfo vehicle_info;
    bool plane = false;
    bool dummy = false;

    CUnitInfo(const std::uintptr_t base_address = 0)
        : base_address(base_address)
    {
    }


    void set_bombsight_local() {
        bombsight_local_enabled = mem.Read<bool>(base_address + offsets::_CUnitInfo::bombsight);
    }

    void set_unit_type() {
        unit_type_str = globals::ReadString(mem.Read<std::uintptr_t>(base_address + offsets::_CUnitInfo::unit_type), 30);
    }

    void set_vehicle_info() {
        vehicle_info = CVehicleInfo(mem.Read<std::uintptr_t>(base_address + offsets::_CUnitInfo::tank_info));
    }

    void set_plane() {
        plane = (unit_type_str == "exp_bomber" || unit_type_str == "exp_assault" || unit_type_str == "exp_fighter");
    }

    void set_dummy() {
        dummy = (unit_type_str == "exp_fortification" || unit_type_str == "exp_structure" ||
            unit_type_str == "exp_aaa" || unit_type_str == "dummy_plane" || unit_type_str == "exp_bridge");
    }

    void update_all() {
        set_bombsight_local();
        set_unit_type();
        set_vehicle_info();
        set_plane();
        set_dummy();
    }

    bool is_valid() const {
        return base_address != 0;
    }
};
