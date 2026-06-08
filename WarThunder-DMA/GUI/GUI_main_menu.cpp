// GUI_main_menu.cpp
#include "Pch.h"
#include "GUI_main_menu.h"  
#include <d3d11.h>
#define IMGUI_ENABLE_FREETYPE
#include "imgui.h"
#include "imgui_freetype.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <tchar.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <windows.h>

#include "ConfigInstance.h"
#include "offsets.h"
#include "Warthunder.h"
#include "handlers/unit_handler/unit_handler.h"
#include "Esp.h"




int Window::screenWidth = 0;
int Window::screenHeight = 0;
Vector2 Window::window_center;
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

ID3D11Device* g_pd3dDevice = NULL;
ID3D11DeviceContext* g_pd3dDeviceContext = NULL;
IDXGISwapChain* g_pSwapChain = NULL;
//IDXGISwapChain* pSwapChain;
ID3D11RenderTargetView* g_mainRenderTargetView = NULL;


bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);














WindowRunResult Window::CreateAndRunWindow(HINSTANCE hInstance, Warthunder* wt, UnitHandler* unit_handler)
{
	RunLogger::StageBegin("创建覆盖层窗口");
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, _T("Dactwere"), NULL };
	RegisterClassEx(&wc);
	Window::screenWidth = GetSystemMetrics(SM_CXSCREEN);
	Window::screenHeight = GetSystemMetrics(SM_CYSCREEN);
	Window::window_center = Vector2(Window::screenWidth * 0.5f, Window::screenHeight * 0.5f);
	RunLogger::Info("屏幕分辨率: %d x %d", screenWidth, screenHeight);
	HWND hwnd = CreateWindowEx(
		0,
		wc.lpszClassName,
		L"Dactwere",
		WS_POPUP | WS_VISIBLE,
		0, 0, screenWidth, screenHeight,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	SetWindowLong(hwnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);

	if (!hwnd)
	{
		RunLogger::StageFail("CreateWindowEx 失败");
		UnregisterClass(wc.lpszClassName, wc.hInstance);
		return WindowRunResult::InitFailed;
	}
	RunLogger::StageOk("Win32 窗口已创建");

	RunLogger::StageBegin("D3D11 设备初始化");
	if (!CreateDeviceD3D(hwnd)) {
		RunLogger::StageFail("D3D11CreateDeviceAndSwapChain 失败");
		CleanupDeviceD3D();
		UnregisterClass(wc.lpszClassName, wc.hInstance);
		return WindowRunResult::InitFailed;
	}
	RunLogger::StageOk();

	ShowWindow(hwnd, SW_SHOWDEFAULT);
	UpdateWindow(hwnd);

	RunLogger::StageBegin("ImGui 初始化");
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	//font inits - robust loader with fallback (prevents "Could not load font file!" crash)
	{
		auto resolve_font = [](const char* filename) -> std::string {
			// Get directory of the running executable (most reliable)
			char exePath[MAX_PATH] = { 0 };
			GetModuleFileNameA(nullptr, exePath, MAX_PATH);
			std::string exeDir(exePath);
			size_t lastSlash = exeDir.find_last_of("\\/");
			if (lastSlash != std::string::npos)
				exeDir = exeDir.substr(0, lastSlash + 1);

			// Candidate locations (covers running from Build\Debug\, from VS debugger cwd, etc.)
			std::vector<std::string> candidates = {
				exeDir + "fonts\\" + filename,           // next to exe / Build\Debug\fonts\
				exeDir + "..\\fonts\\" + filename,       // if exe is deeper
				exeDir + "..\\..\\fonts\\" + filename,
				std::string("fonts\\") + filename,       // relative to current working dir
				std::string("..\\fonts\\") + filename,
				std::string("..\\..\\fonts\\") + filename,
				filename
			};

			for (const auto& p : candidates) {
				std::ifstream test(p, std::ios::binary);
				if (test.good()) {
					return p;
				}
			}
			return {};
		};

		ImFont* main_font = nullptr;

		std::string mainPath = resolve_font("NotoSans.ttf");
		if (!mainPath.empty()) {
			main_font = io.Fonts->AddFontFromFileTTF(
				mainPath.c_str(),
				18.0f,
				nullptr,
				io.Fonts->GetGlyphRangesCyrillic()
			);
			if (main_font) {
				LOG("[Font] Loaded main font: %s\n", mainPath.c_str());
			}
		}

		if (!main_font) {
			printf("[Font] WARNING: Could not load NotoSans.ttf from any known location. Using ImGui default font.\n");
			main_font = io.Fonts->AddFontDefault();
		}

		// Merge CJK fonts if available (MergeMode)
		std::string jpPath = resolve_font("NotoSansJP.ttf");
		if (!jpPath.empty()) {
			ImFontConfig config_jp;
			config_jp.MergeMode = true;
			ImFont* f = io.Fonts->AddFontFromFileTTF(
				jpPath.c_str(),
				18.0f,
				&config_jp,
				io.Fonts->GetGlyphRangesJapanese()
			);
			if (f) LOG("[Font] Merged JP font: %s\n", jpPath.c_str());
		}

		std::string krPath = resolve_font("NotoSansKR.ttf");
		if (!krPath.empty()) {
			ImFontConfig config_kr;
			config_kr.MergeMode = true;
			ImFont* f = io.Fonts->AddFontFromFileTTF(
				krPath.c_str(),
				18.0f,
				&config_kr,
				io.Fonts->GetGlyphRangesKorean()
			);
			if (f) LOG("[Font] Merged KR font: %s\n", krPath.c_str());
		}

		// Chinese (commented in original)
		// std::string scPath = resolve_font("NotoSansSC.ttf");
		// if (!scPath.empty()) { ... }

		io.Fonts->Build();
		io.FontDefault = main_font;
	}
	(void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);
	RunLogger::StageOk();

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(600, 400));

	bool show_menu = true;
	bool insert_key_was_down = false;
	bool request_exit = false;
	bool main_loop_logged = false;

	while (msg.message != WM_QUIT && !request_exit)

	{
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			continue;
		}
		//frame aka do not touch
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		if (!main_loop_logged)
		{
			RunLogger::StageBegin("ESP 渲染主循环");
			RunLogger::Info("主循环已启动（INS 切换菜单，Exit 按钮退出）");
			main_loop_logged = true;
		}

		// Toggle menu visibility with Insert (local overlay machine keyboard).
		const bool insert_key_down = (GetAsyncKeyState(VK_INSERT) & 0x8000) != 0;
		if (insert_key_down && !insert_key_was_down)
			show_menu = !show_menu;
		insert_key_was_down = insert_key_down;

		if (show_menu)
		{
			ImGui::Begin("Menu", nullptr);

			ImGui::BeginTabBar("MainTabs");
			if (ImGui::BeginTabItem("ESP"))
			{
				ImGui::BeginTabBar("EspTabs");

				if (ImGui::BeginTabItem("Player ESP")) {
					ImGui::TextDisabled("INS - toggle menu");

					ImGui::Checkbox("Enable Player ESP", &ConfigInstance.Player_ESP.Enable);
					ImGui::Checkbox("Enable simple box", &ConfigInstance.Player_ESP.Enable_simple_box);

					ImGui::SameLine();
					ImGui::SetNextItemWidth(200.0f);
					ImGui::SliderFloat("box thickness", &ConfigInstance.Player_ESP.box_thickness, 0.01, 3);
					ImGui::SameLine();
					static float color[3] = { 0.0f, 0.0f, 0.0f };
					if (ImGui::ColorEdit3("Box color", color, ImGuiColorEditFlags_NoInputs))
						ConfigInstance.Player_ESP.box_color = ImColor(color[0], color[1], color[2]);

					ImGui::SetNextItemWidth(200.0f);
					ImGui::SliderFloat("Font size", &ConfigInstance.Player_ESP.font_size, 1, 40);
					ImGui::SameLine();
					static float text_color_buffer[3] = { 0.0f, 0.0f, 0.0f };
					if (ImGui::ColorEdit3("Text color", text_color_buffer, ImGuiColorEditFlags_NoInputs))
						ConfigInstance.Player_ESP.text_color = ImColor(text_color_buffer[0], text_color_buffer[1], text_color_buffer[2]);

					ImGui::Checkbox("Show distanse", &ConfigInstance.Player_ESP.Show_distanse);
					ImGui::SameLine();
					ImGui::SetNextItemWidth(200.0f);
					ImGui::SliderInt("Max distanse", &ConfigInstance.Player_ESP.Max_distance, 0, 1200);
					ImGui::Checkbox("Show unit name", &ConfigInstance.Player_ESP.Show_unit_name);
					ImGui::Checkbox("Show invulnerable state", &ConfigInstance.Player_ESP.show_invul_state);

					ImGui::Separator();
					if (ImGui::Button("Exit", ImVec2(120.0f, 0.0f)))
					{
						RunLogger::Info("用户点击 Exit 按钮，准备退出程序");
						request_exit = true;
					}

					ImGui::EndTabItem();
				}

				ImGui::EndTabBar();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("config"))
			{
				static char save_configNameBuffer[60] = "config";
				ImGui::InputTextWithHint("Config Name for save", "Enter config name", save_configNameBuffer, IM_ARRAYSIZE(save_configNameBuffer), ImGuiInputTextFlags_CharsNoBlank);
				if (ImGui::Button("Save config")) {
					ConfigInstance.SaveToFile(save_configNameBuffer);
				}
				static char load_configNameBuffer[60] = "config";
				ImGui::InputTextWithHint("Config Name for load", "Enter config name", load_configNameBuffer, IM_ARRAYSIZE(load_configNameBuffer));
				if (ImGui::Button("Load config")) {
					ConfigInstance.LoadFromFile(load_configNameBuffer);
				}

				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("DEBUG")) {
				ImGui::Text("DMA Initialized: %s", Memory::DMA_INITIALIZED ? "YES" : "NO");
				ImGui::Separator();
				ImGui::Text("Offsets resolved: %s", offsets::resolved ? "YES" : "NO");
				ImGui::Text("CGame RVA: 0x%llX  (%s)", (unsigned long long)offsets::CGame,
					offsets::cgame_resolve_method ? offsets::cgame_resolve_method : "n/a");
				ImGui::Text("m_player RVA: 0x%llX  (%s)", (unsigned long long)offsets::m_player_ptr,
					offsets::mplayer_resolve_method ? offsets::mplayer_resolve_method : "n/a");
				ImGui::Separator();
				if (wt) {
					ImGui::Text("Collection running: %s", wt->running ? "true" : "false");
					ImGui::Text("Last unit count (producer): %zu", wt->last_unit_count.load());
				}
				if (unit_handler) {
					ImGui::Text("UnitHandler running: %s", unit_handler->running ? "true" : "false");
					ImGui::Text("Complete units (consumer): %zu", unit_handler->complete_units.size());
				}
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();

			ImGui::End();
		}




		ImDrawList* draw_list = ImGui::GetBackgroundDrawList();
		//draw_list->AddRect(ImVec2(100, 100), ImVec2(200, 200), IM_COL32(255, 0, 0, 255), 2.0f, ImDrawFlags_None);

		if (wt && wt->c_game && wt->local_unit)
		{
			if (!wt->c_game->camera.is_valid())
			{
				 wt->c_game->set_local_camera();
			}
			wt->c_game->camera.set_view_matrix();
			Esp::DrawPlayerEsp(draw_list, wt->c_game->camera, unit_handler, *wt->local_unit,
				Window::screenWidth, Window::screenHeight);
		}

		// FPS + menu hint
		char fps_buffer[32];
		snprintf(fps_buffer, sizeof(fps_buffer), "FPS: %d", static_cast<int>(round(ImGui::GetIO().Framerate)));
		draw_list->AddText(ImGui::GetFont(), 15.0f, ImVec2(0, 0), IM_COL32(255, 255, 255, 255), fps_buffer);
		if (!show_menu)
			draw_list->AddText(ImGui::GetFont(), 15.0f, ImVec2(0, 18), IM_COL32(200, 200, 200, 255), "INS - Menu");


		// render aka do not touch
		ImGui::Render();
		const float clear_color[4] = { 0.0f, 0.0f, 0.0f, 1.00f };
		g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
		g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		g_pSwapChain->Present(1, 0);
	}

	if (request_exit)
		RunLogger::StageOk("用户主动退出");
	else if (msg.message == WM_QUIT)
		RunLogger::Info("收到 WM_QUIT，主循环结束");

	RunLogger::StageBegin("释放覆盖层资源");
	if (unit_handler) unit_handler->Stop();
	if (wt) wt->Stop();

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	CleanupDeviceD3D();
	DestroyWindow(hwnd);
	UnregisterClass(wc.lpszClassName, wc.hInstance);
	RunLogger::StageOk();

	return request_exit ? WindowRunResult::UserExit : WindowRunResult::Success;
}
bool CreateDeviceD3D(HWND hWnd) {
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferCount = 1;
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, NULL, 0, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, NULL, &g_pd3dDeviceContext) != S_OK)
		return false;

	CreateRenderTarget();
	return true;
}

void CleanupDeviceD3D() {
	CleanupRenderTarget();
	if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = NULL; }
	if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = NULL; }
	if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
}

void CreateRenderTarget() {
	ID3D11Texture2D* pBackBuffer;
	g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
	g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_mainRenderTargetView);
	pBackBuffer->Release();
}

void CleanupRenderTarget() {
	if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = NULL; }
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam)) {
		return true;
	}

	switch (msg) {
		/*case WM_SIZE:
			if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED) {
				CleanupRenderTarget();
				g_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
				CreateRenderTarget();
				ImGui_ImplDX11_InvalidateDeviceObjects();
				ImGui_ImplDX11_CreateDeviceObjects();

				ImGuiIO& io = ImGui::GetIO();
				io.DisplaySize = ImVec2((float)LOWORD(lParam), (float)HIWORD(lParam));
			}
			return 0;*/
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}