#include "mpf.h"
MPL_BEGIN
void mpf::pow_mpi(mpf const& a, mpf const& b, mpf& c)
{
	int64_t exp = b.exp;
	if (exp < 0)
	{
		throw MPL_ERR_INVALID_VAL;
	}

	mpf::k_ray_expt(a, b.mant, c);

	exp = exp * MPI_DIGIT_BIT;
	for (int64_t i = 0; i < exp; i++)
	{
		mpf::sqr(c, c);
	}

	if (b.mant.sign == -1)
	{
		mpf::inv(c, c);
	}
}
MPL_END