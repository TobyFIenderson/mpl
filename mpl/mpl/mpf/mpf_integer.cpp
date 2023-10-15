#include "mpf.h"
MPL_BEGIN
mpf mpf::integer() const
{
	if (exp >= 0)
	{
		return *this;
	}
	int32_t rdigit = rdig();
	if (rdigit > 0)
	{
		mpi _mant = mpi::create_size(size_t(rdigit));
		const mpi_digit* mdp = mant.dp + mant.used - rdigit;
		mpi_digit* _mdp = _mant.dp;
		for (int32_t i = 0; i < rdigit; i++)
		{
			*_mdp++ = *mdp++;
		}
		_mant.sign = mant.sign;
		_mant.used = size_t(rdigit);
		return mpf(std::move(_mant), 0);
	}
	return mpf(0);
}
MPL_END