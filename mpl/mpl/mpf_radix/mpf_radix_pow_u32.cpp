#include "mpf_radix.h"
MPL_BEGIN
void mpf_radix::pow_u32(uint32_t pow, int32_t precision, double_complex* a, int* rev)
{
	if (pow == 1)
	{
		return;
	}
	mpf_radix base = *this;
	mpf_radix ret{ 1,{1},radix,0 };
	while (pow)
	{
		if ((pow & 1) != 0u)
		{
			ret.mul(base, precision, a, rev);
		}
		if (pow > 1u)
		{
			base.mul(base, precision, a, rev);
		}
		pow >>= 1;

	}
	*this = std::move(ret);
}
MPL_END