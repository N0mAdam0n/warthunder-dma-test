#pragma once
#include <cstdint>
#include "Offset_manual_update.h"

namespace offsets
{
	// Runtime image RVAs — populated by offsets::Resolve() at startup.
	inline uintptr_t CGame = 0;
	inline uintptr_t m_player_ptr = 0;

	inline bool resolved = false;
	inline const char* cgame_resolve_method = "";
	inline const char* mplayer_resolve_method = "";

	bool Resolve();
}