#include "mpi.h"
MPL_BEGIN

void mpi::rsh_x(uint64_t d)
{
	if (d == 0)
	{
		return;
	}
	/* �������� this Ϊ 0 ����� */
	if (used <= d)
	{
		set_d(0);
		return;
	}

	mpi_digit* bot = dp;
	const mpi_digit* top = dp + d;

	for (size_t i = 0; i < used - d; i++)
	{
		*bot++ = *top++;
	}

	MPI_ZERO_DIGITS(bot, d);
	used -= d;
}

MPL_END