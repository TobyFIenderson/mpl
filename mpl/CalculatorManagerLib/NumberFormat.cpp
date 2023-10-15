#include "NumberFormat.h"
CM_BEGIN
NumberFormat::NumberFormat() : val(DEFAULT_NUM_FMT_TYPE) {}
NumberFormat::NumberFormat(mpl::num_format nf)
{
	switch (nf)
	{
	case mpl::num_format::flo:
		val = NumberFormatType::Float;
		break;
	case mpl::num_format::sci:
		val = NumberFormatType::Scientific;
		break;
	case mpl::num_format::eng:
		val = NumberFormatType::Engineering;
		break;
	default:
		break;
	}
	val = NumberFormatType::Null;
}

NumberFormat::NumberFormat(NumberFormatType nft) :val(nft) {}

NumberFormat& NumberFormat::operator=(NumberFormat const& r)
{
	val = r.val;
	return *this;
}

NumberFormatType NumberFormat::ToNumberFormatType()
{
	return val;
}

NumberFormat::operator mpl::num_format()
{
	switch (val)
	{
	case CalculatorManagerLib::NumberFormatType::Float:
		return mpl::num_format::flo;
	case CalculatorManagerLib::NumberFormatType::Scientific:
		return mpl::num_format::sci;
	case CalculatorManagerLib::NumberFormatType::Engineering:
		return mpl::num_format::eng;
	}
	return mpl::num_format::flo;
}

NumberFormat::operator int()
{
	return int(val);
}

bool operator==(NumberFormat const& l, NumberFormat const& r)
{
	return l.val == r.val;
}

bool operator!=(NumberFormat const& l, NumberFormat const& r)
{
	return l.val != r.val;
}

CM_END