#include "mpi.h"
MPL_BEGIN
static constexpr int lnz[16] =
{
	4, 0, 1, 0,
	2, 0, 1, 0,
	3, 0, 1, 0,
	2, 0, 1, 0
};
uint64_t mpi::cnt_lsb() const
{

	if (is_zero())
	{
		return 0;
	}

	size_t x;
	mpi_digit q, qq = 0;
	for (x = 0; (x < used) && (dp[x] == 0u); x++);
	q = dp[x];
	x *= MPI_DIGIT_BIT;

	if ((q & 1u) == 0u) 
	{
		do 
		{
			qq = q & 0xf;
			x += lnz[qq];
			q >>= 4;
		} while (qq == 0u);
	}
	return x;
}
MPL_END