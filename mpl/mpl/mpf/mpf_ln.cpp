#include "mpf.h"
MPL_BEGIN
void mpf::ln_taylor(mpf const& a, mpf& c)
{
	mpf i = 3;
	mpf y = a; /* y = (x - 1) / (x + 1) */
	c = a;
	mpf _1 = 1;
	mpf _2 = 2;
	mpf::add(y, _1, y);
	mpf::sub(c, _1, c);
	mpf::div(c, y, y);
	c = y;
	mpf::mul_2(c, c);
	mpf sqr_y;
	mpf::sqr(y, sqr_y);
	mpf thisterm;
	mpf y_pow = y;
	do
	{
		mpf::mul(y_pow, sqr_y, y_pow);
		mpf::div(y_pow, i, thisterm);
		mpf::mul_2(thisterm, thisterm);
		mpf::add(i, _2, i);
		mpf::add(c, thisterm, c);
	} while (!thisterm.is_small_enough());
}

void mpf::ln(mpf const& a, mpf& c)
{
	/* 负数没有对数 */
	if (a.is_zero() || a.is_neg())
	{
		throw MPL_ERR_INVALID_VAL;
	}
	if (a.is_abs_one())
	{
		c.set_d(0);
		return;
	}

	c = a;
	int32_t rdigit = c.rdig();
	bool neg_ret = rdigit <= 0;
	if (neg_ret)
	{
		/* 当 a < 1 时 (x-1)/(x+1) 的值下降得很快
		将会有很很大概率趋于 -1 ，这将会影响泰勒级数的收敛速度 */
		mpf::inv(c, c);
	}
	/*
	我们知道，一个 mpf 可以表现为以下形式 mant * MPI_BASE^exp
	而 MPI_BASE = 2^MPI_DIGIT_BIT 于是
	ln(mant * MPI_BASE^exp) = ln(mant) + ln(MPI_BASE^exp)
							= ln(mant) + MPI_DIGIT_BIT*exp*ln(2)
	*/
	mpf expln2 = 0;
	int32_t exp2 = 0;
	if (rdigit > 1)
	{
		exp2 = rdigit - 1;
		c.exp -= exp2;
		exp2 *= MPI_DIGIT_BIT;
	}

	uint32_t msd_digit = c.mant.dp[c.mant.used - 1];
	uint32_t msd_bit = 0;
	while (msd_digit > 0)
	{
		++msd_bit;
		msd_digit >>= 1;
	}

	exp2 += msd_bit - 1;
	expln2 = exp2;
	mpf::mul(expln2, mpf::const_ln2(), expln2);

	mpf::div_2d(c, msd_bit - 1, c);
	mpf::ln_taylor(c, c);
	mpf::add(c, expln2, c);
	if (neg_ret)
	{
		c.mant.sign *= -1;
	}
}
MPL_END