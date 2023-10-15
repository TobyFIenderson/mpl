#include "mpf.h"
MPL_BEGIN
void mpf::asinh_taylor(mpf const& a, mpf& c)
{
	mpf x2 = a;
	mpf::sqr(x2, x2);
	x2.mant.sign = -1;
	mpf t = a;
	mpi_digit n = 1;
	c = a;
	do
	{
		mpf::mul(t, x2, t);
		mpf::mul_d(t, n * n, t);
		mpf::div_d(t, (n + 1) * (n + 2), t);
		n += 2;
		mpf::add(c, t, c);
	} while (!t.is_small_enough());
}

void mpf::asinh(mpf const& a, mpf& c)
{
	if (a.is_zero())
	{
		c.set_d(0);
		return;
	}
	mpf sqrthalf = mpf::const_sqrt2();
	sqrthalf.trimit();
	mpf::div_2(sqrthalf, sqrthalf);
	int32_t sign = a.mant.sign;
	c = a;
	c.mant.sign = 1;
	if (mpf::abs_cmp(c, sqrthalf) == ord::gt)
	{
		mpf t = c;
		mpf::sqr(t, t);
		mpf::add(t, 1, t);
		mpf::sqrt(t, t);
		mpf::add(t, c, c);
		mpf::ln(c, c);
	}
	else
	{
		asinh_taylor(c, c);
	}
	c.mant.sign = sign;
}

MPL_END