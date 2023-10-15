#include "mpf.h"
MPL_BEGIN
void mpf::sech(mpf const& a, mpf& c)
{
	mpf::cosh(a, c);
	mpf::inv(c, c);
}
MPL_END