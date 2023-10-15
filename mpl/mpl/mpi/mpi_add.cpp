#include "mpi.h"
MPL_BEGIN

void mpi::mpi::add(mpi const& a, mpi const& b, mpi& c)
{
	if (a.sign == b.sign)
	{
		c.sign = a.sign;
		abs_add(a, b, c);
	}
	else
	{
		if (abs_cmp(a, b) == ord::lt)
		{
			c.sign = b.sign;
			abs_sub(b, a, c);
		}
		else
		{
			c.sign = a.sign;
			abs_sub(a, b, c);
		}
	}
}

MPL_END