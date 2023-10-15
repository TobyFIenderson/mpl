#include "mpf.h"
MPL_BEGIN
void mpf::acsch(mpf const& a, mpf& c)
{
	if (a.is_zero())
	{
		throw MPL_ERR_INVALID_VAL;
	}
	mpf::inv(a, c);
	mpf::asinh(a, c);
}
MPL_END