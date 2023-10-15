#include "mpf_radix.h"
MPL_BEGIN
mpf_radix& mpf_radix::operator=(mpf_radix const& m)
{
	sign = m.sign;
	mant = m.mant;
	radix = m.radix;
	exp = m.exp;
	return *this;
}

mpf_radix& mpf_radix::operator=(mpf_radix&& m) noexcept
{
	sign = m.sign;
	mant = std::move(m.mant);
	radix = m.radix;
	exp = m.exp;
	return *this;
}
MPL_END