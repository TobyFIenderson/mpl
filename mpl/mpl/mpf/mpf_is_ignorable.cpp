#include "mpf.h"
MPL_BEGIN
bool mpf::is_ignorable() const
{
	return mpf::abs_cmp(mpf::const_smallest(), *this) != ord::lt;
}
MPL_END