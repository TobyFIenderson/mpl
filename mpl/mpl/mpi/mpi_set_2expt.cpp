#include "mpi.h"
MPL_BEGIN
void mpi::set_2expt(uint32_t d)
{
	set_d(0);
	grow(size_t(d / MPI_DIGIT_BIT) + 1);
	used = size_t(d / MPI_DIGIT_BIT + 1);
	dp[d / MPI_DIGIT_BIT] = mpi_digit(1 << (d % MPI_DIGIT_BIT));
}
MPL_END