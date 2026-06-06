#pragma once
#include <cstdint>

#define NEW_PATCH 0

namespace offsets
{
	constexpr uintptr_t CGame_offset = 0x40;
#if NEW_PATCH == 1
	//CGame_offset from mem.memreport_sys
	extern uint64_t memreport_sys_ptr;
	inline  uintptr_t CGame = 0x0;

	inline void UpdateCGame() {
		CGame = static_cast<uintptr_t>(memreport_sys_ptr) + CGame_offset;
	}

	extern  uint64_t m_player_ptr;
#endif NEW_PATCH == 1
#if NEW_PATCH == 0
	constexpr uintptr_t CGame = 0x5BD0270 + CGame_offset;
	constexpr  uint64_t m_player_ptr = 0x5B923B8;
#endif NEW_PATCH == 0
	constexpr uintptr_t CHud = 0x0;

	constexpr uintptr_t YawOffset = 0x0;


	namespace m_player
	{
		constexpr uint64_t entity_list = 0x40;
		constexpr uint64_t local_player = 0x58;
	}
	namespace _CHud
	{
		constexpr uintptr_t can_select_unit = 0x20; // bool
		constexpr uintptr_t arcade_crosshair = 0x2A8; //bool
		constexpr uintptr_t aircraft_distance = 0x2AA; //bool
		constexpr uintptr_t ground_to_air_prediction = 0x2B0; //bool TO FALSE ???
		constexpr uintptr_t air_to_air_prediction = 0x29D;
		constexpr uintptr_t tank_esp = 0x1DC;  // int_8_t to 2 or bitfield 7 bit to 1 other - 0
	}
	namespace _CGame
	{
		constexpr uintptr_t unit_list3 = 0x328;
		constexpr uintptr_t unit_count3 = 0x338;
		constexpr uintptr_t prediction_engine_ptr = 0x418;
		constexpr uintptr_t local_camera = 0x5D8;
		constexpr uintptr_t ballistics_ptr = 0x418;
	}
	namespace _Ballistics
	{
		constexpr uintptr_t selected_unit_ptr = 0x668;
		constexpr uintptr_t	weapon_position_two = 0x1ED8;
		constexpr uintptr_t	velocity = 0x1EE8;
		constexpr uintptr_t	mass = 0x1EF4;
		constexpr uintptr_t	caliber = 0x1EF8;
		constexpr uintptr_t	length = 0x1EFC;
		constexpr uintptr_t	max_dist = 0x1F00;
		constexpr uintptr_t	bomb_impact_point = 0x1AE4;
		constexpr uintptr_t	bullet_impact_point = 0x22C0;
		constexpr uintptr_t	ingame_ballistics = 0x2318;
		constexpr uintptr_t	weapon_position = 0x1EDC;
	}
	namespace _CPredictionEngine
	{
		constexpr uintptr_t selected_unit = 0x630;
		constexpr uintptr_t selected_unit_prediction = 0x20B8;
	}
	namespace _CPlayer
	{
		constexpr uintptr_t team = 0x210;
		constexpr uintptr_t player_unit = 0x7E8; //updated
		constexpr uintptr_t gui_state = 0x5F0; //updated
		constexpr uintptr_t name = 0xB8;
	}
	namespace _CUnitInfo
	{
		constexpr uintptr_t tank_info = 0x20;
		constexpr uintptr_t tank_info2 = 0x20;
		constexpr uintptr_t bombsight = 0x3F1;
		constexpr uintptr_t unit_type = 0x38;
	}
	namespace _CVehiceInfo
	{
		constexpr uintptr_t vehicle_name = 0x0;
	}
	namespace _CCamera
	{
		constexpr uintptr_t camera_position = 0x58;
		constexpr uintptr_t camera_viewmatrix = 0x1C0;
	}

	namespace _CUnit
	{
		constexpr uintptr_t invulnerable_offset = 0xc48;
		constexpr uintptr_t position = 0xAf0;//updated
		constexpr uintptr_t boundsmin = 0x220;//updated
		constexpr uintptr_t boundsmax = 0x22c;//updated
		constexpr uintptr_t rotation_matrix = 0xB94; // Matrix3x3
		constexpr uintptr_t unit_info = 0xDC8; //updated
		constexpr uintptr_t reload_progress = 0x3C0; // int8_t 
		constexpr uintptr_t unit_state = 0xD28; //int16_t 
		constexpr uintptr_t player_ptr = 0xD40; //entity_ptr  updated
		constexpr uintptr_t damagemodelinfo1 = 0x58;
		constexpr uintptr_t damagemodelinfo2 = 0xA0;
		constexpr uintptr_t damagemodelinfo3 = 0xF50;
		constexpr uintptr_t team_num = 0xDB8; // uint8_t updated
	}
}