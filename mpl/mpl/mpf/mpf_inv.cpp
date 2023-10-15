#include "mpf.h"
MPL_BEGIN
void mpf::inv(mpf const& a, mpf& c)
{
	if (a.is_zero())
	{
		throw MPL_ERR_DIV_BY_ZERO;
	}
	if (a.is_abs_one())
	{
		c = a;
		return;
	}
	mpf b = a;
	mpi_digit* bdp = b.mant.dp;
	for (int32_t i = 0; i < int32_t(b.mant.used) - 1; i++)
	{
		*bdp++ = 0;
	}
	*bdp = 1;
	b.exp = 1 - int32_t(b.mant.used);
	mpf::div(b, a, c);
	c.mant.sign = b.mant.sign;
	c.clamp_tail();
}
MPL_END