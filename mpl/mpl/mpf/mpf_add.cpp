#include "mpf.h"
MPL_BEGIN

/*

	if (a.is_zero())
	{
		c = b;
		return;
	}
	if (b.is_zero())
	{
		c = a;
		return;
	}

	const mpf* maxexp, * minexp;
	if (a.exp > b.exp)
	{
		maxexp = &a;
		minexp = &b;
	}
	else
	{
		maxexp = &b;
		minexp = &a;
	}

	// 如果是 4 prec 考虑 1234e+1 + 1 --> 12340 + 1 --> 12341 --> 1234e+1
	// 前后没有变化就不用计算了
	if (a.mant.sign == b.mant.sign)
	{
		int32_t diff = maxexp->exp - minexp->exp;
		int32_t willrshd = int32_t(maxexp->mant.used) + diff - MPF_PRECISION;
		if (willrshd >= int32_t(minexp->mant.used))
		{
			c = *maxexp;
			return;
		}
	}
	mpi r = maxexp->mant;
	r.lsh_x(uint64_t(maxexp->exp - minexp->exp));
	mpi::add(r, minexp->mant, c.mant);
	c.exp = minexp->exp;
	c.trimit();

*/

void mpf::add(mpf const& a, mpf const& b, mpf& c)
{
	if (a.is_zero())
	{
		c = b;
		return;
	}
	if (b.is_zero())
	{
		c = a;
		return;
	}

	if (a.mant.sign == b.mant.sign)
	{
		int32_t sign = a.mant.sign;
		mpf::abs_add(a, b, c);
		c.mant.sign = sign;
	}
	else
	{
		if (mpf::abs_cmp(a, b) == ord::lt)
		{
			int32_t sign = b.mant.sign;
			mpf::abs_sub(b, a, c);
			c.mant.sign = sign;
		}
		else
		{
			int32_t sign = a.mant.sign;
			mpf::abs_sub(a, b, c);
			c.mant.sign = sign;
		}
	}
	c.trimit();
}
MPL_END