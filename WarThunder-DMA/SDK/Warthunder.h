#pragma once
#include <thread>

#include "CHud/CHud.h"
#include "CGame/CGame.h"
#include "CUnitList/CUnitList.h"
#include "CPlayer/CPlayer.h"
#include "CUnit/CUnit.h"
#include "CUnitInfo/CUnitInfo.h"
#include "CVehicleInfo/CVehicleInfo.h"
#include <iostream>
#include <vector>
#include <mutex>
#include "globals/globals.h"

#include "ConfigInstance.h"

inline bool IsValidAddress(uint64_t addr) {
	return addr >= globals::game_base && addr <= globals::max_valid_addr;
}


class Warthunder
{
public:
	Warthunder()
	{
		try {

			globals();


#if NEW_PATCH == 1
			{
				uint64_t string_addr = globals::FindString("mem.memreport_sys");
				offsets::memreport_sys_ptr = globals::FindPointer(string_addr) - globals::game_base ;
				if (!offsets::memreport_sys_ptr)
				{
					throw "Failed to get memreport_sys_ptr";
				}
				std::cout << "RVA:memreport_sys_ptr = " << std::hex <<  offsets::memreport_sys_ptr  << std::endl;
				offsets::UpdateCGame();

				string_addr = globals::FindString("m_player");
				offsets::m_player_ptr = globals::FindPointer(string_addr) - globals::game_base ;
				if (!offsets::memreport_sys_ptr)
				{
					throw "Failed to get m_player_ptr";
				}
				std::cout << "RVA:m_player_ptr = " << std::hex <<  offsets::m_player_ptr << std::endl;
			}
#endif NEW_PATCH == 1
			c_game = new CGame;
			if (!c_game->set_instance())
				throw "Failed to get CGame instance";

			local_player = new CPlayer(CPlayer::get_local());
			if (!local_player->is_valid())
				throw "Failed to get local_player instance";

			local_unit = new CUnit(local_player->unit());
			if (!local_unit->is_valid())
				throw "Failed to get local_unit instance";

			//c_hud = new CHud;
			//if (!c_hud->get_instance())
			//	throw "Failed to get CHud instance";
			
			//local_unit_info = new CUnitInfo(local_unit->unit_info());
			//if (!local_unit_info->is_valid())
			//	throw "Failed to get local_unit_info instance";

			//prediction_engine = new CPredictionEngine(c_game->prediciton_engine());
			//if (!prediction_engine->is_valid())
			//	throw "Failed to get prediction_engine instance";
			


			std::thread loopne([&]() { // kill me please cringe coding
				c_game->set_instance();
				while (1) {

					auto handle = mem.CreateScatterHandle();

					/*c_hud->get_instance();*/
					*local_player = CPlayer::get_local();
					local_player->read_gui_state();
					*local_unit = local_player->unit();
					local_unit->read_position_scatter_request(handle);
					local_unit->read_team_num_scatter_request(handle);
					/**local_unit_info = local_unit->unit_info();*/
					/**prediction_engine = c_game->prediciton_engine();*/



					mem.ExecuteReadScatter(handle);
					if (local_player->gui_state == GuiState::ALIVE || local_player->gui_state == GuiState::SPEC || local_player->gui_state == GuiState::MENU) {

						auto unit_list = c_game->get_unit_list3();
						const auto unit_count = c_game->get_unit_count3();
						
						std::vector<CUnit> units = unit_list.unit_scatter(handle, unit_count);

						if (unit_list.is_valid()) {

							for (CUnit unit : units) {
		

								unit.read_unit_state_scatter_request(handle);
								unit.read_player();
								unit.player.read_gui_state();
								unit.read_team_num_scatter_request(handle);
								if (ConfigInstance.Player_ESP.show_invul_state)
								{
									unit.read_invul_state_scatter_request(handle);
								}
								


								mem.ExecuteReadScatter(handle);

								if (!unit.is_valid()|| !unit.is_alive() || unit.team_num == local_unit->team_num)
									continue;
								unit.read_unit_info();
								unit.unit_info.set_vehicle_info();
								unit.unit_info.vehicle_info.set_vehicle_name();


							


							





								temp_units.push_back(unit);
							}
						}
					}

					/*unit_list_mutex.lock();*/
					
					unit_list = temp_units;
					unit_list_ready = true;
					/*unit_list_mutex.unlock();*/
					temp_units.clear();

					std::this_thread::sleep_for(std::chrono::milliseconds(350));
				}
			});

			loopne.detach();





		}
		catch (const char* message) {
			std::cout << message << std::endl;
		}
		catch (...) {
			std::cout << "[!] Some error in Warthunder class" << std::endl;
		}
		
		
	}
	~Warthunder()
	{
		delete c_hud;
		delete c_game;
		delete local_player;
		delete local_unit;
		delete local_unit_info;
		delete prediction_engine;
	}

	auto rangle() -> const RAngle;
	auto rangle(const RAngle angle) -> void;

	CHud* c_hud;
	CGame* c_game;

	CPlayer* local_player;
	CUnit* local_unit;
	CUnitInfo* local_unit_info;

	CPredictionEngine* prediction_engine;
	
	//std::mutex unit_list_mutex; // bebebe
	bool unit_list_ready = false;
	std::vector<CUnit> unit_list;
	
private:
	std::vector<CUnit> temp_units;
}; inline Warthunder* warthunder;

