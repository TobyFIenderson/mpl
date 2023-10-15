#include "mpi.h"
MPL_BEGIN

void mpi::div_d(mpi const& a, mpi_digit b, mpi* c, mpi_digit* d)
{
	if (b == 0)
	{
		throw MPL_ERR_DIV_BY_ZERO;
	}

	if (b == 1 || a.is_zero())
	{
		if (d != nullptr)
		{
			*d = 0;
		}
		if (c != nullptr)
		{
			*c = a;
		}
		return;
	}

	int32_t i = 0;
	if ((b & (b - 1)) == 0)
	{
		i = 1;
		while (i < MPI_DIGIT_BIT && b != mpi_digit(1 << i))
		{
			i++;
		}

		if (d != nullptr)
		{
			*d = a.dp[0] & mpi_digit((1 << i) - 1);
		}
		if (c != nullptr)
		{
			*c = a;
			c->rsh(i);
		}
		return;
	}

	if (b == 3)
	{
		div_3(a, c, d);
	}

	mpi q = create_size(a.used);
	q.used = a.used;
	q.sign = a.sign;
	mpi_word w = 0;
	mpi_digit t = 0;
	for (i = int32_t(a.used - 1); i >= 0; i--)
	{
		w = (w << mpi_word(MPI_DIGIT_BIT)) | mpi_word(a.dp[i]);
		if (w >= b)
		{
			t = mpi_digit(w / b);
			w -= mpi_word(t) * mpi_word(b);
		}
		else
		{
			t = 0;
		}
		q.dp[i] = t;
	}

	if (d != nullptr)
	{
		*d = mpi_digit(w);
	}
	if (c != nullptr)
	{
		q.clamp();
		q.swap(*c);
	}
}

MPL_END