#include "pch.h"
#include "Memory.h"
#include "GUI_main_menu.h"
#include "ConfigInstance.h"
#include "Warthunder.h"
#include "handlers/unit_handler/unit_handler.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	AllocConsole();
	FILE* pCout = nullptr;
	freopen_s(&pCout, "CONOUT$", "w", stdout);

	if (!RunLogger::Init())
	{
		printf("[ERROR] 无法创建运行日志文件，程序继续运行但仅输出到控制台。\n");
	}

	RunLogger::StageBegin("程序启动");
	RunLogger::Info("WarThunder-DMA 启动");
	RunLogger::StageOk();

	RunLogger::StageBegin("检查 DMA 依赖库");
	if (!Memory::LibrariesLoaded())
	{
		RunLogger::StageFail("vmm.dll / leechcore.dll / FTD3XX.dll 加载失败");
		RunLogger::Warn("请确认 DLL 与可执行文件位于同一目录或已加入 PATH");
	}
	else
	{
		RunLogger::StageOk("DMA 依赖库已加载（全局 Memory 构造阶段完成）");
	}

	RunLogger::StageBegin("DMA 初始化");
	const bool dmaReady = mem.Init("aces.exe", /*memMap=*/false);
	if (!dmaReady)
	{
		RunLogger::StageFail("DMA 或目标进程 aces.exe 初始化失败");
		RunLogger::Warn("可能原因：DMA 硬件未连接、设备被占用、或游戏未运行");
	}
	else
	{
		RunLogger::StageOk("DMA 与 aces.exe 进程已就绪");
	}

	RunLogger::StageBegin("加载默认配置");
	ConfigInstance.LoadFromFile("default_config");
	RunLogger::StageOk("default_config");

	RunLogger::StageBegin("键盘热键初始化");
	if (!mem.GetKeyboard()->InitKeyboard())
	{
		RunLogger::StageFail("内核键盘状态读取初始化失败");
		RunLogger::Warn("INS 菜单切换仍可用（本机键盘），DMA 热键功能不可用");
	}
	else
	{
		RunLogger::StageOk();
	}

	Warthunder* wt = nullptr;
	UnitHandler* unit_handler = nullptr;

	RunLogger::StageBegin("Warthunder 核心初始化");
	wt = new Warthunder();
	if (!wt->c_game || !wt->local_player || !wt->local_unit)
	{
		RunLogger::StageFail("CGame / 本地玩家 / 本地单位 初始化不完整");
		RunLogger::Shutdown(RunExitStatus::Error, "游戏 SDK 初始化失败，无法继续");
		delete wt;
		return 1;
	}
	RunLogger::StageOk();

	RunLogger::StageBegin("UnitHandler 初始化");
	unit_handler = new UnitHandler(wt);
	RunLogger::StageOk("单位消费线程已启动");

	const auto window_result = Window::CreateAndRunWindow(hInstance, wt, unit_handler);
	if (window_result == WindowRunResult::InitFailed)
	{
		RunLogger::StageFail("覆盖层初始化失败");
		delete unit_handler;
		delete wt;
		RunLogger::Shutdown(RunExitStatus::Error, "覆盖层窗口或 D3D/ImGui 初始化失败");
		return 1;
	}

	delete unit_handler;
	delete wt;

	if (window_result == WindowRunResult::UserExit)
		RunLogger::Shutdown(RunExitStatus::UserExit, "用户通过 Exit 按钮退出");
	else
		RunLogger::Shutdown(RunExitStatus::Success, "所有阶段正常完成");
	return 0;
}