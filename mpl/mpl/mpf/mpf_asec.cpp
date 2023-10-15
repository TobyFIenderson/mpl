#include "mpf.h"
MPL_BEGIN
void mpf::asec(mpf const& a, ang b, mpf& c)
{
	if (a.is_zero())
	{
		throw MPL_ERR_INVALID_VAL;
	}
	c = a;
	mpf::inv(c, c);
	mpf::acos(c, b, c);
}
MPL_END