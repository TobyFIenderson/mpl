#include "mpi.h"
MPL_BEGIN

void mpi::div(mpi const& a, mpi const& b, mpi* c, mpi* d)
{
	if (b.is_zero())
	{
		throw MPL_ERR_DIV_BY_ZERO;
	}
	int32_t sign = a.sign * b.sign;

	ord abcmp = abs_cmp(a, b);
	if (abcmp == ord::eq)
	{
		if (c != nullptr)
		{
			c->set_d(1);
			c->sign = a.sign * b.sign;
		}
		if (d != nullptr)
		{
			d->set_d(0);
		}
		return;
	}
	else if (abcmp == ord::lt)
	{
		if (c != nullptr)
		{
			c->set_d(0);
		}
		if (d != nullptr)
		{
			*d = a;
		}
		return;
	}

	mpi q = create_size(a.used + 2);
	q.used = a.used + 2;
	mpi t1, t2;
	mpi x = a, y = b;
	x.sign = y.sign = 1;

	/* 归一化 */
	uint64_t norm = y.cnt_bits() % MPI_DIGIT_BIT;
	if (norm < MPI_DIGIT_BIT - 1)
	{
		norm = MPI_DIGIT_BIT - 1 - norm;
		x.lsh(norm);
		y.lsh(norm);
	}
	else
	{
		norm = 0;
	}

	size_t n = x.used - 1;
	size_t t = y.used - 1;

	/* 由于保证了 x > y 所以 n >= t */
	y.lsh_x(n - t);

	while (cmp(x, y) != ord::lt)
	{
		++(q.dp[n - t]);
		sub(x, y, x);
	}

	y.rsh_x(n - t);

	for (size_t i = n; i >= t + 1; i--)
	{
		if (i >= x.used)
		{
			continue;
		}
		if (x.dp[i] == y.dp[t])
		{
			q.dp[i - t - 1] = MPI_MASK;
		}
		else
		{
			mpi_word tmp;
			tmp = mpi_word(x.dp[i]) << mpi_word(MPI_DIGIT_BIT);
			tmp |= mpi_word(x.dp[i - 1]);
			tmp /= mpi_word(y.dp[t]);
			if (tmp > mpi_word(MPI_MASK))
			{
				tmp = MPI_MASK;
			}
			q.dp[i - t - 1] = mpi_digit(tmp & mpi_word(MPI_MASK));
		}

		q.dp[i - t - 1] = (q.dp[i - t - 1] + 1) & MPI_MASK; /* 为了保证第一次循环其值不变 */
		do
		{
			q.dp[i - t - 1] = (q.dp[i - t - 1] - 1) & MPI_MASK;

			t1.set_d(0);
			t1.dp[0] = t < 1 ? 0 : y.dp[t - 1];
			t1.dp[1] = y.dp[t];
			t1.used = 2;
			mul_d(t1, q.dp[i - t - 1], t1);

			t2.dp[0] = i < 2 ? 0 : x.dp[i - 2];
			t2.dp[1] = x.dp[i - 1];
			t2.dp[2] = x.dp[i];
			t2.used = 3;
		} while (abs_cmp(t1, t2) == ord::gt);

		mul_d(y, q.dp[i - t - 1], t1);
		t1.lsh_x(i - t - 1);
		sub(x, t1, x);

		if (x.sign == -1)
		{
			t1 = y;
			t1.lsh_x(i - t - 1);
			add(x, t1, x);
			q.dp[i - t - 1] = (q.dp[i - t - 1] - 1) & MPI_MASK;
		}

	}

	x.sign = x.used == 0 ? 1 : a.sign;

	if (c != nullptr)
	{
		q.clamp();
		c->swap(q);
		c->sign = sign;
	}
	if (d != nullptr)
	{
		x.rsh(norm);
		d->swap(x);
	}
}

MPL_END