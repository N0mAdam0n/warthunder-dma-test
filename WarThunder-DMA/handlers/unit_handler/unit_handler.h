#pragma once

#include "../SDK/CUnit/CUnit.h"
#include <Warthunder.h>

class UnitHandler
{
public:
	UnitHandler(Warthunder *wt);
	std::vector<CUnit> complete_units;
	std::mutex complete_units_mutex;
private:
	std::vector<CUnit> temp_units;
};

