#include "pch.h"
#include "offsets.h"
#include "offset_scanner.h"

bool offsets::Resolve()
{
	if (resolved)
		return true;

	const auto cgame = offset_scanner::ResolveCGame();
	const auto mplayer = offset_scanner::ResolveMPlayer();

	if (cgame.ok)
	{
		CGame = cgame.rva;
		cgame_resolve_method = cgame.method;
	}
	if (mplayer.ok)
	{
		m_player_ptr = mplayer.rva;
		mplayer_resolve_method = mplayer.method;
	}

	resolved = cgame.ok && mplayer.ok;

	if (resolved)
	{
		RunLogger::Info("偏移解析成功 — CGame=0x%llX (%s), m_player=0x%llX (%s)",
			(unsigned long long)CGame, cgame_resolve_method,
			(unsigned long long)m_player_ptr, mplayer_resolve_method);
	}
	else
	{
		RunLogger::Error("偏移解析不完整 — CGame=%s, m_player=%s",
			cgame.ok ? "ok" : "FAILED",
			mplayer.ok ? "ok" : "FAILED");
	}

	return resolved;
}