#include "mpf.h"
MPL_BEGIN
bool mpf::is_neg() const
{
	return mant.sign == -1;
}
void mpf::is_neg(bool isneg)
{
	mant.sign = isneg ? -1 : 1;
}
MPL_END