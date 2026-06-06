#pragma once
#include <cstdint>
#include "../CUnitInfo/CUnitInfo.h"
#include "../CPlayer/CPlayer.h" 
#include "../../memory/Memory.h"
#include "../offsets.h"

//class CPlayer; // FUCKING C++ MODULES IDK HOW THIS WORKS I'M IN 8 CLASS HELP ME @Mevasik 2024

class CUnit
{
public:

    std::uintptr_t base_address;
    Vector3 position;
    Vector3 boundsmin;
    Vector3 boundsmax;
    Matrix3x3 rotation_matrix;
    
    bool invul_state;
    std::uint8_t reload_progress ;
    std::uint8_t team_num ;
    std::uint16_t unit_state ;
    CUnitInfo unit_info;
    CPlayer player;

    CUnit(const std::uintptr_t base_address = 0)
        : base_address(base_address)
    {
    }


    void read_position() {
        position = mem.Read<Vector3>(base_address + offsets::_CUnit::position);
    }
    void read_position_scatter_request(VMMDLL_SCATTER_HANDLE handle){
        mem.AddScatterReadRequest(handle,base_address + offsets::_CUnit::position,&position);
    }

    void read_boundsmin() {
        boundsmin = mem.Read<Vector3>(base_address + offsets::_CUnit::boundsmin);
    }
    void read_boundsmin_scatter_request(VMMDLL_SCATTER_HANDLE handle) {
        mem.AddScatterReadRequest(handle,base_address + offsets::_CUnit::boundsmin, &boundsmin);
    }
    void read_boundsmax() {
        boundsmax = mem.Read<Vector3>(base_address + offsets::_CUnit::boundsmax);
    }
    void read_boundsmax_scatter_request(VMMDLL_SCATTER_HANDLE handle) {
        mem.AddScatterReadRequest(handle, base_address + offsets::_CUnit::boundsmax, &boundsmax);
    }
    void read_rotation_matrix() {
        rotation_matrix = mem.Read<Matrix3x3>(base_address + offsets::_CUnit::rotation_matrix);
    }

    void read_reload_progress() {
        reload_progress = mem.Read<std::uint8_t>(base_address + offsets::_CUnit::reload_progress);
    }

    void read_invul_state() {
        invul_state = mem.Read<std::uint8_t>(base_address + offsets::_CUnit::invulnerable_offset);
    }
    void read_invul_state_scatter_request(VMMDLL_SCATTER_HANDLE handle) {
        mem.AddScatterReadRequest(handle, base_address + offsets::_CUnit::invulnerable_offset, &invul_state);
    }
    void read_team_num() {
        team_num = mem.Read<std::uint8_t>(base_address + offsets::_CUnit::team_num);
    }
    void read_team_num_scatter_request(VMMDLL_SCATTER_HANDLE handle) {
        mem.AddScatterReadRequest(handle,base_address + offsets::_CUnit::team_num,&team_num);
    }
    void read_unit_state() {
        unit_state = mem.Read<std::uint16_t>(base_address + offsets::_CUnit::unit_state);
    }
    void read_unit_state_scatter_request(VMMDLL_SCATTER_HANDLE handle) {
        mem.AddScatterReadRequest(handle, base_address + offsets::_CUnit::unit_state, &unit_state);
    }

    void read_unit_info() {
        unit_info = CUnitInfo(mem.Read<std::uintptr_t>(base_address + offsets::_CUnit::unit_info));
    }

    void read_player() {
        player = CPlayer(mem.Read<std::uintptr_t>(base_address + offsets::_CUnit::player_ptr));
    }

    matrix4x4 dm_transform(const std::uintptr_t dm_transform_ptr, const int idx)
    {
        return mem.Read<matrix4x4>(dm_transform_ptr + idx);
    }


    bool is_valid() const {
        return base_address != 0;
    }


    bool is_alive() const {
        if (unit_state >= 2)
            return false;

        //if (!unit_info.is_valid())
        //    return false;

        //if (!unit_info.vehicle_info.is_valid())
        //    return false;


            if (player.gui_state != GuiState::ALIVE)
                return false;
        

        return true;
    }


};
