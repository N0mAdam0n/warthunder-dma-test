#pragma once
#include <thread>
#include <atomic>
#include <chrono>

// Threading refactor applied:
// - Single producer thread with mutex+cv snapshot (latest_units)
// - Position/bounds scatter merged into producer
// - Consumer (UnitHandler) is now lightweight + sleeps when disabled
// - Proper Stop()/join for clean shutdown
// See plan.md section 10 for details.

#include "CHud/CHud.h"
#include "CGame/CGame.h"
#include "CUnitList/CUnitList.h"
// Memory::DMA_INITIALIZED is available via pch.h (included by the .cpp that uses this header).
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
		if (!Memory::DMA_INITIALIZED) {
			std::cout << "[Warthunder] DMA not ready — limited UI-test mode (no memory reads).\n";
			// Start a dummy idle thread so GetLatestUnits() / consumers stay happy and running flag works.
			collection_thread = std::thread([this]() {
				while (running) {
					std::this_thread::sleep_for(std::chrono::milliseconds(300));
				}
			});
			return; // skip all memory-dependent init below
		}

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
			


			collection_thread = std::thread([this]() { // refactored threading (single producer with snapshot + cv)
				c_game->set_instance();
				while (running) {
					if (!Memory::DMA_INITIALIZED) {
						// No DMA hardware connected — idle cleanly so the GUI can still run for testing.
						std::this_thread::sleep_for(std::chrono::milliseconds(300));
						continue;
					}

					auto handle = mem.CreateScatterHandle();

					*local_player = CPlayer::get_local();
					local_player->read_gui_state();
					*local_unit = local_player->unit();
					local_unit->read_position_scatter_request(handle);
					local_unit->read_team_num_scatter_request(handle);

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

								if (!unit.is_valid() || !unit.is_alive() || unit.team_num == local_unit->team_num)
									continue;
								unit.read_unit_info();
								unit.unit_info.set_vehicle_info();
								unit.unit_info.vehicle_info.set_vehicle_name();


							


							





								temp_units.push_back(unit);
							}
						}

						// Merged position/bounds scatter here (producer enriches before publish)
						if (!temp_units.empty())
						{
							auto pos_handle = mem.CreateScatterHandle();
							for (auto& unit : temp_units)
							{
								unit.read_position_scatter_request(pos_handle);
								if (ConfigInstance.Player_ESP.Enable_simple_box)
								{
									unit.read_boundsmin_scatter_request(pos_handle);
									unit.read_boundsmax_scatter_request(pos_handle);
								}
							}
							mem.ExecuteReadScatter(pos_handle);
							mem.CloseScatterHandle(pos_handle);
						}
					}

					// Publish snapshot (protected by mutex + cv)
					{
						std::lock_guard<std::mutex> lk(units_mutex);
						latest_units = std::move(temp_units);
						last_unit_count = latest_units.size();
						last_update_time = std::chrono::steady_clock::now();
					}
					units_cv.notify_one();

					temp_units.clear();

					std::this_thread::sleep_for(collection_interval);
				}
			});

			// Thread is now a proper member with clean shutdown via Stop()/join.





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
		Stop();  // ensure collection thread is joined (Step 2+)
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

	// Threading / lifetime control + clean snapshot (refactored)
	std::atomic<bool> running{true};
	std::thread collection_thread;

	void Stop();

	// Proper snapshot handoff (the only public way for consumers now)
	std::mutex units_mutex;
	std::condition_variable units_cv;
	std::vector<CUnit> latest_units;  // protected snapshot for consumers

	std::vector<CUnit> GetLatestUnits();

	// For DEBUG / verification
	std::atomic<size_t> last_unit_count{0};
	std::atomic<std::chrono::steady_clock::time_point> last_update_time;

	// Tunable collection interval (ms). Can be driven from Config later.
	std::chrono::milliseconds collection_interval{180};

	// Helper to update interval at runtime (e.g. from config)
	void SetCollectionInterval(int ms) {
		if (ms > 0) collection_interval = std::chrono::milliseconds(ms);
	}

private:
	std::vector<CUnit> temp_units;
}; inline Warthunder* warthunder;

inline void Warthunder::Stop()
{
	running = false;
	units_cv.notify_one();  // wake consumer if waiting
	if (collection_thread.joinable())
	{
		collection_thread.join();
	}
}

inline std::vector<CUnit> Warthunder::GetLatestUnits()
{
	std::lock_guard<std::mutex> lk(units_mutex);
	return latest_units;  // copy for first version (safe, simple)
}

