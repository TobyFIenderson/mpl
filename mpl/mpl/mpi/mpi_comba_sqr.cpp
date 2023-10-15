#include "mpi.h"
MPL_BEGIN

void mpi::comba_sqr(mpi const& a, mpi& b)
{
	mpi_digit W[MPI_COMBA_ARRAY]{};

	size_t pa = a.used + a.used;
	b.grow(pa);
	mpi_word u = 0;
	const mpi_digit* tmpx = nullptr, *tmpy = nullptr;
	size_t i = 0, j = 0;
	for (i = 0; i < pa; i++)
	{
		mpi_word w = 0;
		size_t ty = std::min(a.used - 1, i);
		size_t tx = i - ty;

		tmpx = a.dp + tx;
		tmpy = a.dp + ty;

		size_t k = std::min(a.used - tx, ty + 1);
		k = std::min(k, (ty - tx + 1) >> 1);
		for (j = 0; j < k; j++)
		{
			w += mpi_word(*tmpx++) * mpi_word(*tmpy--);
		}
		w = w + w + u;
		if ((i & 1) == 0u)
		{
			w += mpi_word(a.dp[i >> 1]) * mpi_word(a.dp[i >> 1]);
		}
		W[i] = mpi_digit(w) & MPI_MASK;
		u = w >> mpi_word(MPI_DIGIT_BIT);
	}

	size_t olduse = b.used;
	b.used = a.used + a.used;
	mpi_digit* tmpb = b.dp;
	for (i = 0; i < pa; i++)
	{
		*tmpb++ = W[i] & MPI_MASK;
	}
	MPI_ZERO_DIGITS(tmpb, olduse - i);
	b.clamp();
	b.sign = 1;
}

MPL_END