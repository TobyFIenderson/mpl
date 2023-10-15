#include "mpi.h"
MPL_BEGIN

ord mpi::abs_cmp(mpi const& a, mpi const& b)
{

	if (a.used > b.used)
	{
		return ord::gt;
	}
	if (a.used < b.used)
	{
		return ord::lt;
	}

	const mpi_digit* tmpa = a.dp + a.used - 1;
	const mpi_digit* tmpb = b.dp + a.used - 1;

	for (size_t i = 0; i < a.used; i++, tmpa--, tmpb--)
	{
		if (*tmpa > *tmpb)
		{
			return ord::gt;
		}
		if (*tmpa < *tmpb)
		{
			return ord::lt;
		}
	}

	return ord::eq;
}

MPL_END