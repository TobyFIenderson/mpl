#pragma once
#ifndef __INCLUDE_NUMBER_FORMAT_H__
#define __INCLUDE_NUMBER_FORMAT_H__
#include "CalculatorManagerLibNS.h"
#include "../mpl/mpf_radix/mpf_radix.h"
CM_BEGIN
enum class NumberFormatType
{
	Float = 0,		// 浮点计数法
	Scientific = 1,	// 科学计数法
	Engineering = 2, // 工程计数法
	Null = 3
};

inline constexpr NumberFormatType DEFAULT_NUM_FMT_TYPE = NumberFormatType::Float;

class NumberFormat
{
public:
	static const mpl::num_format Float = mpl::num_format::flo;
	static const mpl::num_format Scientific = mpl::num_format::sci;
	static const mpl::num_format Engineering = mpl::num_format::eng;
	static const NumberFormatType Null = NumberFormatType::Null;

	NumberFormat();
	NumberFormat(mpl::num_format nf);
	NumberFormat(NumberFormatType nft);
	NumberFormat& operator=(NumberFormat const& r);
	friend bool operator==(NumberFormat const& l, NumberFormat const& r);
	friend bool operator!=(NumberFormat const& l, NumberFormat const& r);
	NumberFormatType ToNumberFormatType();
	operator mpl::num_format();
	operator int();
private:
	NumberFormatType val;
};

CM_END
#endif // !__INCLUDE_NUMBER_FORMAT_H__
