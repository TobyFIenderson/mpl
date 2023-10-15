#include "mpi.h"
MPL_BEGIN
uint64_t mpi::get_mag_64() const
{
	size_t i = std::min(used, (sizeof(uint64_t) * CHAR_BIT + MPI_DIGIT_BIT - 1) / MPI_DIGIT_BIT);
	uint64_t res = 0;
	while (i-- > 0u)
	{
		if constexpr (sizeof(uint64_t) * CHAR_BIT > MPI_DIGIT_BIT)
		{
			res <<= MPI_DIGIT_BIT;
		}
		res |= uint64_t(dp[i]);
		if constexpr (sizeof(uint64_t) * CHAR_BIT <= MPI_DIGIT_BIT)
		{
			break;
		}
	}
	return res;
}
MPL_END