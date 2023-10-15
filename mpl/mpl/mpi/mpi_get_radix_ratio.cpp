#include "mpi.h"
MPL_BEGIN
uint32_t mpi::get_radix_ratio(uint32_t radix)
{
	/* ´ò±í */
	if constexpr (MPI_DIGIT_BIT == 28)
	{
		switch (radix)
		{
		case 2: return 28;
		case 8: return 10;
		case 10: return 9;
		case 16: return 7;
		}
	}
	uint32_t ratio = 1;
	mpi_digit tmp = mpi_digit(radix);
	while (tmp < MPI_MASK)
	{
		tmp *= mpi_digit(radix);
		ratio++;
	}
	return ratio;
}
MPL_END