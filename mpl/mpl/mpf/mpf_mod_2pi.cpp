#include "mpf.h"
MPL_BEGIN
void mpf::mod_2pi(mpf const& a, mpf& c)
{
	ord o = mpf::abs_cmp(a, mpf::const_2pi());
	if (o == ord::lt)
	{
		c = a;
		return;
	}
	if (o == ord::eq)
	{
		c.set_d(0);
		return;
	}
	/* newprec >= MPF_ORIGINAL_PRECISION */
	int32_t newprec = std::max(a.rdig() + MPF_ORIGINAL_PRECISION, MPF_ORIGINAL_PRECISION);
	int32_t oldprec = MPF_ORIGINAL_PRECISION;
	MPF_ORIGINAL_PRECISION = newprec;
	mpf::mod(a, mpf::const_2pi(), c);
	MPF_ORIGINAL_PRECISION = oldprec;
	c.trimit();
}
MPL_END