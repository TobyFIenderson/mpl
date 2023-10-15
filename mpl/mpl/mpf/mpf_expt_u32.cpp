#include "mpf.h"
MPL_BEGIN
void mpf::expt_u32(mpf const& a, uint32_t b, mpf& c)
{
	mpf g = a;
	c.set_d(1);
	while (b)
	{
		if ((b & 1) != 0u)
		{
			mul(c, g, c);
		}
		if (b > 1u)
		{
			sqr(g, g);
		}
		b >>= 1;
	}
}
MPL_END