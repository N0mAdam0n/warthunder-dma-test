#include "pch.h"
#include "unit_handler.h"
#include "ConfigInstance.h"

UnitHandler::UnitHandler(Warthunder* wt)
{
	fast_thread = std::thread([wt, this]() {
		RunLogger::Info("UnitHandler 消费线程已进入主循环");
		while (running)
		{
			if (ConfigInstance.Player_ESP.Enable)
			{
				auto snapshot = wt->GetLatestUnits();
				std::lock_guard<std::mutex> lock(complete_units_mutex);
				complete_units = std::move(snapshot);
				std::this_thread::sleep_for(std::chrono::milliseconds(50));
			}
			else
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(200));
			}
		}
		RunLogger::Info("UnitHandler 消费线程已退出");
	});
}

void UnitHandler::Stop()
{
	RunLogger::Info("正在停止 UnitHandler...");
	running = false;
	if (fast_thread.joinable())
		fast_thread.join();
	RunLogger::Info("UnitHandler 已停止");
}