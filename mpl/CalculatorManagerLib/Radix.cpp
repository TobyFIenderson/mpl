#include "Radix.h"
CM_BEGIN
Radix::Radix() : val(DEFAULT_RADIX_TYPE) {}
Radix::Radix(RadixType rt) : val(rt) {}
Radix::operator int() const
{
	switch (val)
	{
	case CalculatorManagerLib::RadixType::Binary:
		return 2;
		break;
	case CalculatorManagerLib::RadixType::Octal:
		return 8;
		break;
	case CalculatorManagerLib::RadixType::Decimal:
		return 10;
		break;
	case CalculatorManagerLib::RadixType::Hexadecimal:
		return 16;
		break;
	case CalculatorManagerLib::RadixType::Null:
		return 10;
		break;
	default:
		break;
	}
	return 10;
}
Radix& Radix::operator=(Radix const& r)
{
	val = r.val;
	return *this;
}
RadixType Radix::ToRadixType() const
{
	return val;
}
size_t Radix::ToIndex() const
{
	return static_cast<size_t>(val);
}
bool operator==(Radix const& l, Radix const& r)
{
	return l.val == r.val;
}
bool operator!=(Radix const& l, Radix const& r)
{
	return l.val != r.val;
}
CM_END