#include "pch.h"
#include "globals.h"

uint64_t globals::game_base = 0x0;
size_t globals::game_size = 0x0;
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

std::string globals::ConvertWideToUTF8(const std::wstring& wstr) {
    if (wstr.empty())
    {
        return std::string();
    }

    int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
    std::string strTo(size_needed, 0);

    WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);

    return strTo;
}

uint64_t globals::FindString(const char* searchString)
{
    if (!searchString || !game_base)
        return 0;

    size_t len = strlen(searchString);
    if (len == 0)
        return 0;


    uint64_t start_addr = game_base;
    uint64_t end_addr = start_addr + globals::game_size;
    const size_t chunk_size = 4096;

    char* buffer = (char*)malloc(chunk_size + len);
    if (!buffer)
        return 0;

    for (uint64_t addr = start_addr; addr < end_addr; addr += chunk_size) {
        size_t read_size = (size_t)std::min(chunk_size + len, end_addr - addr);

        if (mem.Read(addr, buffer, read_size)) {
            for (size_t i = 0; i <= read_size - len; ++i) {
                if (memcmp(buffer + i, searchString, len) == 0) {
                    uint64_t found_addr = addr + i;
                    free(buffer);
                    return found_addr;
                }
            }
        }
    }

    free(buffer);
    return 0;
}

uint64_t globals::FindPointer(uint64_t pointer_value)
{
    if (!game_base)
        return 0;


    uint64_t start_addr = game_base;
    uint64_t end_addr = start_addr + globals::game_size;
    const size_t chunk_size = 4096;
    const size_t pointer_size = 8;

    char* buffer = (char*)malloc(chunk_size + pointer_size);
    if (!buffer)
        return 0;

    for (uint64_t addr = start_addr; addr < end_addr; addr += chunk_size) {
        size_t read_size = (size_t)std::min(chunk_size + pointer_size, end_addr - addr);

        if (mem.Read(addr, buffer, read_size)) {
            for (size_t i = 0; i <= read_size - pointer_size; ++i) {
                uint64_t* ptr = (uint64_t*)(buffer + i);
                if (*ptr == pointer_value) {
                    uint64_t found_addr = addr + i;
                    free(buffer);
                    return found_addr;
                }
            }
        }
    }

    free(buffer);
    return 0;
}
