#pragma once

#include <atomic>
#include <mutex>
#include <thread>
#include <vector>

#include "Warthunder.h"
#include "CUnit/CUnit.h"

class UnitHandler
{
public:
	explicit UnitHandler(Warthunder* wt);
	~UnitHandler() { Stop(); }

	void Stop();

	std::vector<CUnit> complete_units;
	std::mutex complete_units_mutex;
	std::atomic<bool> running{ true };

private:
	std::thread fast_thread;
};