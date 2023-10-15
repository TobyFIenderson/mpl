#include "mpf.h"
MPL_BEGIN
void mpf::atanh(mpf const& a, mpf& c)
{
	if (a.is_zero())
	{
		c.set_d(0);
		return;
	}

	if (mpf::abs_cmp(a, 1) != ord::lt)
	{
		throw MPL_ERR_INVALID_VAL;
	}

	mpf t = a;
	mpf::add(1, a, t);
	mpf::sub(1, a, c);
	mpf::div(t, c, c);
	mpf::ln(c, c);
	mpf::div_2(c, c);
}
MPL_END