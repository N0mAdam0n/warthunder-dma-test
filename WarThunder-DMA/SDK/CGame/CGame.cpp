#include "pch.h"
#include "CGame.h"
#include "../offsets.h"
#include "../globals/globals.h"
auto CGame::set_instance() -> const bool
{
	this->base_address = mem.Read<std::uintptr_t>( globals::game_base + offsets::CGame);
	return this->base_address != 0;
}

//auto CGame::get_unit_list() -> CUnitList
//{
//	return CUnitList(mem.Read<std::uintptr_t>(this->base_address + offsets::cgame::unitlist1));
//}
//auto CGame::get_unit_count() -> const std::uint32_t
//{
//	return mem.Read<std::uint32_t>(this->base_address + offsets::cgame::unitcount1);
//}
//auto CGame::get_unit_list2() -> CUnitList
//{
//	return CUnitList(mem.Read<std::uintptr_t>(this->base_address + offsets::cgame::unitlist2));
//}
//auto CGame::get_unit_count2() -> const std::uint32_t
//{
//	return mem.Read<std::uint32_t>(this->base_address + offsets::cgame::unitcount2);
//}

auto CGame::get_unit_list3() -> CUnitList
{
	return CUnitList(mem.Read<std::uintptr_t>(this->base_address + offsets::_CGame::unit_list3));
}
auto CGame::get_unit_count3() -> const std::uint32_t
{
	return mem.Read<std::uint32_t>(this->base_address + offsets::_CGame::unit_count3);
}

void CGame::set_local_camera() 
{
	camera =  CCamera(mem.Read<std::uintptr_t>(this->base_address + offsets::_CGame::local_camera));
}

auto CGame::prediciton_engine() -> CPredictionEngine
{
	return CPredictionEngine(mem.Read<std::uintptr_t>(this->base_address + offsets::_CGame::prediction_engine_ptr));
}




