#include "mpf.h"
MPL_BEGIN
void mpf::csch(mpf const& a, mpf& c)
{
	if (a.is_zero())
	{
		throw MPL_ERR_INVALID_VAL;
	}
	mpf::sinh(a, c);
	mpf::inv(c, c);
}
MPL_END