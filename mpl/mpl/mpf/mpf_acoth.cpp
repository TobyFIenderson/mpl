#include "mpf.h"
MPL_BEGIN
void mpf::acoth(mpf const& a, mpf& c)
{
	if (mpf::abs_cmp(a, 1) != ord::gt)
	{
		throw MPL_ERR_INVALID_VAL;
	}
	mpf::inv(a, c);
	mpf::atanh(c, c);
}
MPL_END