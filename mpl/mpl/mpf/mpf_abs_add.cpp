#include "mpf.h"
MPL_BEGIN
void mpf::abs_add(mpf const& a, mpf const& b, mpf& c)
{
	const mpf* maxexp, * minexp;
	if (a.exp < b.exp)
	{
		maxexp = &b;
		minexp = &a;
	}
	else
	{
		maxexp = &a;
		minexp = &b;
	}

	size_t diff = static_cast<size_t>(maxexp->exp - minexp->exp);
	size_t mu = std::max(int64_t(maxexp->mant.used + diff) - MPF_PRECISION, 0i64);

	if (mu >= minexp->mant.used)
	{
		c = *maxexp;
		return;
	}

	mpf r = mpi::create_size(std::min(
		std::max(maxexp->mant.used + diff, minexp->mant.used),
		size_t(MPF_PRECISION)) + 1);
	r.exp = minexp->exp;

	size_t mintmp = std::min(diff, minexp->mant.used);
	mpi_digit* tmpr = r.mant.dp;
	const mpi_digit* tmpi = minexp->mant.dp + mu;
	for (size_t i = mu; i < mintmp; i++)
	{
		*tmpr++ = *tmpi++;
	}

	mpi_digit u = 0;
	const mpi_digit* tmpa = maxexp->mant.dp;
	tmpr = r.mant.dp + diff - mu;
	size_t i = 0;
	if (minexp->mant.used > diff)
	{
		mintmp = std::min(maxexp->mant.used, minexp->mant.used - diff);
		for (i = 0; i < mintmp; i++)
		{
			*tmpr = u + *tmpa++ + *tmpi++;
			u = *tmpr >> MPI_DIGIT_BIT;
			*tmpr++ &= MPI_MASK;
		}
	}

	if (i == maxexp->mant.used)
	{
		mintmp = minexp->mant.used - diff;
		while (i++ < mintmp)
		{
			*tmpr = u + *tmpi++;
			u = *tmpr >> MPI_DIGIT_BIT;
			*tmpr++ &= MPI_MASK;
		}
	}
	else
	{
		while (i++ < maxexp->mant.used)
		{
			*tmpr = u + *tmpa++;
			u = *tmpr >> MPI_DIGIT_BIT;
			*tmpr++ &= MPI_MASK;
		}
	}
	*tmpr = u;
	r.mant.used = r.mant.alloc;
	r.mant.clamp();
	r.trimit();
	r.exp += int32_t(mu);
	r.swap(c);
}
MPL_END