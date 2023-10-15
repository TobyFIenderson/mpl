#include "mpi.h"
MPL_BEGIN

void mpi::abs_sub(mpi const& a, mpi const& b, mpi& c)
{
	c.grow(a.used);

	mpi_digit carry = 0;
	const mpi_digit* tmpa = a.dp;
	const mpi_digit* tmpb = b.dp;
	mpi_digit* tmpc = c.dp;

	size_t i = 0;
	for (; i < b.used; i++)
	{
		*tmpc = *tmpa++ - *tmpb++ - carry;
		carry = *tmpc >> mpi_digit(sizeof(mpi_digit) * CHAR_BIT - 1);
		*tmpc++ &= MPI_MASK;
	}

	for (; i < a.used; i++)
	{
		*tmpc = *tmpa++ - carry;
		carry = *tmpc >> mpi_digit(sizeof(mpi_digit) * CHAR_BIT - 1);
		*tmpc++ &= MPI_MASK;
	}

	size_t olduse = c.used;
	c.used = a.used;
	MPI_ZERO_DIGITS(tmpc, olduse - c.used);
	c.clamp();
}

MPL_END