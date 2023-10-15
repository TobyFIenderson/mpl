#include "mpf_radix.h"
MPL_BEGIN
void mpf_radix::clamp()
{
	while (!mant.empty() && mant.back() == 0)
	{
		mant.pop_back();
	}
	if (mant.empty())
	{
		sign = 1;
		exp = 0;
	}
}
MPL_END