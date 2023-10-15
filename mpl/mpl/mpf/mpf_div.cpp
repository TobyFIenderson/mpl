#include "mpf.h"
MPL_BEGIN
void mpf::div(mpf const& a, mpf const& b, mpf& c)
{
	mpi d = a.mant, e = b.mant, r, q;
	e.sign = d.sign = 1;
	int32_t sign = a.mant.sign * b.mant.sign;
	int32_t exp = 1;
	int32_t aexp = a.exp;
	int32_t bexp = b.exp;
	c.set_d(0);
	while (c.mant.used <= MPF_PRECISION)
	{
		mpi::div(d, e, &q, &r);
		c.mant.lsh_x(1);
		mpi::add(c.mant, q, c.mant);
		if (r.used == 0)
		{
			exp--;
			break;
		}
		r.lsh_x(1);
		d.swap(r);
		exp--;
	}
	c.mant.sign = sign;
	c.exp = aexp - bexp + exp;
	c.trimit();
}
MPL_END