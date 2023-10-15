#include "mpi.h"
MPL_BEGIN

/* 此算法借鉴自 GMP 有点像 Barrett 缩减算法 */
void mpi::div_3(mpi const& a, mpi* c, mpi_digit * d)
{
	mpi q = create_size(a.used);
	mpi_digit b = MPI_BASE / 3;
	mpi_word w = 0, t = 0;
	q.used = a.used;
	q.sign = a.sign;
	const mpi_digit* tmpa = a.dp + a.used - 1;
	mpi_digit* tmpq = q.dp + q.used - 1;

	for (size_t i = a.used; i > 0; i--)
	{
		w = (w << mpi_word(MPI_DIGIT_BIT)) | mpi_word(*tmpa--);
		if (w >= 3)
		{
			t = (w * mpi_word(b)) >> mpi_word(MPI_DIGIT_BIT);
			w -= t + t + t;
			while (w >= 3)
			{
				t++;
				w -= 3;
			}
		}
		else
		{
			t = 0;
		}
		*tmpq-- = mpi_digit(t);
	}

	if (d != nullptr)
	{
		*d = mpi_digit(w);
	}

	if (c != nullptr)
	{
		q.clamp();
		c->swap(q);
	}
}

MPL_END