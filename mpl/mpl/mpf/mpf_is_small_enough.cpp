#include "mpf.h"
MPL_BEGIN
bool mpf::is_small_enough() const
{
	return is_zero() || -rdig() > MPF_PRECISION;
}
MPL_END