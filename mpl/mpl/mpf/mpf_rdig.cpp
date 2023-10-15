#include "mpf.h"
MPL_BEGIN
int32_t mpf::rdig() const
{
	return mant.used == 0 ? 0 : int32_t(mant.used) + exp;
}
MPL_END