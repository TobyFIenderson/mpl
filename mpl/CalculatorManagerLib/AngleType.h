#pragma once
#ifndef __INCLUDE_ANGLE_TYPE_H__
#define __INCLUDE_ANGLE_TYPE_H__
#include "CalculatorManagerLibNS.h"
#include "../mpl/mpf/mpf.h"
CM_BEGIN
enum class AngleType
{
	Degree = 0, // 360
	Radians = 1, // 2pi
	Gradians = 2, // 400
	Null = 3
};
inline mpl::ang AngleTypeToAng(AngleType at)
{
	switch (at)
	{
	case CalculatorManagerLib::AngleType::Degree:
		return mpl::ang::deg;
	case CalculatorManagerLib::AngleType::Radians:
		return mpl::ang::rad;
	case CalculatorManagerLib::AngleType::Gradians:
		return mpl::ang::grad;
	}
	throw MPL MPL_ERR_INVALID_VAL;
}
CM_END
#endif // !__INCLUDE_ANGLE_TYPE_H__
