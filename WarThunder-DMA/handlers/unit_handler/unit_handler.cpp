#include "pch.h"
#include "unit_handler.h"
#include "ConfigInstance.h"

UnitHandler::UnitHandler(Warthunder* wt)
{


	std::thread fast_loop([wt, this]() {
		while (1) {
			if (ConfigInstance.Player_ESP.Enable)
			{
				/*wt.unit_list_mutex.lock();*/
				if (wt->unit_list_ready)
				{
					temp_units = std::move(wt->unit_list);
					wt->unit_list_ready = false;
				}
				/*wt.unit_list_mutex.unlock();*/
				if (!temp_units.empty())
				{
					auto handle = mem.CreateScatterHandle();
					for (auto& unit : temp_units)
					{
						unit.read_position_scatter_request(handle);
						if (ConfigInstance.Player_ESP.Enable_simple_box)
						{
							unit.read_boundsmin_scatter_request(handle);
							unit.read_boundsmax_scatter_request(handle);
						}

					}

					mem.ExecuteReadScatter(handle);

					mem.CloseScatterHandle(handle);


				}


				complete_units_mutex.lock();
				complete_units = temp_units;
				complete_units_mutex.unlock();
			}
		}

		});

	fast_loop.detach();
}
