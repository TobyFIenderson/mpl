#include "mpi.h"
MPL_BEGIN

void mpi::sub(mpi const& a, mpi const& b, mpi& c)
{
	if (a.sign != b.sign)
	{
		c.sign = a.sign;
		abs_add(a, b, c);
	}
	else
	{
		if (abs_cmp(a, b) != ord::lt)
		{
			c.sign = a.sign;
			abs_sub(a, b, c);
		}
		else
		{
			c.sign = a.sign * -1;
			abs_sub(b, a, c);
		}
	}
}

MPL_END