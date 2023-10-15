#include "mpi.h"
MPL_BEGIN
void mpi::add_d(mpi const& a, mpi_digit b, mpi& c)
{
	c.grow(a.used + 1);
	/* 如果 a 为负数且 |a| >= b ，那么有 c = -(|a| - b) */
	if (a.is_neg() && (a.used > 1 || a.dp[0] >= b))
	{
		mpi _a(1, a.alloc, a.used, a.dp);
		sub_d(_a, b, c);
		c.sign = -1;
		c.clamp();
		_a.dp = nullptr;
		return;
	}

	size_t olduse = c.used;
	const mpi_digit* tmpa = a.dp;
	mpi_digit* tmpc = c.dp;

	size_t i = 0;
	/* a 为负数且 |a| < b */
	if (a.is_neg())
	{
		c.used = 1;
		*tmpc++ = a.used == 1 ? b - a.dp[0] : b;
		i = 1;
	}
	else
	{
		mpi_digit _c = b;
		for (i = 0; i < a.used; i++)
		{
			*tmpc = *tmpa++ + _c;
			_c = *tmpc >> MPI_DIGIT_BIT;
			*tmpc++ &= MPI_MASK;
		}
		i++;
		*tmpc++ = _c;
		c.used = a.used + 1;
	}/* 这两种情况结果均为正数 */

	c.sign = 1;

	MPI_ZERO_DIGITS(tmpc, olduse - i);
	c.clamp();
}
MPL_END