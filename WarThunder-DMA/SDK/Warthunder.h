#pragma once

#include <atomic>
#include <chrono>
#include <mutex>
#include <thread>
#include <vector>

#include "CGame/CGame.h"
#include "CPlayer/CPlayer.h"
#include "CUnit/CUnit.h"

class Warthunder
{
public:
	Warthunder();
	~Warthunder();

	void Stop();
	std::vector<CUnit> GetLatestUnits();

	CGame* c_game = nullptr;
	CPlayer* local_player = nullptr;
	CUnit* local_unit = nullptr;

	std::atomic<bool> running{ true };
	std::atomic<size_t> last_unit_count{ 0 };
	std::atomic<std::chrono::steady_clock::time_point> last_update_time;

private:
	void CollectionLoop();

	std::thread collection_thread;
	std::mutex units_mutex;
	std::vector<CUnit> latest_units;
	std::vector<CUnit> temp_units;
	std::chrono::milliseconds collection_interval{ 180 };
};