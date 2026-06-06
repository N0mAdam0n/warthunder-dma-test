#include "pch.h"
#include "CPredictionEngine.h"
#include "../../memory/Memory.h"
#include "../offsets.h"

auto CPredictionEngine::is_valid() -> const bool
{
	return this->base_address != 0;
}

auto CPredictionEngine::selected_unit() -> CUnit
{
	return CUnit(mem.Read<std::uintptr_t>(this->base_address + offsets::_CPredictionEngine::selected_unit));
}

auto CPredictionEngine::predicted_position() -> const Vector3
{
	return mem.Read<Vector3>(this->base_address + offsets::_CPredictionEngine::selected_unit_prediction);
}

auto CPredictionEngine::is_unit_selected() -> const bool
{
	return selected_unit().is_valid() && mem.Read<float>(this->base_address + offsets::_CPredictionEngine::selected_unit_prediction) != 0.f;
}
