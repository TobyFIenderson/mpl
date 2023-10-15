#include "mpf.h"
MPL_BEGIN
void mpf::div_d(mpf const& a, mpi_digit b, mpf& c)
{
	if (b == 0)
	{
		throw MPL_ERR_DIV_BY_ZERO;
		return;
	}

	if ((b & (b - 1)) == 0)
	{
		int32_t i = 1;
		while (i < MPI_DIGIT_BIT && b != mpi_digit(1 << i))
		{
			i++;
		}
		mpf::div_2d(a, i, c);
		return;
	}
	if (b == 3)
	{
		mpf::div_3(a, c);
		return;
	}
	int32_t sign = a.mant.sign;
	int32_t aexp = a.exp, exp = 1;
	mpi d = a.mant, q;
	d.sign = 1;
	mpi_digit r = 0;
	c.set_d(0);

	while (c.mant.used <= MPF_PRECISION)
	{
		mpi::div_d(d, b, &q, &r);
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