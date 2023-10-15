#include "mpf.h"
MPL_BEGIN
void mpf::ceil(mpf const& a, mpf& c)
{
	mpf f = a.fraction();
	c = a.integer();
	if (!(a.is_neg() || f.is_zero()))
	{
		mpf::add(c, 1, c);
	}
}
MPL_END