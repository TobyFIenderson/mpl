#include "mpf.h"
MPL_BEGIN

/* 使用泰勒展开计算，于是可以推出递推式：
f\left( n+1 \right) =f\left( n \right) \times \frac{-x^2}{2n\cdot \left( 2n+1 \right)}
f\left( 0 \right) =x
\sin x=\sum_{n=0}^{\infty}{f\left( n \right)}
*/
void mpf::sin_taylor(mpf const& a, mpf& c)
{
	mpf x2 = a;
	mpf::sqr(x2, x2); /* x2 = -x^2 */
	x2.mant.sign = -1;
	mpi_digit n = 1;
	mpf fn = a;
	c = a;            /* c = x */
	do
	{
		mpf::mul(fn, x2, fn);
		mpf::div_d(fn, (n + 1) * (n + 2), fn);
		mpf::add(c, fn, c);
		n = n + 2;
	} while (!fn.is_small_enough());
}

void mpf::sin(mpf const& a, ang b, mpf& c)
{
	mpf aa;
	switch (b)
	{
	case ang::deg:
		mpf::mod(a, 360, aa);
		mpf::mul(aa, mpf::const_dtor_ratio(), aa);
		break;
	case ang::rad:
		mpf::mod_2pi(a, aa);
		break;
	default:
		throw MPL_ERR_INVALID_VAL;
		break;
	}
	mpf::sin_taylor(aa, c);
	if (c.is_ignorable())
	{
		c.set_d(0);
	}
}

MPL_END