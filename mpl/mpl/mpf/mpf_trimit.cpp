#include "mpf.h"
MPL_BEGIN
void mpf::trimit()
{
	if (MPF_ALLOW_INFINITE_PRECISION)
	{
		return;
	}
	if (mant.used <= MPF_PRECISION)
	{

		if (mant.used == 0)
		{
			exp = 0;
		}
		return;
	}

	uint64_t diff = mant.used - uint64_t(MPF_PRECISION);
	exp += int32_t(diff);
	mant.rsh_x(diff);
	if (mant.used == 0)
	{
		exp = 0;
	}
}
MPL_END