#include "mpf.h"
MPL_BEGIN
void mpf::acosh(mpf const& a, mpf& c)
{
	if (mpf::cmp(a, 1) == ord::lt)
	{
		throw MPL_ERR_INVALID_VAL;
	}
	if (a.is_abs_one())
	{
		c.set_d(0);
		return;
	}
	mpf t = a;
	mpf::sqr(t, t);
	mpf::sub(t, 1, t);
	mpf::sqrt(t, t);
	mpf::add(t, a, t);
	mpf::ln(t, c);
}
MPL_END