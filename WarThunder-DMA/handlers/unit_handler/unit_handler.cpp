#include "pch.h"
#include "unit_handler.h"
#include "ConfigInstance.h"

UnitHandler::UnitHandler(Warthunder* wt)
{
	fast_thread = std::thread([wt, this]() {
		while (running) {
			if (ConfigInstance.Player_ESP.Enable)
			{
				// Snapshot from producer already has positions (merged scatter in Warthunder).
				auto snapshot = wt->GetLatestUnits();

				complete_units_mutex.lock();
				complete_units = std::move(snapshot);
				complete_units_mutex.unlock();

				// Throttle to avoid spinning; producer rate is what matters.
				std::this_thread::sleep_for(std::chrono::milliseconds(50));
			}
			else
			{
				// Sleep aggressively when ESP is off (eliminates the previous 100% CPU spin).
				std::this_thread::sleep_for(std::chrono::milliseconds(200));
			}
		}
	});
}

void UnitHandler::Stop()
{
	running = false;
	if (fast_thread.joinable())
	{
		fast_thread.join();
	}
}
