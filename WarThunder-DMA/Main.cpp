#include "pch.h"
#include "memory.h"
#include <dwmapi.h>

//gui
#include "GUI_main_menu.h"

//ESP
#include <Esp.h>

#include "Kmboxnet.h"

#include "ConfigInstance.h"



//aim
#include "Aim.h"


//SDK
#include <Warthunder.h>
#include "handlers/unit_handler/unit_handler.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	//console init for debug
	AllocConsole();
	FILE* pCout;
	freopen_s(&pCout, "CONOUT$", "w", stdout);

	//printf("1");



	if (!mem.Init("aces.exe"))
	{
		printf("Failed to initialize process\n");
		system("pause");
	}

	//sdk init
	Warthunder* wt = new Warthunder;
	UnitHandler* unit_handler = new UnitHandler(wt);
	Esp esp;
	//Aim aim;

	
	
	ConfigInstance.LoadFromFile("default_config");

	if (!mem.GetKeyboard()->InitKeyboard())
	{
		std::cout << "Failed to initialize keyboard hotkeys through kernel." << std::endl;

	}
	/*if (ConfigInstance.Settings.kmbox_ip != "" && ConfigInstance.Settings.kmbox_port != "" && ConfigInstance.Settings.kmbox_uuid != "")
	{
		int result = kmNet_init(ConfigInstance.Settings.kmbox_ip.data(), ConfigInstance.Settings.kmbox_port.data(), ConfigInstance.Settings.kmbox_uuid.data());
		if (result == success) {
			printf("Initialization successful!\n");
		}
		else {
			printf("Initialization failed with error code: %d\n", result);
			switch (result) {
			case err_creat_socket:
				printf("Error: Failed to create socket.\n");
				break;
			case err_net_version:
				printf("Error: Socket version error.\n");
				break;
			case err_net_tx:
				printf("Error: Socket sending error.\n");
				break;
			case err_net_rx_timeout:
				printf("Error: Socket receive timeout.\n");
				break;
			case err_net_cmd:
				printf("Error: Command error.\n");
				break;
			case err_net_pts:
				printf("Error: Timestamp error.\n");
				break;
			case usb_dev_tx_timeout:
				printf("Error: USB device sending failure.\n");
				break;
			default:
				printf("Error: Unknown error code.\n");
				break;
			}

		}
	}*/


	//std::thread AimPlayerUpdateThread(Aim::UpdateThread, &aim);
	//AimPlayerUpdateThread.detach();


	if (!Window::CreateAndRunWindow(hInstance,wt,unit_handler)) {
		return 1;
	}
	return 0;
}
void main()
{
	//not work in window mode

}
