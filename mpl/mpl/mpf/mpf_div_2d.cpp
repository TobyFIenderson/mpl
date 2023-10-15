#include "mpf.h"
MPL_BEGIN
void mpf::div_2d(mpf const& a, mpi_digit d, mpf& c)
{
	if (a.is_zero())
	{
		c.set_d(0);
		return;
	}
	if (d == 1)
	{
		mpf::div_2(a, c);
		return;
	}
	c = a;
	c.exp -= d / MPI_DIGIT_BIT;
	d %= MPI_DIGIT_BIT;
	if (d != 0)
	{
		c.mant.lsh_x(1);
		c.exp--;
		c.mant.rsh(d);
		c.trimit();
	}
}
MPL_END