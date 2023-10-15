#pragma once
#ifndef __INCLUDE_NUMBER_WIDTH_H__
#define __INCLUDE_NUMBER_WIDTH_H__
#include "Radix.h"
#include "../mpl/mpi/mpi.h"
CM_BEGIN
enum class NumberWidthType
{
	Byte = 0,
	Word = 1,
	DWord = 2,
	QWord = 3, // default
	Null = 4
};

inline constexpr size_t CHOP_NUM_LEN = static_cast<size_t>(NumberWidthType::Null);
inline constexpr NumberWidthType DEFAULT_NUMBER_WIDTH_TYPE = NumberWidthType::QWord;
class NumberWidth
{
public:
	NumberWidth();
	NumberWidth(NumberWidthType number_width_type);

	mpl::mpi const& GetChopNumber() const;
	mpl::mpi const& GetMaxNumber() const;
	mpl::mpi const& GetMinNumber() const;
	std::string const& GetMaxNumberString(Radix const& radix) const;
	std::string const& GetMinNumberString(Radix const& radix) const;
	size_t GetMaxNumberDigit(Radix const& radix) const;
	operator int() const;
	friend bool operator==(NumberWidth const& lhs, NumberWidth const& rhs);
	friend bool operator!=(NumberWidth const& lhs, NumberWidth const& rhs);
	NumberWidth& operator=(NumberWidth const& number_width);

	NumberWidthType const& ToNumberWidthType() const;
	size_t ToIndex() const;
private:

	// 当前字宽的 -1 的补码表示。
	static std::array<mpl::mpi, CHOP_NUM_LEN> chop_numbers;

	// 127 1111 111
	static std::array<mpl::mpi, CHOP_NUM_LEN> max_numbers;

	// -128 1000 0000
	static std::array<mpl::mpi, CHOP_NUM_LEN> min_numbers;

	// max_num_strings 和  min_num_strings 存放的是当前字宽下能达到的最大最小值的字符串表达形式。
	// 例如如果当前字宽为 byte(8 位) ，那么 GetMaxNumberString(RadixType::Decimal) 和 GetMinNumberString(RadixType::Decimal) 
	// 分别返回 127 和 128 。十六进制下为 7F 和 80 。
	static std::array<
		std::array<std::string, CATEGORY_NUM_OF_RADIX >,
		CHOP_NUM_LEN> max_num_strings; // max_num_strings[NumberWidthType][RadixType]
	static std::array<
		std::array<std::string, CATEGORY_NUM_OF_RADIX >,
		CHOP_NUM_LEN> min_num_strings; // min_num_strings[NumberWidthType][RadixType]

	NumberWidthType value;
};

CM_END
#endif // !__INCLUDE_NUMBER_WIDTH_H__