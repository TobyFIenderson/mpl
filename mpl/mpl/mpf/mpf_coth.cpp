#include "mpf.h"
MPL_BEGIN
void mpf::coth(mpf const& a, mpf& c)
{
	if (a.is_zero())
	{
		throw MPL_ERR_INVALID_VAL;
	}
	mpf b;
	mpf::sinh(a, b);
	mpf::cosh(a, c);
	mpf::div(c, b, c);
}
MPL_END