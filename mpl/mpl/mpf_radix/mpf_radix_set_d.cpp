#include "mpf_radix.h"
MPL_BEGIN
void mpf_radix::set_d(uint32_t d)
{
	exp = 0;
	sign = 1;
	mant.clear();
	while (d > 0)
	{
		mant.push_back(d % radix);
		d /= radix;
	}
}
MPL_END