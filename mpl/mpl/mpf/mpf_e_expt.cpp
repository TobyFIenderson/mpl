#include "mpf.h"
MPL_BEGIN
/*
使用泰勒展开计算 e^{x}：
LaTex： e^{x} = \sum_{n=0}^{\infty} {x^{n} \over n!}
于是：
f\left( n \right) =f\left( n-1 \right) \times \left( \frac{x}{n} \right) ,f\left( 0 \right) =x
result = \sum_{i=0}^n{f\left( i \right)}
*/
void mpf::e_expt_taylor(mpf const& a, mpf& c)
{
	if (a.is_zero())
	{
		c.set_d(1);
		return;
	}
	mpf x = a;
	mpf n = 1;
	mpf fn = 1;
	c.set_d(1);
	do
	{
		mpf::mul(fn, x, fn);
		mpf::div(fn, n, fn);
		mpf::add(c, fn, c);
		n.mant.incr();
	} while (!fn.is_small_enough());
}

void mpf::e_expt(mpf const& a, mpf& c)
{
	int32_t sign = a.mant.sign;
	mpf inte = a.integer();
	mpf frac = a.fraction();
	inte.mant.sign = 1;
	frac.mant.sign = 1;
	mpf::pow_mpi(mpf::const_e(), inte.mant, inte);
	mpf::e_expt_taylor(frac, frac);
	mpf::mul(inte, frac, c);
	if (sign == -1)
	{
		mpf::inv(c, c);
	}
}
MPL_END