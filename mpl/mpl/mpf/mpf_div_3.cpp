#include "mpf.h"
MPL_BEGIN
void mpf::div_3(mpf const& a, mpf& c)
{
	int32_t sign = a.mant.sign;
	int32_t aexp = a.exp, exp = 1;
	mpi d = a.mant, q;
	d.sign = 1;
	mpi_digit r = 0;
	c.set_d(0);

	while (c.mant.used <= MPF_PRECISION)
	{
		mpi::div_3(d, &q, &r);
		c.mant.lsh_x(1);
		mpi::add(c.mant, q, c.mant);
		if (r == 0)
		{
			exp--;
			break;
		}
		d = r;
		d.lsh_x(1);
		exp--;
	}
	c.mant.sign = sign;
	c.exp = aexp + exp;
	c.trimit();
}
MPL_END