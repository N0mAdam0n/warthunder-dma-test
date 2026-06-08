#pragma once
#include <cstdarg>
#include <string>

enum class RunExitStatus
{
	Running,
	Success,
	UserExit,
	Error,
	WarningContinue
};

class RunLogger
{
public:
	static bool Init();
	static void Shutdown(RunExitStatus status, const char* message = nullptr);

	static void StageBegin(const char* stage_name);
	static void StageOk(const char* detail = nullptr);
	static void StageFail(const char* detail = nullptr);

	static void Info(const char* fmt, ...);
	static void Warn(const char* fmt, ...);
	static void Error(const char* fmt, ...);
	static void LogPrintf(const char* fmt, ...);

	static const char* GetLogFilePath();
	static const char* GetCurrentStage();
	static bool IsInitialized();
	static bool WasShutdownCalled();

private:
	static void WriteLine(const char* level, const char* text);
	static void WriteFormatted(const char* level, const char* fmt, va_list args);
	static std::string MakeTimestamp();
	static void AtExitHandler();
};