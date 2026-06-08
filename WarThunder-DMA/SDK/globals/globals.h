#pragma once
#include <cstdint>
#include <string>

class globals
{
public:
	static uint64_t game_base;
	static size_t game_size;
	static uint64_t max_valid_addr;

	static std::string ReadString(uint64_t address, size_t max_length);
	static uint64_t FindString(const char* searchString);

	globals();
};