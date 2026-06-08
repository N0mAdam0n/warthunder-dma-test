#include "pch.h"
#include "globals.h"
#include "../../Memory/Memory.h"

uint64_t globals::game_base = 0;
size_t globals::game_size = 0;
uint64_t globals::max_valid_addr = 0x7FFFFFFFFFFF;

globals::globals()
{
	game_base = mem.GetBaseDaddy("aces.exe");
	game_size = mem.GetBaseSize("aces.exe");
}

std::string globals::ReadString(uint64_t address, size_t size)
{
	if (!address || size > 1024)
		return "";

	char string[1024] = { 0 };
	if (!mem.Read(address, string, size))
		return "";

	return std::string(string);
}

uint64_t globals::FindString(const char* searchString)
{
	if (!searchString || !game_base)
		return 0;

	const size_t len = strlen(searchString);
	if (len == 0)
		return 0;

	const uint64_t start_addr = game_base;
	const uint64_t end_addr = start_addr + game_size;
	const size_t chunk_size = 4096;

	char* buffer = static_cast<char*>(malloc(chunk_size + len));
	if (!buffer)
		return 0;

	for (uint64_t addr = start_addr; addr < end_addr; addr += chunk_size)
	{
		const size_t read_size = (size_t)std::min(chunk_size + len, end_addr - addr);
		if (mem.Read(addr, buffer, read_size))
		{
			for (size_t i = 0; i <= read_size - len; ++i)
			{
				if (memcmp(buffer + i, searchString, len) == 0)
				{
					const uint64_t found_addr = addr + i;
					free(buffer);
					return found_addr;
				}
			}
		}
	}

	free(buffer);
	return 0;
}