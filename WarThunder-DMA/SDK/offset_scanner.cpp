#include "pch.h"
#include "offset_scanner.h"
#include "globals/globals.h"
#include "../Memory/Memory.h"
#include "offsets.h"

namespace offset_scanner
{
	static bool IsHeapOrGamePtr(uint64_t addr)
	{
		return addr >= globals::game_base && addr <= globals::max_valid_addr;
	}

	std::vector<uint64_t> FindPointerAll(uint64_t pointer_value)
	{
		std::vector<uint64_t> results;
		if (!pointer_value || !globals::game_base)
			return results;

		const uint64_t start_addr = globals::game_base;
		const uint64_t end_addr = start_addr + globals::game_size;
		const size_t chunk_size = 4096;
		const size_t pointer_size = 8;

		std::vector<char> buffer(chunk_size + pointer_size);
		for (uint64_t addr = start_addr; addr < end_addr; addr += chunk_size)
		{
			const size_t read_size = (size_t)std::min(chunk_size + pointer_size, end_addr - addr);
			if (!mem.Read(addr, buffer.data(), read_size))
				continue;

			for (size_t i = 0; i + pointer_size <= read_size; ++i)
			{
				const uint64_t value = *reinterpret_cast<uint64_t*>(buffer.data() + i);
				if (value == pointer_value)
					results.push_back(addr + i);
			}
		}
		return results;
	}

	uint64_t ExtractRipRelative(uint64_t instr_addr, int disp_offset, int instr_size)
	{
		const int32_t rel = mem.Read<int32_t>(instr_addr + disp_offset);
		return instr_addr + instr_size + rel;
	}

	uint64_t ScanTextForRipRelativeRva(const RipSignature& sig)
	{
		if (!sig.pattern || !globals::game_base)
			return 0;

		uint64_t cursor = globals::game_base;
		const uint64_t end = globals::game_base + globals::game_size;

		while (cursor < end)
		{
			const uint64_t match = mem.FindSignature(sig.pattern, cursor, end);
			if (!match)
				break;

			const uint64_t abs = ExtractRipRelative(match, sig.disp_offset, sig.instr_size);
			if (abs >= globals::game_base && abs < globals::game_base + globals::game_size)
				return abs - globals::game_base;

			cursor = match + 1;
		}
		return 0;
	}

	bool ValidateCGameRva(uintptr_t cgame_rva)
	{
		if (!cgame_rva)
			return false;

		const uintptr_t instance = mem.Read<uintptr_t>(globals::game_base + cgame_rva);
		return instance != 0 && IsHeapOrGamePtr(instance);
	}

	bool ValidateMPlayerRva(uintptr_t m_player_rva)
	{
		if (!m_player_rva)
			return false;

		const uintptr_t local_player = mem.Read<uintptr_t>(
			globals::game_base + m_player_rva + offsets::m_player::local_player);

		return local_player != 0 && IsHeapOrGamePtr(local_player);
	}

	static ResolveResult ResolveFromStringGlobal(
		const char* string_label,
		uintptr_t field_offset,
		bool (*validator)(uintptr_t))
	{
		ResolveResult result;
		const uint64_t string_addr = globals::FindString(string_label);
		if (!string_addr)
		{
			LOG("[Scanner] String '%s' not found\n", string_label);
			return result;
		}

		const auto pointer_sites = FindPointerAll(string_addr);
		LOG("[Scanner] '%s' @ 0x%llx, %zu pointer site(s)\n",
			string_label,
			(unsigned long long)string_addr,
			pointer_sites.size());

		for (const uint64_t site : pointer_sites)
		{
			const uintptr_t global_rva = static_cast<uintptr_t>(site - globals::game_base);
			const uintptr_t candidate = global_rva + field_offset;
			if (validator(candidate))
			{
				result.rva = candidate;
				result.method = "string_ref";
				result.ok = true;
				LOG("[Scanner] '%s' resolved via string_ref -> RVA 0x%llx\n",
					string_label, (unsigned long long)candidate);
				return result;
			}
		}

		LOG("[Scanner] '%s' string_ref candidates failed validation\n", string_label);
		return result;
	}

	ResolveResult ResolveCGame()
	{
		// Primary: Dagor engine registers "mem.memreport_sys"; CGame lives at +0x40 in that global block.
		auto result = ResolveFromStringGlobal(
			"mem.memreport_sys",
			offsets::CGame_field_offset,
			ValidateCGameRva);

		if (result.ok)
			return result;

		// Secondary: code signatures (update patterns in IDA after game updates).
		static const RipSignature signatures[] = {
			// mov rax, [rip+CGame]; common x64 global load — replace with your IDA pattern:
			// { "cgame_load", "48 8B 05 ?? ?? ?? ??", 3, 7 },
		};

		for (const auto& sig : signatures)
		{
			const uint64_t rva = ScanTextForRipRelativeRva(sig);
			if (rva && ValidateCGameRva(static_cast<uintptr_t>(rva)))
			{
				result.rva = static_cast<uintptr_t>(rva);
				result.method = sig.name;
				result.ok = true;
				LOG("[Scanner] CGame resolved via signature '%s' -> RVA 0x%llx\n",
					sig.name, (unsigned long long)rva);
				return result;
			}
		}

		// Fallback: last known hardcoded RVA
		if (ValidateCGameRva(offsets::fallback::CGame))
		{
			result.rva = offsets::fallback::CGame;
			result.method = "fallback";
			result.ok = true;
			LOG("[Scanner] CGame using hardcoded fallback RVA 0x%llx\n",
				(unsigned long long)result.rva);
			return result;
		}

		LOG("[Scanner] CGame resolution failed\n");
		return result;
	}

	ResolveResult ResolveMPlayer()
	{
		auto result = ResolveFromStringGlobal(
			"m_player",
			0,
			ValidateMPlayerRva);

		if (result.ok)
			return result;

		static const RipSignature signatures[] = {
			// { "m_player_load", "48 8B 0D ?? ?? ?? ??", 3, 7 },
		};

		for (const auto& sig : signatures)
		{
			const uint64_t rva = ScanTextForRipRelativeRva(sig);
			if (rva && ValidateMPlayerRva(static_cast<uintptr_t>(rva)))
			{
				result.rva = static_cast<uintptr_t>(rva);
				result.method = sig.name;
				result.ok = true;
				LOG("[Scanner] m_player resolved via signature '%s' -> RVA 0x%llx\n",
					sig.name, (unsigned long long)rva);
				return result;
			}
		}

		if (ValidateMPlayerRva(offsets::fallback::m_player_ptr))
		{
			result.rva = offsets::fallback::m_player_ptr;
			result.method = "fallback";
			result.ok = true;
			LOG("[Scanner] m_player using hardcoded fallback RVA 0x%llx\n",
				(unsigned long long)result.rva);
			return result;
		}

		LOG("[Scanner] m_player resolution failed\n");
		return result;
	}
}