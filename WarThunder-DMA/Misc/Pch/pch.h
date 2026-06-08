//pch.h
#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define NOMINMAX
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <algorithm> 
#include <string>
#include <vector>
#include <map>
#include <set>
#include <list>
#include <fstream>
#include <ppltasks.h>
//#include <windowsx.h>
//#include <WinSock2.h>
#include <ws2tcpip.h>
#include <TlHelp32.h>
#include <thread>
#include <filesystem>
#include <cctype>    
#include <iomanip>
#include <random>
#include <sstream>
#include <locale>
#include <cstdint>
//#include <d2d1_1.h>
#include <dwrite.h>
#include <dwmapi.h>
#include <unordered_set>
//#include <ShellScalingApi.h>

#ifdef DrawText
#undef DrawText
#endif
#ifdef GetCurrentTime
#undef GetCurrentTime
#endif
#ifdef GetObject
#undef GetObject
#endif
#ifdef SendMessage
#undef SendMessage
#endif
#ifdef GetUserName
#undef GetUserName
#endif
#ifdef CreateFont
#undef CreateFont
#endif
#ifdef Button
#undef Button
#endif
#ifdef max
#undef max
#endif
#ifdef min
#undef min
#endif

// Silence noisy warnings from 3rd-party headers (freetype, vmmdll, nlohmann/json, stb, leechcore etc.)
#pragma warning(disable: 4200)   // nonstandard extension: zero-sized array in struct/union
#pragma warning(disable: 4244)   // conversion from 'size_t' to 'DWORD', possible loss of data (and similar truncations)
#pragma warning(disable: 4267)   // same as above for 64->32
#pragma warning(disable: 4305)   // truncation from 'double' to 'float'
#pragma warning(disable: 4100)   // unreferenced formal parameter (common in headers)
#pragma warning(disable: 4189)   // local variable is initialized but not referenced
#pragma warning(disable: 4101)   // 'e' : unreferenced local variable (common in catch blocks)
#pragma warning(disable: 4018)   // '<' : signed/unsigned mismatch
#pragma warning(disable: 4477)   // 'printf' : format string '%p' requires 'void*', but got uintptr_t etc. (we cast where critical)

#include <vmmdll.h>
#include "RunLogger.h"
#define DEBUG_INFO
#ifdef DEBUG_INFO
#define LOG(fmt, ...) RunLogger::LogPrintf(fmt, ##__VA_ARGS__)
#define LOGW(fmt, ...) std::wprintf(fmt, ##__VA_ARGS__)
#else
#define LOG(...)
#define LOGW(...)
#endif
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "vmm.lib")
#pragma comment(lib, "leechcore.lib")
#pragma comment(lib, "FTD3XX.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "Dwrite")
#pragma comment(lib, "windowscodecs.lib")
#pragma comment(lib, "Dwmapi.lib")
#pragma comment(lib, "Shcore.lib")

#include "Vector.h"
#include "Memory.h"