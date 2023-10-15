#include "mpf.h"
MPL_BEGIN
static const mpf MPF_COSH_TAYLOR_CUTOFF = 2500;
void mpf::cosh_taylor(mpf const& a, mpf& c)
{
	mpf x2 = a;
	mpf::sqr(x2, x2);
	mpi_digit n = 0;
	c.set_d(1);
	mpf t = 1;
	do
	{
		mpf::mul(t, x2, t);
		mpf::div_d(t, (n + 1) * (n + 2), t);
		n += 2;
		mpf::add(c, t, c);
	} while (!t.is_small_enough());
}
void mpf::cosh(mpf const& a, mpf& c)
{
	c = a;
	c.mant.sign = 1;
	/* > 2500 (e^x+e^-x)/2 */
	if (c.fraction().is_zero() || mpf::abs_cmp(c, MPF_COSH_TAYLOR_CUTOFF) == ord::gt)
	{
		mpf t;
		mpf::e_expt(c, c);
		mpf::inv(c, t);
		mpf::add(c, t, c);
		mpf::div_2(c, c);
	}
	else
	{
		mpf::cosh_taylor(c, c);
	}
}
MPL_END