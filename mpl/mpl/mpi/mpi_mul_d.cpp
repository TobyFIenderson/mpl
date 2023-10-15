#include "mpi.h"
MPL_BEGIN

void mpi::mul_d(mpi const& a, mpi_digit b, mpi& c)
{
	c.grow(a.used + 1);

	mpi_word r = 0;
	mpi_digit u = 0;
	const mpi_digit* tmpa = a.dp;
	mpi_digit* tmpc = c.dp;

	size_t olduse = c.used;
	c.sign = a.sign;

	for (size_t i = 0; i < a.used; i++)
	{
		r = mpi_word(u) + mpi_word(*tmpa++) * mpi_word(b);
		*tmpc++ = mpi_digit(r & mpi_word(MPI_MASK));
		u = mpi_digit(r >> mpi_word(MPI_DIGIT_BIT));
	}

	*tmpc++ = u;
	MPI_ZERO_DIGITS(tmpc, olduse - a.used - 1);
	c.used = a.used + 1;
	c.clamp();
}

MPL_END