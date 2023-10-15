#include "mpf.h"
MPL_BEGIN
uint32_t mpf::cnt_tail_zero() const
{
	const mpi_digit* tmpm = mant.dp;
	uint32_t x = 0;
	for (size_t i = 0; i < mant.used; i++)
	{
		if (*tmpm++ == 0)
		{
			x++;
		}
		else
		{
			break;
		}
	}
	return x;
}
MPL_END