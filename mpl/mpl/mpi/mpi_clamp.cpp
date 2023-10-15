#include "mpi.h"
MPL_BEGIN

void mpi::clamp()
{
	while (used > 0 && dp[used - 1] == 0)
	{
		used--;
	}

	if (used == 0)
	{
		sign = 1;
	}
}

MPL_END