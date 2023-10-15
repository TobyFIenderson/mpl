#include "mpi.h"
MPL_BEGIN

uint64_t mpi::cnt_bits() const
{
	if (is_zero())
	{
		return 0;
	}

	uint64_t r = (used - 1) * MPI_DIGIT_BIT;
	mpi_digit q = dp[used - 1];
	while (q > 0)
	{
		++r;
		q >>= 1;
	}
	return r;
}

MPL_END