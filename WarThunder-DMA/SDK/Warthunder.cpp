#include "pch.h"
#include "Warthunder.h"
#include "../Memory/Memory.h"
#include "globals/globals.h"
#include "offsets.h"
#include "ConfigInstance.h"

Warthunder::Warthunder()
{
	try
	{
		RunLogger::StageBegin("读取游戏模块基址");
		globals();
		RunLogger::Info("game_base=0x%llX, game_size=0x%llX",
			(unsigned long long)globals::game_base,
			(unsigned long long)globals::game_size);
		if (!globals::game_base || !globals::game_size)
			throw "游戏模块基址或大小无效";
		RunLogger::StageOk();

		RunLogger::StageBegin("偏移量解析");
		if (!offsets::Resolve())
			throw "偏移量自动解析失败（字符串/特征码扫描）";
		RunLogger::StageOk();

		RunLogger::StageBegin("CGame 实例");
		c_game = new CGame();
		if (!c_game->set_instance())
			throw "无法获取 CGame 实例";
		RunLogger::StageOk();

		RunLogger::StageBegin("本地玩家");
		local_player = new CPlayer(CPlayer::get_local());
		if (!local_player->is_valid())
			throw "无法获取 local_player 实例";
		RunLogger::StageOk();

		RunLogger::StageBegin("本地单位");
		local_unit = new CUnit(local_player->unit());
		if (!local_unit->is_valid())
			throw "无法获取 local_unit 实例";
		RunLogger::StageOk();

		RunLogger::StageBegin("单位采集线程");
		collection_thread = std::thread(&Warthunder::CollectionLoop, this);
		RunLogger::StageOk("CollectionLoop 已启动");
	}
	catch (const char* message)
	{
		RunLogger::StageFail(message);
	}
	catch (...)
	{
		RunLogger::StageFail("Warthunder 构造函数发生未知异常");
	}
}

Warthunder::~Warthunder()
{
	RunLogger::Info("正在停止 Warthunder...");
	Stop();
	delete local_unit;
	delete local_player;
	delete c_game;
	RunLogger::Info("Warthunder 资源已释放");
}

void Warthunder::Stop()
{
	running = false;
	if (collection_thread.joinable())
		collection_thread.join();
}

std::vector<CUnit> Warthunder::GetLatestUnits()
{
	std::lock_guard<std::mutex> lk(units_mutex);
	return latest_units;
}

void Warthunder::CollectionLoop()
{
	RunLogger::Info("CollectionLoop 线程已进入主循环");
	while (running)
	{
		c_game->set_instance();

		auto handle = mem.CreateScatterHandle();

		*local_player = CPlayer::get_local();
		local_player->read_gui_state();
		*local_unit = local_player->unit();
		local_unit->read_position_scatter_request(handle);
		local_unit->read_team_num_scatter_request(handle);
		mem.ExecuteReadScatter(handle);

		auto unit_list = c_game->get_unit_list3();
		const auto unit_count = c_game->get_unit_count3();

		if (unit_list.is_valid() && unit_count > 0)
		{
			std::vector<CUnit> units = unit_list.unit_scatter(handle, unit_count);

			for (CUnit unit : units)
			{
				unit.read_unit_state_scatter_request(handle);
				unit.read_player();
				unit.player.read_gui_state();
				unit.read_team_num_scatter_request(handle);

				if (ConfigInstance.Player_ESP.show_invul_state)
					unit.read_invul_state_scatter_request(handle);

				mem.ExecuteReadScatter(handle);

				if (!unit.is_valid() || !unit.is_alive() || unit.team_num == local_unit->team_num)
					continue;

				unit.read_unit_info();
				unit.unit_info.set_vehicle_info();
				unit.unit_info.vehicle_info.set_vehicle_name();
				temp_units.push_back(unit);
			}

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

		mem.CloseScatterHandle(handle);

		{
			std::lock_guard<std::mutex> lk(units_mutex);
			latest_units = std::move(temp_units);
			last_unit_count = latest_units.size();
			last_update_time = std::chrono::steady_clock::now();
		}
		temp_units.clear();

		std::this_thread::sleep_for(collection_interval);
	}
	RunLogger::Info("CollectionLoop 线程已退出");
}