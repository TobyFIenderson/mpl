#include "mpf.h"
MPL_BEGIN
void mpf::tanh(mpf const& a, mpf& c)
{
	mpf b;
	mpf::sinh(a, b);
	mpf::cosh(a, c);
	mpf::div(b, c, c);
}
MPL_END