#include "mpf.h"
MPL_BEGIN
void mpf::mul_2d(mpf const& a, mpi_digit d, mpf& c)
{
	if (a.is_zero())
	{
		c.set_d(0);
		return;
	}
	c = a;
	c.exp += d / MPI_DIGIT_BIT;
	d = d % MPI_DIGIT_BIT;
	if (d != 0)
	{
		c.mant.lsh(d);
		c.trimit();
	}
}
MPL_END