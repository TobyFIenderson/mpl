#include "mpi.h"
MPL_BEGIN
void mpi::sub_d(mpi const& a, mpi_digit b, mpi& c)
{
	c.grow(a.used + 1);
	if (a.is_neg())
	{
		/* 这里可以使用浅拷贝优化 */
		mpi _a(1, a.alloc, a.used, a.dp);
		add_d(_a, b, c);
		c.sign = -1;
		c.clamp();
		_a.dp = nullptr;
		return;
	}

	size_t olduse = c.used;
	const mpi_digit* tmpa = a.dp;
	mpi_digit* tmpc = c.dp;
	size_t i = 0;

	if ((a.used == 1 && a.dp[0] <= b) || a.is_zero())
	{
		*tmpc++ = a.used == 1 ? b - *tmpa : b;
		i = 1;
		c.sign = -1;
		c.used = 1;
	}
	else
	{
		mpi_digit _c = b;
		for (i = 0; i < a.used; i++)
		{
			*tmpc = *tmpa++ - _c;
			_c = *tmpc >> (sizeof(mpi_digit) * CHAR_BIT - 1);
			*tmpc++ &= MPI_MASK;
		}
		c.sign = 1;
		c.used = a.used;
	}

	MPI_ZERO_DIGITS(tmpc, olduse - i);
	c.clamp();
}
MPL_END