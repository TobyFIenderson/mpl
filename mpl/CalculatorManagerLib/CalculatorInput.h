#pragma once
#ifndef __INCLUDE_CALCULATOR_INPUT_H__
#define __INCLUDE_CALCULATOR_INPUT_H__

#include "CalculatorManagerLibNS.h"
#include "Radix.h"
#include "NumberWidth.h"
#include "CalculatorMode.h"
#include "../mpl/mpf/mpf.h"
CM_BEGIN

inline constexpr size_t MAX_EXP_LEN = 32;
/* 32 是完全够用了，因为 int 类型最多可以表示 9 位 */
inline constexpr size_t MAX_STR_LEN = 1024 + 32 + 4;

class InputString
{
public:
	InputString() :value(), is_negative(false) {}

	void Clear()
	{
		value.clear();
		is_negative = false;
	}

	std::string value;
	bool is_negative;
};

class CalculatorInput
{
public:
	CalculatorInput(char dec_pt, Radix const& o_radix, NumberWidth const& number_width, CalculatorMode calc_mode, int32_t precision);
	void Clear();
	void SetRadix(Radix const& new_radix);
	void SetNumberWidth(NumberWidth const& new_num_width);
	void SetCalculatorMode(CalculatorMode calc_mode);
	void SetDecimalSymbol(char dec_pt);
	void SetPrecision(int32_t new_prec);
	void Backspace();

	bool TryAddDigit(unsigned int value);
	bool TryToggleSign();
	bool TryAddDecimalPoint();
	bool TryAddExponentBegin();
	bool IsEmpty();
	bool HasDecimalPoint();
	bool IsBinaryBaseNegative();

	std::string ToString();
	mpl::mpf ToMpf();

private:
	bool has_exponent;
	bool has_decimal_point;
	size_t decimal_point_index;
	char decimal_point;
	InputString base;
	InputString exponent;
	Radix radix;
	NumberWidth word_bit_width;
	int32_t precision;
	CalculatorMode calculator_mode;
};

CM_END

#endif // !__INCLUDE_CALCULATOR_INPUT_H__