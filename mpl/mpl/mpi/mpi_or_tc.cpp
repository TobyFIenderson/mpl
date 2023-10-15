#include "mpi.h"
MPL_BEGIN

void mpi::or_tc(mpi const& a, mpi const& b, mpi& c)
{
	size_t used = std::max(a.used, b.used) + 1;
	mpi_digit ac = 1, bc = 1, cc = 1;
	int32_t sign = (a.sign == -1 || b.sign == -1) ? -1 : 1;

	c.grow(used);
	mpi_digit x, y;
	for (size_t i = 0; i < used; i++)
	{
		if (a.sign == -1)
		{
			ac += i >= a.used ? MPI_MASK : (~a.dp[i] & MPI_MASK);
			x = ac & MPI_MASK;
			ac >>= MPI_DIGIT_BIT;
		}
		else
		{
			x = i >= a.used ? 0 : a.dp[i];
		}

		if (b.sign == -1)
		{
			bc += i >= b.used ? MPI_MASK : (~b.dp[i] & MPI_MASK);
			y = bc & MPI_MASK;
			bc >>= MPI_DIGIT_BIT;
		}
		else
		{
			y = i >= b.used ? 0 : b.dp[i];
		}

		c.dp[i] = x | y;

		if (sign == -1)
		{
			cc += ~c.dp[i] & MPI_MASK;
			c.dp[i] = cc & MPI_MASK;
			cc >>= MPI_DIGIT_BIT;
		}
	}

	c.used = used;
	c.sign = sign;
	c.clamp();
}

MPL_END