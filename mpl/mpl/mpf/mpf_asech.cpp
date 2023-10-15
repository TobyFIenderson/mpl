#include "mpf.h"
MPL_BEGIN
void mpf::asech(mpf const& a, mpf& c)
{
	if (a.is_neg() || a.is_zero() || mpf::cmp(a, 1) == ord::gt)
	{
		throw MPL_ERR_INVALID_VAL;
	}
	if (a.is_abs_one())
	{
		c.set_d(0);
		return;
	}
	mpf::inv(a, c);
	mpf::acosh(c, c);
}
MPL_END