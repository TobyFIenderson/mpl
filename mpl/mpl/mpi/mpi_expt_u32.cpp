#include "mpi.h"
MPL_BEGIN
void mpi::expt_u32(mpi const& a, uint32_t b, mpi& c)
{
	if (a.is_zero())
	{
		c.set_d(0);
		return;
	}
	if (b == 0)
	{
		c.set_d(1);
		return;
	}
	if (b == 1)
	{
		c = a;
		return;
	}

	mpi g = a;
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