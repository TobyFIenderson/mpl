#include "mpi.h"
#include <algorithm>
MPL_BEGIN
void mpi::comba_mul(mpi const& a, mpi const& b, mpi& c)
{
	size_t pa = a.used + b.used;
	c.grow(pa);

	mpi_word carry = 0;
	mpi_digit w[MPI_COMBA_ARRAY]{};
	for (size_t i = 0; i < pa; i++)
	{
		int64_t tb = std::min<int64_t>(b.used - 1, i);
		int64_t ta = int64_t(i) - tb;

		const mpi_digit* tmpa = a.dp + ta;
		const mpi_digit* tmpb = b.dp + tb;

		int64_t j = std::min<int64_t>(a.used - ta, tb + 1);
		for (int64_t k = 0; k < j; k++)
		{
			carry += mpi_word(*tmpa++) * mpi_word(*tmpb--);
		}

		w[i] = mpi_digit(carry) & MPI_MASK;
		carry >>= mpi_word(MPI_DIGIT_BIT);
	}

	size_t olduse = c.used;
	c.used = pa;

	mpi_digit* tmpc = c.dp;
	for (size_t i = 0; i < pa; i++)
	{
		*tmpc++ = w[i];
	}

	MPI_ZERO_DIGITS(tmpc, olduse - pa);
	c.clamp();
}
MPL_END