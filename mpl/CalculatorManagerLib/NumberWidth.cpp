#include "NumberWidth.h"
#include <array>
#include <cassert>
using namespace mpl;
using namespace std;
CM_BEGIN
array<mpi, CHOP_NUM_LEN> NumberWidth::chop_numbers = []()
{
	assert(chop_numbers.size() >= 4);
	array<mpi, CHOP_NUM_LEN> t;
	t[static_cast<size_t>(NumberWidthType::QWord)] =
		0b1111'1111'1111'1111'1111'1111'1111'1111'1111'1111'1111'1111'1111'1111'1111'1111;
	t[static_cast<size_t>(NumberWidthType::DWord)] = 0b1111'1111'1111'1111'1111'1111'1111'1111;
	t[static_cast<size_t>(NumberWidthType::Word)] = 0b1111'1111'1111'1111;
	t[static_cast<size_t>(NumberWidthType::Byte)] = 0b1111'1111;
	return t;
}();

array<mpi, CHOP_NUM_LEN> NumberWidth::max_numbers = []()
{
	array<mpi, CHOP_NUM_LEN> t;
	for (size_t i = 0; i < CHOP_NUM_LEN; i++)
	{
		t[i] = NumberWidth::chop_numbers[i];
		t[i].rsh(1);
	}
	return t;
}();

array<mpi, CHOP_NUM_LEN> NumberWidth::min_numbers = []()
{
	array<mpi, CHOP_NUM_LEN> t;
	for (size_t i = 0; i < CHOP_NUM_LEN; i++)
	{
		t[i] = NumberWidth::max_numbers[i];
		t[i].incr();
	}
	return t;
}();

array<
	array<string, CATEGORY_NUM_OF_RADIX>,
	CHOP_NUM_LEN> NumberWidth::max_num_strings = []()
{
	array<
		array<string, CATEGORY_NUM_OF_RADIX>,
		CHOP_NUM_LEN> t;
	for (size_t i = 0; i < CHOP_NUM_LEN; i++)
	{
		for (size_t j = 0; j < CATEGORY_NUM_OF_RADIX; j++)
		{
			t[i][j] = NumberWidth::max_numbers[i].to_str(Radix(RadixType(j)));
		}
	}
	return t;
}();

array<
	array<string, CATEGORY_NUM_OF_RADIX>,
	CHOP_NUM_LEN> NumberWidth::min_num_strings = []()
{
	array<
		array<string, CATEGORY_NUM_OF_RADIX>,
		CHOP_NUM_LEN> t;
	for (size_t i = 0; i < CHOP_NUM_LEN; i++)
	{
		for (size_t j = 0; j < CATEGORY_NUM_OF_RADIX; j++)
		{
			t[i][j] = NumberWidth::min_numbers[i].to_str(Radix(RadixType(j)));
		}
	}
	return t;
}();



NumberWidth::NumberWidth() : NumberWidth(NumberWidthType::QWord) {}

NumberWidth::NumberWidth(NumberWidthType number_width_type) : value(number_width_type) {}

mpi const& NumberWidth::GetChopNumber() const
{
	return chop_numbers[static_cast<size_t>(value)];
}

mpi const& NumberWidth::GetMaxNumber() const
{
	return max_numbers[static_cast<size_t>(value)];
}

mpi const& NumberWidth::GetMinNumber() const
{
	return min_numbers[static_cast<size_t>(value)];
}

string const& NumberWidth::GetMaxNumberString(Radix const& radix) const
{
	return max_num_strings[static_cast<size_t>(value)][radix.ToIndex()];
}

string const& NumberWidth::GetMinNumberString(Radix const& radix) const
{
	return min_num_strings[static_cast<size_t>(value)][radix.ToIndex()];
}

size_t NumberWidth::GetMaxNumberDigit(Radix const& radix) const
{
	return min_num_strings[static_cast<size_t>(value)][radix.ToIndex()].size();
}

NumberWidth::operator int() const
{
	switch (value)
	{
	case NumberWidthType::DWord:
		return 32;
	case NumberWidthType::Word:
		return 16;
	case NumberWidthType::Byte:
		return 8;
	case NumberWidthType::QWord:
	default:
		return 64;
	}
}

bool operator==(NumberWidth const& lhs, NumberWidth const& rhs)
{
	return lhs.value == rhs.value;
}

bool operator!=(NumberWidth const& lhs, NumberWidth const& rhs)
{
	return lhs.value != rhs.value;
}

NumberWidth& NumberWidth::operator=(NumberWidth const& number_width)
{
	value = number_width.value;
	return *this;
}

NumberWidthType const& NumberWidth::ToNumberWidthType() const
{
	return value;
}

size_t NumberWidth::ToIndex() const
{
	return static_cast<size_t>(value);
}

CM_END