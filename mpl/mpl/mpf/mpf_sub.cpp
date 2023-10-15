#include "mpf.h"
MPL_BEGIN
void mpf::sub(mpf const& a, mpf const& b, mpf& c)
{
	mpf r = mpf(mpi(b.mant.sign, b.mant.alloc, b.mant.used, b.mant.dp), b.exp);
	r.mant.sign *= -1;
	mpf::add(a, r, c);
	r.mant.dp = nullptr;
}
MPL_END