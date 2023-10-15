#include "mpi.h"
MPL_BEGIN
void mpi::incr()
{
	if (is_zero())
	{
		set_d(1);
	}
	else if (sign == -1 && dp[0] >= 1)
	{
		dp[0]--;
		clamp();
	}
	else if (sign == 1 && dp[0] < MPI_MASK)
	{
		dp[0]++;
	}
	else
	{
		add_d(*this, 1, *this);
	}
}
MPL_END