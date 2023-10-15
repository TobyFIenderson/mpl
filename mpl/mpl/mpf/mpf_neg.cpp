#include "mpf.h"
MPL_BEGIN
void mpf::neg(mpf const& a, mpf& c)
{
	c = a;
	c.mant.sign *= -1;
}
MPL_END