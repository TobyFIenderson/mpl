#include "mpf.h"
MPL_BEGIN
void mpf::div_2(mpf const& a, mpf& c)
{
	if (a.is_zero())
	{
		c.set_d(0);
		return;
	}
	c = a;
	if ((c.mant.dp[0] & 1) == 0)
	{
		c.mant.rsh(1);
	}
	else
	{
		c.mant.lsh_x(1);
		c.exp--;
		c.mant.rsh(1);
		c.trimit();
	}
}
MPL_END