#include "mpi.h"
MPL_BEGIN

void mpi::baseline_sqr(mpi const& a, mpi& b)
{
	size_t pa = a.used;
	mpi t = create_size(2 * pa + 1);
	t.used = 2 * pa + 1;

	size_t i = 0, j = 0;
	mpi_word r = 0;
	mpi_digit u = 0, tmpx = 0, * tmpt = nullptr;
	for (i = 0; i < pa; i++)
	{
		r = mpi_word(t.dp[i + i]) + mpi_word(a.dp[i]) * mpi_word(a.dp[i]);
		t.dp[i + i] = mpi_digit(r & mpi_word(MPI_MASK));
		u = mpi_digit(r >> mpi_word(MPI_DIGIT_BIT));
		tmpx = a.dp[i];
		tmpt = t.dp + (i + i + 1);
		for (j = i + 1; j < pa; j++)
		{
			r = mpi_word(tmpx) * mpi_word(a.dp[j]);
			r = r + r + mpi_word(*tmpt) + mpi_word(u);
			*tmpt++ = mpi_digit(r & mpi_word(MPI_MASK));
			u = mpi_digit(r >> mpi_word(MPI_DIGIT_BIT));
		}
		while (u != 0)
		{
			r = mpi_word(*tmpt) + mpi_word(u);
			*tmpt++ = mpi_digit(r & mpi_word(MPI_MASK));
			u = mpi_digit(r >> mpi_word(MPI_DIGIT_BIT));
		}
	}
	t.clamp();
	t.swap(b);
}

MPL_END