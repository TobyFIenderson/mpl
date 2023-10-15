#include "mpf.h"
MPL_BEGIN
/* 1/sinx */
void mpf::csc(mpf const& a, ang b, mpf& c)
{
	mpf::sin(a, b, c);
	if (c.is_zero())
	{
		throw MPL_ERR_INVALID_VAL;
	}
	mpf::inv(c, c);
}
MPL_END