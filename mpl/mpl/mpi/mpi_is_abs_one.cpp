#include "mpi.h"
MPL_BEGIN
bool mpi::is_abs_one() const
{
	return used == 1 && dp[0] == 1;
}
MPL_END