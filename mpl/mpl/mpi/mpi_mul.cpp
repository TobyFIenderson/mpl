#include "mpi.h"
#include <algorithm>
MPL_BEGIN
void mpi::mul(mpi const& a, mpi const& b, mpi& c)
{
	int32_t sign = a.sign * b.sign;
	if (a.is_zero() || b.is_zero())
	{
		c.set_d(0);
		return;
	}
	if (a.is_abs_one())
	{
		c = b;
		c.sign = sign;
		return;
	}
	if (b.is_abs_one())
	{
		c = a;
		c.sign = sign;
		return;
	}

	size_t minuse = std::min<size_t>(a.used, b.used);
	if (minuse >= MPI_TOOM_MUL_CUTOFF)
	{
		toom_cook_mul(a, b, c);
	}
	else if (minuse >= MPI_KARA_MUL_CUTOFF)
	{
		karatsuba_mul(a, b, c);
	}
	else if (a.used + b.used <= MPI_COMBA_ARRAY && minuse <= MPI_MAX_COMBA)
	{
		comba_mul(a, b, c);
	}
	else
	{
		baseline_mul(a, b, c);
	}
	c.sign = c.used > 0 ? sign : 1;
}
MPL_END