#include "mpf.h"
MPL_BEGIN
void mpf::mod(mpf const& a, mpf const& b, mpf& c)
{
	ord o = abs_cmp(a, b);
	if (o == ord::lt)
	{
		c = a;
		return;
	}
	if (o == ord::eq)
	{
		c.set_d(0);
		return;
	}
	if (a.exp >= 0 && b.exp >= 0)
	{
		mpi::mod(a.to_mpi(), b.to_mpi(), c.mant);
		c.exp = 0;
	}
	else
	{
		int32_t minexp = std::min(a.exp, b.exp);
		mpi ad = a.mant, bd = b.mant;
		ad.lsh_x(uint64_t(a.exp - minexp));
		bd.lsh_x(uint64_t(b.exp - minexp));
		mpi::mod(ad, bd, c.mant);
		c.exp = minexp;
	}
	c.trimit();
}
MPL_END