#include "mpf_radix.h"
MPL_BEGIN
void mpf_radix::abs_add(mpf_radix const& num,int32_t precision)
{
	if (radix != num.radix)
	{
		throw MPL_ERR_INVALID_VAL;
	}
	const mpf_radix* minexp, * maxexp;
	if (exp < num.exp)
	{
		minexp = this;
		maxexp = &num;
	}
	else
	{
		minexp = &num;
		maxexp = this;
	}

	mpf_radix _maxexp = *maxexp;
	_maxexp.mant.insert(_maxexp.mant.begin(), size_t(maxexp->exp - minexp->exp), 0);
	std::vector<mpi_digit> ans(std::max(_maxexp.mant.size(), minexp->mant.size()) + 1, 0);
	mpi_digit u = 0;
	size_t i = 0;
	for (; i < _maxexp.mant.size() || i < minexp->mant.size(); i++)
	{
		mpi_digit j = i < _maxexp.mant.size() ? _maxexp.mant[i] : 0;
		mpi_digit k = i < minexp->mant.size() ? minexp->mant[i] : 0;
		ans[i] = j + k + u;
		u = ans[i] / radix;
		ans[i] %= radix;
	}
	if (u != 0)
	{
		ans[i] = u;
	}
	exp = minexp->exp;
	mant = ans;
	trimit(precision);
	clamp();
}
MPL_END