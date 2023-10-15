#include "mpf.h"
MPL_BEGIN
void mpf::mul_2(mpf const& a, mpf& c)
{
	if (a.is_zero())
	{
		c.set_d(0);
		return;
	}
	c = a;
	c.mant.lsh(1);
	c.trimit();
}
MPL_END