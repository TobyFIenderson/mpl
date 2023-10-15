#pragma once
#ifndef __INCLUDE__RADIX_H__
#define __INCLUDE__RADIX_H__

#include "CalculatorManagerLibNS.h"

CM_BEGIN

enum class RadixType
{
	Binary = 0,
	Octal = 1,
	Decimal = 2, // default
	Hexadecimal = 3,
	Null = 4
};

/* RadixType 一共有 4 种 */
inline constexpr size_t CATEGORY_NUM_OF_RADIX = static_cast<size_t>(RadixType::Null);
inline constexpr RadixType DEFAULT_RADIX_TYPE = RadixType::Decimal;

class Radix
{
public:
	Radix();
	Radix(RadixType rt);

	operator int() const;
	Radix& operator=(Radix const& r);
	friend bool operator==(Radix const& l, Radix const& r);
	friend bool operator!=(Radix const& l, Radix const& r);

	RadixType ToRadixType() const;
	size_t ToIndex() const;

private:
	RadixType val;
};

CM_END

#endif // !__INCLUDE__RADIX_H__