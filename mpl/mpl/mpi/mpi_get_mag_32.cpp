#include "mpi.h"
MPL_BEGIN
uint32_t mpi::get_mag_32() const
{
	size_t i = std::min(used, (sizeof(uint32_t) * CHAR_BIT + MPI_DIGIT_BIT - 1) / MPI_DIGIT_BIT);
	uint32_t res = 0;
	while (i-- > 0u)
	{
		if constexpr (sizeof(uint32_t) * CHAR_BIT > MPI_DIGIT_BIT)
		{
			res <<= MPI_DIGIT_BIT;
		}
		res |= uint32_t(dp[i]);
		if constexpr (sizeof(uint32_t) * CHAR_BIT <= MPI_DIGIT_BIT)
		{
			break;
		}
	}
	return res;
}
MPL_END