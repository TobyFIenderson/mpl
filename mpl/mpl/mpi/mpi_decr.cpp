#include "mpi.h"
MPL_BEGIN
void mpi::decr()
{
	if (is_zero())
	{
		set_d(1);
		sign = -1;
	}
	else if (sign == -1 && dp[0] < MPI_MASK)
	{
		dp[0]++;
	}
	else if (sign == 1 && dp[0] >= 1)
	{
		dp[0]--;
		clamp();
	}
	else
	{
		sub_d(*this, 1, *this);
	}
}
MPL_END