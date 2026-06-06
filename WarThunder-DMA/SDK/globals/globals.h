#pragma once
class globals
{
public:
	static uint64_t game_base;
	static size_t game_size;
	static uint64_t max_valid_addr;
	static std::string ReadString(uint64_t address, size_t max_length);
	static std::string ConvertWideToUTF8(const std::wstring& wstr);


	static uint64_t FindString(const char* searchString);
	static uint64_t FindPointer(uint64_t pointer_value);


	globals();
private:

};
