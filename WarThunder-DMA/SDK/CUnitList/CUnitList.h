#pragma once
#include <cstdint>
#include <vector>
#include "../CUnit/CUnit.h"

class CUnitList
{
public:
	explicit CUnitList(const std::uintptr_t addr = 0) : base_address(addr) {}

	const std::uintptr_t operator=(const std::uintptr_t addr) {
		return this->base_address = addr;
	}

	auto is_valid() -> const bool;
	std::vector<CUnit> unit_scatter(VMMDLL_SCATTER_HANDLE handle, uint32_t count);

private:
	std::uintptr_t base_address = 0;
};