#include "mpf.h"
MPL_BEGIN
/*
用牛顿迭代的方式计算平方根：
LaTeX：\sqrt{x} = y_{n+1} = \frac{1}{2} \times (y_{n} + \frac{x}{y_{n}})
其中：y_{1} = \sqrt{x}
*/
void mpf::sqrt(mpf const& a, mpf& c)
{
	if (a.is_zero())
	{
		c.set_d(0);
		return;
	}
	if (a.is_neg())
	{
		throw MPL_ERR_INVALID_VAL;
	}

	mpf x(a.mant, 0);
	int32_t _exp = a.exp;
	if (a.exp & 1)
	{
		x.mant.lsh_x(1);
		_exp--;
	}
	_exp >>= 1;

	mpf& yn = c;
	mpi::sqrt(x.mant, yn.mant);
	yn.exp = 0;
	mpf x_yn;
	mpf oldyn;
	do
	{
		oldyn = yn;
		mpf::div(x, yn, x_yn);
		mpf::add(yn, x_yn, yn);
		mpf::div_2(yn, yn);
		mpf::sub(yn, oldyn, oldyn);
	} while (!oldyn.is_small_enough());

	yn.exp += _exp;
}
MPL_END