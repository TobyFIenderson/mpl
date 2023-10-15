#include "mpi.h"
MPL_BEGIN

void mpi::set_d(mpi_digit d)
{
	dp[0] = d & MPI_MASK;
	sign = 1;
	size_t olduse = used;
	used = dp[0] == 0u ? 0u : 1u;
	MPI_ZERO_DIGITS(dp + used, olduse - used);
}

MPL_END