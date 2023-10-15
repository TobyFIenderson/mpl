#include "mpf.h"
MPL_BEGIN
void mpf::sec(mpf const& a, ang b, mpf& c)
{
	mpf::cos(a, b, c);
	if (c.is_zero())
	{
		throw MPL_ERR_INVALID_VAL;
	}
	mpf::inv(c, c);
}
MPL_END