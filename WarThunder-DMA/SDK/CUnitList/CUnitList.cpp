#include "pch.h"
#include "CUnitList.h"
#include "../../Memory/Memory.h"
#include "../offsets.h"

auto CUnitList::is_valid() -> const bool
{
	return this->base_address != 0;
}

std::vector<CUnit> CUnitList::unit_scatter(VMMDLL_SCATTER_HANDLE handle, uint32_t count)
{
    std::vector<std::uintptr_t> pointers(count);
    std::vector<CUnit> result;
    result.reserve(count);

    for (size_t i = 0; i < count; i++)
    {
        mem.AddScatterReadRequest(handle, this->base_address + 0x8 * i, &pointers[i]);
    }
    mem.ExecuteReadScatter(handle);

    for (size_t i = 0; i < count; i++)
    {
        result.emplace_back(CUnit(pointers.at(i)));
    }

    return result;
}

