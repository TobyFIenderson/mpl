#include "mpi.h"
MPL_BEGIN
void mpi::lsh(uint64_t d)
{
	if (d == 0 || is_zero())
	{
		return;
	}

	grow(used + d / MPI_DIGIT_BIT + 1);
	lsh_x(d / MPI_DIGIT_BIT);

	d = d % MPI_DIGIT_BIT;
	if (d != 0)
	{
		mpi_digit mask = mpi_digit(1 << d) - mpi_digit(1);
		mpi_digit shift = mpi_digit(MPI_DIGIT_BIT - d);
		mpi_digit* tmpc = dp;
		mpi_digit r = 0, rr = 0;
		for (size_t i = 0; i < used; i++)
		{
			rr = (*tmpc >> shift) & mask;
			*tmpc = ((*tmpc << d) | r) & MPI_MASK;
			++tmpc;
			r = rr;
		}

		if (r != 0)
		{
			dp[used++] = r;
		}
	}
	clamp();
}
MPL_END