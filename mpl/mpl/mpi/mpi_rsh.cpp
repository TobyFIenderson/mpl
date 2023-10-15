#include "mpi.h"
MPL_BEGIN

void mpi::rsh(uint64_t d)
{
	if (d == 0 || is_zero())
	{
		return;
	}

	rsh_x(d / MPI_DIGIT_BIT);
	d = d % MPI_DIGIT_BIT;
	if (d != 0)
	{
		mpi_digit mask = (1 << d) - 1;
		mpi_digit shift = mpi_digit(MPI_DIGIT_BIT - d);
		mpi_digit* tmpc = dp + used - 1;
		mpi_digit r = 0, rr = 0;
		for (size_t i = used; i > 0; i--)/* һ����� for (size_t i = used - 1; i >= 0; i--) ������������ C6295 ���� */
		{
			rr = *tmpc & mask;
			*tmpc = (*tmpc >> d) | (r << shift);
			--tmpc;
			r = rr;
		}
	}
	clamp();
}

MPL_END