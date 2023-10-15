#include "mpf.h"
MPL_BEGIN
void mpf::clamp_tail()
{
	uint32_t x = cnt_tail_zero();
	mant.rsh_x(x);
	if (mant.used == 0)
	{
		exp = 0;
	}
	else
	{
		exp += int32_t(x);
	}
}
MPL_END