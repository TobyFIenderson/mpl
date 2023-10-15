#include "mpf_radix.h"
MPL_BEGIN
void mpf_radix::trimit(int32_t precision, bool round)
{
	mpi_digit last_digit = 0;
	while (mant.size() > size_t(precision))
	{
		uint64_t diff = mant.size() - size_t(precision);
		last_digit = mant[diff - 1];
		mant.erase(mant.begin(), mant.begin() + diff);
		exp += int32_t(diff);
		if (round && last_digit >= radix / 2)
		{
			add_1();
		}
	}
}
MPL_END