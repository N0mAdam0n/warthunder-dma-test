#pragma once
#include <cstdint>
#include <vector>
#include <string>

// Resolves image-level RVAs (CGame, m_player, etc.) at runtime via:
//   1. String reference scan  — find engine registration strings, locate globals
//   2. Code signature scan    — RIP-relative operand extraction from .text patterns
//   3. Hardcoded fallback     — last resort when both methods fail
//
// Struct member offsets (_CUnit::position, _CGame::unit_list3, …) are still
// compile-time constants; those are class-layout offsets and require RE per major
// patch. See offsets.h for details.

namespace offset_scanner
{
	struct RipSignature
	{
		const char* name;
		const char* pattern;   // IDA style, e.g. "48 8B 05 ?? ?? ?? ??"
		int         disp_offset; // byte offset of rel32 inside the matched instruction
		int         instr_size;  // total instruction length (for RIP calculation)
	};

	struct ResolveResult
	{
		uintptr_t rva = 0;
		const char* method = nullptr;
		bool ok = false;
	};

	std::vector<uint64_t> FindPointerAll(uint64_t pointer_value);

	uint64_t ExtractRipRelative(uint64_t instr_addr, int disp_offset, int instr_size);
	uint64_t ScanTextForRipRelativeRva(const RipSignature& sig);

	ResolveResult ResolveCGame();
	ResolveResult ResolveMPlayer();

	bool ValidateCGameRva(uintptr_t cgame_rva);
	bool ValidateMPlayerRva(uintptr_t m_player_rva);
}