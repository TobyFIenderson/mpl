#include "mpf_radix.h"
MPL_BEGIN

std::string mpf_radix::to_str(num_format fmt, int32_t precision) const
{
	if (mant.empty())
	{
		return "0";
	}
	mpf_radix tmp = *this;
	tmp.trimit(precision, true);

	switch (fmt)
	{
	case num_format::flo:
		return tmp.to_flo_str();
		break;
	case num_format::sci:
		return tmp.to_sci_str();
		break;
	default:
		return "BAD FORMAT!!!";
		break;
	}
}

MPL_END