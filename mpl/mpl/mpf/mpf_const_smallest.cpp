#include "mpf.h"
MPL_BEGIN
static mpf smallest = 1;
mpf const& mpf::const_smallest()
{
	smallest.exp = -MPF_ORIGINAL_PRECISION;
	return smallest;
}
MPL_END