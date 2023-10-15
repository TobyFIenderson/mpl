#include "mpf.h"
MPL_BEGIN
mpf mpf::fraction() const
{
	if (exp >= 0)
	{
		return mpf(0);
	}
	mpi _mant = mpi::create_size(size_t(-exp));
	const mpi_digit* mdp = mant.dp;
	mpi_digit* _mdp = _mant.dp;
	int32_t i;
	for (i = 0; i < -exp && i < int32_t(mant.used); i++)
	{
		*_mdp++ = *mdp++;
	}
	_mant.used = size_t(i);
	_mant.sign = mant.sign;
	return mpf(std::move(_mant), exp);
}
MPL_END