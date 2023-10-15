#include "mpf.h"
MPL_BEGIN
ord mpf::abs_cmp(mpf const& a, mpf const& b)
{
	if (a.rdig() > b.rdig())
	{
		return ord::gt;
	}
	if (a.rdig() < b.rdig())
	{
		return ord::lt;
	}

	const mpi_digit* tmpa = a.mant.dp + a.mant.used - 1;
	const mpi_digit* tmpb = b.mant.dp + b.mant.used - 1;
	size_t realdigit = std::max(a.mant.used, b.mant.used), i = 0;
	mpi_digit ab, bb;
	while (realdigit--)
	{
		ab = i < a.mant.used ? *tmpa-- : 0;
		bb = i < b.mant.used ? *tmpb-- : 0;
		if (ab > bb)
		{
			return ord::gt;
		}
		if (ab < bb)
		{
			return ord::lt;
		}
		i++;
	}
	return ord::eq;
}
MPL_END