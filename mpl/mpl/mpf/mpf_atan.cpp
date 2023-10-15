#include "mpf.h"
MPL_BEGIN
void mpf::atan_taylor(mpf const& a, mpf& c)
{
	mpf x2 = a;
	mpf::sqr(x2, x2);
	x2.mant.sign = -1;
	mpi_digit n = 1;
	mpf t = a;
	c = a;
	mpf tt;
	do
	{
		mpf::mul(t, x2, t);
		n += 2;
		mpf::div_d(t, n, tt);
		mpf::add(c, tt, c);
	} while (!tt.is_small_enough());
}

void mpf::atan(mpf const& a, ang b, mpf& c)
{
	int32_t sign = a.mant.sign;
	c = a;
	c.mant.sign = 1;
	if (mpf::abs_cmp(c, 1) == ord::eq)
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
	mpf sqrt2 = mpf::const_sqrt2();
	sqrt2.trimit();
	mpf halfsqrt2 = sqrt2;
	mpf::div_2(halfsqrt2, halfsqrt2);
	if (mpf::abs_cmp(c, halfsqrt2) == ord::gt)
	{
		if (mpf::abs_cmp(c, sqrt2) == ord::gt)
		{
			mpf::inv(c, c);
			mpf::atan_taylor(c, c);
			c.mant.sign = sign;
			mpf halfpi = mpf::const_half_pi();
			halfpi.trimit();
			mpf::sub(halfpi, c, c);
		}
		else
		{
			mpf t = c;
			mpf::sqr(t, t);
			mpf::add(t, 1, t);
			mpf::sqrt(t, t);
			mpf::div(c, t, c);
			mpf::asin(c, ang::rad, c);
			c.mant.sign = sign;
		}
	}
	else
	{
		mpf::atan_taylor(c, c);
		c.mant.sign = sign;
	}
	c.swap(c);
	if (b == ang::deg)
	{
		mpf::mul(c, mpf::const_rtod_ratio(), c);
	}
}
MPL_END