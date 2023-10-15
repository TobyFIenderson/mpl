#include "mpf.h"
MPL_BEGIN
bool mpf::is_abs_one() const
{
	if (rdig() == 1 && mant.dp[mant.used - 1] == 1)
	{
		uint32_t x = cnt_tail_zero();
		if (x + exp == 0)
		{
			return true;
		}
	}

	return false;
}
MPL_END