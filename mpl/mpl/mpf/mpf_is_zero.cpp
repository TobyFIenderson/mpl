#include "mpf.h"
MPL_BEGIN
bool mpf::is_zero() const
{
	return mant.used == 0;
}
MPL_END