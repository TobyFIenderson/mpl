#include "mpi.h"
MPL_BEGIN

void mpi::lsh_x(uint64_t d)
{
	if (d == 0 || is_zero())
	{
		return;
	}

	grow(used + d);
	used += d;

	mpi_digit* top = dp + used - 1;
	const mpi_digit* bot = dp + used - 1 - d; /* used >= d + 1，所以这句不会越界 */

	for (size_t i = used - 1; i >= d; i--)
	{
		*top-- = *bot--;
	}

	MPI_ZERO_DIGITS(dp, d);
}

MPL_END