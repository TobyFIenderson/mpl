#include "mpf.h"
MPL_BEGIN

void mpf::abs(mpf const& a, mpf& c)
{
	c = a;
	c.mant.sign = 1;
}

MPL_END