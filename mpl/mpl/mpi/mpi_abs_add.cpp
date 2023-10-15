#include "mpi.h"
MPL_BEGIN

void mpi::abs_add(mpi const& a, mpi const& b, mpi& c)
{
	size_t minuse, maxuse;
	const mpi* x = nullptr;
	if (a.used > b.used)
	{
		minuse = b.used;
		maxuse = a.used;
		x = &a;
	}
	else
	{
		minuse = a.used;
		maxuse = b.used;
		x = &b;
	}

	c.grow(maxuse + 1); /* 注意 c.grow() 会修改 a.dp 或者 b.dp 的指向！！！*/

	const mpi_digit* tmpa = a.dp;
	const mpi_digit* tmpb = b.dp;
	mpi_digit* tmpc = c.dp;
	mpi_digit carry = 0;

	size_t i = 0;
	for (; i < minuse; i++)
	{
		*tmpc = *tmpa++ + *tmpb++ + carry;
		carry = *tmpc >> MPI_DIGIT_BIT;
		*tmpc++ &= MPI_MASK;
	}

	if (minuse != maxuse)
	{
		const mpi_digit* tmpx = x->dp + minuse;
		for (; i < maxuse; i++)
		{
			*tmpc = *tmpx++ + carry;
			carry = *tmpc >> MPI_DIGIT_BIT;
			*tmpc++ &= MPI_MASK;
		}
	}
	*tmpc++ = carry;

	size_t olduse = c.used;
	c.used = maxuse + 1;

	MPI_ZERO_DIGITS(tmpc, olduse - c.used);
	c.clamp();
}

MPL_END