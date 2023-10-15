#include "mpf.h"
MPL_BEGIN
void mpf::mul_d(mpf const& a, mpi_digit d, mpf& c)
{
	mpi::mul_d(a.mant, d, c.mant);
	c.trimit();
}
MPL_END