#include "mpf.h"
MPL_BEGIN
void mpf::mul(mpf const& a, mpf const& b, mpf& c)
{
	mpi::mul(a.mant, b.mant, c.mant);
	c.exp = a.exp + b.exp;
	c.trimit();
}
MPL_END