#include "mpf.h"
MPL_BEGIN
void mpf::sqr(mpf const& a, mpf& c)
{
	mpi::sqr(a.mant, c.mant);
	c.exp = a.exp << 1;
	c.trimit();
}
MPL_END