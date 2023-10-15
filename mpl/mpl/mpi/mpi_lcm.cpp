#include "mpi.h"
MPL_BEGIN

void mpi::lcm(mpi const& a, mpi const& b, mpi& c)
{
	mpi t1, t2;

	gcd(a, b, t1);
	if (abs_cmp(a, b) == ord::lt)
	{
		div(a, t1, &t2, nullptr);
		mul(b, t2, c);
	}
	else
	{
		div(b, t1, &t2, nullptr);
		mul(a, t2, c);
	}
	c.sign = 1;
}

MPL_END