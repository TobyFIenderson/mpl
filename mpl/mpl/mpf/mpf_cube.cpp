#include "mpf.h"
MPL_BEGIN
void mpf::cube(mpf const& a, mpf& c)
{
	c = a;
	mpf b = a;
	mpf::sqr(c, c);
	mpf::mul(c, b, c);
}
MPL_END