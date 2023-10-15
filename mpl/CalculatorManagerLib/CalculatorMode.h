#pragma once
#ifndef __INCLUDE_CALCULATOR_MODE_H__
#define __INCLUDE_CALCULATOR_MODE_H__
#include "CalculatorManagerLibNS.h"
#include "../mpl/mpf/mpf.h"
CM_BEGIN
enum class CalculatorMode
{
	Standard = 0,
	Scientific = 1,
	Programmer = 2,
	Statistical = 3,
	Null = 4
};

inline int GetMaxInputLength(CalculatorMode cm)
{
	int ratio = int(std::log10(MPL MPI_BASE));
	ratio = std::max(ratio, 1);
	switch (cm)
	{
	case CalculatorMode::Standard: return 16;
	case CalculatorMode::Scientific: return MPL MPF_ORIGINAL_PRECISION * ratio;
	case CalculatorMode::Programmer: return 64;
	case CalculatorMode::Statistical: return 32;
	default: return 32;
	}
}

CM_END
#endif // !__INCLUDE_CALCULATOR_MODE_H__
