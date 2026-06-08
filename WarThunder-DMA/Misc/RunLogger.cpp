#include "pch.h"
#include "RunLogger.h"

#include <chrono>
#include <cstdio>
#include <filesystem>
#include <iomanip>
#include <mutex>
#include <sstream>

namespace
{
	std::mutex g_log_mutex;
	std::ofstream g_log_file;
	std::string g_log_path;
	std::string g_current_stage = "未开始";
	bool g_initialized = false;
	bool g_shutdown_called = false;
	RunExitStatus g_exit_status = RunExitStatus::Running;
	std::string g_exit_message;
}

std::string RunLogger::MakeTimestamp()
{
	const auto now = std::chrono::system_clock::now();
	const auto time = std::chrono::system_clock::to_time_t(now);
	const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
		now.time_since_epoch()) % 1000;

	std::tm local_tm{};
	localtime_s(&local_tm, &time);

	std::ostringstream oss;
	oss << std::put_time(&local_tm, "%Y-%m-%d %H:%M:%S")
		<< '.' << std::setfill('0') << std::setw(3) << ms.count();
	return oss.str();
}

void RunLogger::WriteLine(const char* level, const char* text)
{
	const std::string line = "[" + MakeTimestamp() + "][" + level + "] " + text + "\n";

	std::lock_guard<std::mutex> lock(g_log_mutex);
	std::fputs(line.c_str(), stdout);
	std::fflush(stdout);

	if (g_initialized && g_log_file.is_open())
	{
		g_log_file << line;
		g_log_file.flush();
	}
}

void RunLogger::WriteFormatted(const char* level, const char* fmt, va_list args)
{
	char buffer[2048] = {};
	vsnprintf(buffer, sizeof(buffer), fmt, args);
	WriteLine(level, buffer);
}

bool RunLogger::Init()
{
	std::lock_guard<std::mutex> lock(g_log_mutex);
	if (g_initialized)
		return true;

	char exe_path[MAX_PATH] = {};
	if (!GetModuleFileNameA(nullptr, exe_path, MAX_PATH))
		return false;

	std::filesystem::path exe_dir = std::filesystem::path(exe_path).parent_path();
	const std::filesystem::path logs_dir = exe_dir / "logs";
	std::error_code ec;
	std::filesystem::create_directories(logs_dir, ec);

	const auto now = std::chrono::system_clock::now();
	const auto time = std::chrono::system_clock::to_time_t(now);
	std::tm local_tm{};
	localtime_s(&local_tm, &time);

	std::ostringstream filename;
	filename << "run_"
		<< std::put_time(&local_tm, "%Y%m%d_%H%M%S")
		<< ".log";

	g_log_path = (logs_dir / filename.str()).string();
	g_log_file.open(g_log_path, std::ios::out | std::ios::trunc);
	if (!g_log_file.is_open())
		return false;

	g_initialized = true;
	g_shutdown_called = false;
	g_exit_status = RunExitStatus::Running;
	g_exit_message.clear();
	g_current_stage = "日志系统初始化";

	std::atexit(AtExitHandler);

	const std::string header = "========== WarThunder-DMA 运行日志 ==========\n"
		"日志文件: " + g_log_path + "\n";
	g_log_file << "[" << MakeTimestamp() << "][INFO] " << header;
	g_log_file.flush();

	std::fputs(("[RunLogger] 日志文件: " + g_log_path + "\n").c_str(), stdout);
	std::fflush(stdout);
	return true;
}

void RunLogger::Shutdown(RunExitStatus status, const char* message)
{
	std::lock_guard<std::mutex> lock(g_log_mutex);
	if (g_shutdown_called)
		return;

	g_shutdown_called = true;
	g_exit_status = status;
	if (message)
		g_exit_message = message;

	const char* status_text = "未知";
	switch (status)
	{
	case RunExitStatus::Success:         status_text = "正常结束"; break;
	case RunExitStatus::UserExit:        status_text = "用户退出"; break;
	case RunExitStatus::Error:           status_text = "错误退出"; break;
	case RunExitStatus::WarningContinue: status_text = "警告后继续"; break;
	default:                             status_text = "运行中异常终止"; break;
	}

	std::ostringstream footer;
	footer << "========== 程序终止 ==========\n"
		<< "终止状态: " << status_text << "\n"
		<< "最后阶段: " << g_current_stage << "\n";
	if (!g_exit_message.empty())
		footer << "附加信息: " << g_exit_message << "\n";

	const std::string footer_str = footer.str();
	std::fputs(("[RunLogger] " + footer_str).c_str(), stdout);
	std::fflush(stdout);

	if (g_initialized && g_log_file.is_open())
	{
		g_log_file << "[" << MakeTimestamp() << "][终止] " << footer_str;
		g_log_file.flush();
		g_log_file.close();
	}
}

void RunLogger::AtExitHandler()
{
	if (!g_shutdown_called)
		Shutdown(RunExitStatus::Running, "进程退出时未执行正常清理（可能为异常终止）");
}

void RunLogger::StageBegin(const char* stage_name)
{
	if (!stage_name)
		return;

	std::string text = std::string(">>> 正在执行: ") + stage_name;
	{
		std::lock_guard<std::mutex> lock(g_log_mutex);
		g_current_stage = stage_name;
	}
	WriteLine("阶段", text.c_str());
}

void RunLogger::StageOk(const char* detail)
{
	std::string text = std::string("<<< 完成: ") + g_current_stage;
	if (detail && detail[0])
		text += std::string(" — ") + detail;
	WriteLine("阶段", text.c_str());
}

void RunLogger::StageFail(const char* detail)
{
	std::string text = std::string("!!! 失败: ") + g_current_stage;
	if (detail && detail[0])
		text += std::string(" — ") + detail;
	WriteLine("阶段", text.c_str());
}

void RunLogger::Info(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	WriteFormatted("INFO", fmt, args);
	va_end(args);
}

void RunLogger::Warn(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	WriteFormatted("WARN", fmt, args);
	va_end(args);
}

void RunLogger::Error(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	WriteFormatted("ERROR", fmt, args);
	va_end(args);
}

void RunLogger::LogPrintf(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	WriteFormatted("LOG", fmt, args);
	va_end(args);
}

const char* RunLogger::GetLogFilePath()
{
	return g_log_path.c_str();
}

const char* RunLogger::GetCurrentStage()
{
	return g_current_stage.c_str();
}

bool RunLogger::IsInitialized()
{
	return g_initialized;
}

bool RunLogger::WasShutdownCalled()
{
	return g_shutdown_called;
}