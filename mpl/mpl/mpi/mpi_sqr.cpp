#include "mpi.h"
MPL_BEGIN

void mpi::sqr(mpi const& a, mpi& b)
{
	if (a.is_zero())
	{
		b.set_d(0);
		return;
	}
	if (a.is_abs_one())
	{
		b.set_d(1);
		return;
	}


	if (a.used >= MPI_TOOM_SQR_CUTOFF)
	{
		toom_cook_sqr(a, b);
	}
	else if (a.used >= MPI_KARA_SQR_CUTOFF)
	{
		karatsuba_sqr(a, b);
	}
	else if (a.used <= MPI_MAX_COMBA)
	{
		comba_sqr(a, b);
	}
	else
	{
		baseline_sqr(a, b);
	}
	b.sign = 1;
}

MPL_END