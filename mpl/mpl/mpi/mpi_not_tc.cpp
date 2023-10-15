#include "mpi.h"
MPL_BEGIN
void mpi::not_tc(mpi const& a, mpi& c)
{
	c = a;
	if (c.used != 0)
	{
		c.sign *= -1;
	}
	sub_d(c, 1, c);
}
MPL_END