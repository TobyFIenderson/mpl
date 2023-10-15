#include "mpf_radix.h"
MPL_BEGIN

void mpf_radix::add_1()
{
	mpi_digit u = 0;
	mant[0]++;
	if (mant[0] >= radix)
	{
		for (size_t i = 0; i < mant.size(); i++)
		{
			mant[i] = mant[i] + u;
			u = mant[i] / radix;
			mant[i] %= radix;
		}
	}
	if (u != 0)
	{
		mant.push_back(u);
	}
}

MPL_END