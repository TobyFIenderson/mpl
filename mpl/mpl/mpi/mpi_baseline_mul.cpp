#include "mpi.h"
MPL_BEGIN

void mpi::baseline_mul(mpi const& a, mpi const& b, mpi& c)
{
	mpi r = create_size(a.used + b.used);
	r.used = a.used + b.used;

	mpi_word w = 0;
	mpi_digit carry = 0, tmpa;
	const mpi_digit* tmpb;
	mpi_digit* tmpr;
	for (size_t i = 0; i < a.used; i++)
	{
		carry = 0;
		tmpr = r.dp + i;
		tmpa = a.dp[i];
		tmpb = b.dp;
		for (size_t j = 0; j < b.used; j++)
		{
			w = mpi_word(*tmpr) + mpi_word(tmpa) * mpi_word(*tmpb++) + mpi_word(carry);
			*tmpr++ = mpi_digit(w & mpi_word(MPI_MASK));
			carry = mpi_digit(w >> mpi_word(MPI_DIGIT_BIT));
		}
		*tmpr = carry;
	}

	r.clamp();
	c.swap(r);
}

MPL_END