#pragma once

#include "../SDK/CUnit/CUnit.h"
#include <Warthunder.h>
#include <atomic>
#include <thread>

class UnitHandler
{
public:
	UnitHandler(Warthunder *wt);
	std::vector<CUnit> complete_units;
	std::mutex complete_units_mutex;

	// Lightweight consumer thread that copies the latest snapshot from Warthunder
	// (positions are now enriched by the producer).
	std::atomic<bool> running{true};
	std::thread fast_thread;

	void Stop();

	~UnitHandler()
	{
		Stop();
	}

private:
	// temp_units removed after refactor (direct move to complete_units)
};

