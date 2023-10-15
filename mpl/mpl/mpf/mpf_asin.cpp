#include "mpf.h"
MPL_BEGIN
void mpf::asin_taylor(mpf const& a, mpf& c)
{
	mpf x2 = a, t = a;
	mpf::sqr(x2, x2);
	c = a;
	mpi_digit n = 1;
	do
	{
		mpf::mul(t, x2, t);
		mpf::mul_d(t, n * n, t);
		mpf::div_d(t, (n + 1) * (n + 2), t);
		mpf::add(c, t, c);
		n += 2;
	} while (!t.is_small_enough());
}
void mpf::asin(mpf const& a, ang b, mpf& c)
{
	ord o = mpf::abs_cmp(a, 1);
	if (o == ord::gt)
	{
		throw MPL_ERR_INVALID_VAL;
	}
	if (o == ord::eq)
	{
		if (b == ang::deg)
		{
			c.set_d(90);
		}
		else
		{
			c = mpf::const_pi();
			c.trimit();
			mpf::div_2(c, c);
		}
		c.mant.sign = a.mant.sign;
		return;
	}

	int32_t sign = a.mant.sign;
	c = a;
	c.mant.sign = 1;
	mpf halfsqrt2 = mpf::const_sqrt2();
	halfsqrt2.trimit();
	mpf::div_2(halfsqrt2, halfsqrt2);
	o = mpf::abs_cmp(c, halfsqrt2);
	if (o == ord::gt)
	{
		mpf::sqr(c, c);
		mpf::sub(1, c, c);
		mpf::sqrt(c, c);
		mpf::asin_taylor(c, c);
		mpf half_pi = mpf::const_half_pi();
		half_pi.trimit();
		mpf::sub(half_pi, c, c);
	}
	else if (o == ord::eq)
	{
		if (b == ang::deg)
		{
			c.set_d(45);
		}
		else
		{
			c = mpf::const_pi();
			c.trimit();
			mpf::div_2d(c, 2, c);
		}
		c.mant.sign = sign;
		return;
	}
	else
	{
		mpf::asin_taylor(c, c);
	}
	c.mant.sign = sign;
	if (b == ang::deg)
	{
		mpf::mul(c, mpf::const_rtod_ratio(), c);
	}
}
MPL_END