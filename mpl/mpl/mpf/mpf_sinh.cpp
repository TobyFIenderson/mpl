#include "mpf.h"
MPL_BEGIN
static const mpf MPF_SINH_TAYLOR_CUTOFF = 2500;
void mpf::sinh_taylor(mpf const& a, mpf& c)
{
	mpf x2 = a;
	mpf::sqr(x2, x2);
	mpf t = a;
	mpi_digit n = 1;
	c = a;
	do
	{
		mpf::mul(t, x2, t);
		mpf::div_d(t, (n + 1) * (n + 2), t);
		n += 2;
		mpf::add(c, t, c);
	} while (!t.is_small_enough());
}

void mpf::sinh(mpf const& a, mpf& c)
{
	int32_t sign = a.mant.sign;
	c = a;
	c.mant.sign = 1;
	/* > 2500 (e^x-e^-x)/2 */
	if (c.fraction().is_zero() || mpf::abs_cmp(c, MPF_SINH_TAYLOR_CUTOFF) == ord::gt)
	{
		mpf t;
		mpf::e_expt(c, c);
		mpf::inv(c, t);
		mpf::sub(c, t, c);
		mpf::div_2(c, c);
	}
	else
	{
		mpf::sinh_taylor(c, c);
	}
	c.mant.sign = sign;
}
MPL_END