#include "mpf.h"
MPL_BEGIN

void mpf::pow(mpf const& a, mpf const& b, mpf& c)
{
	/*假定 0^0 = 1 (lim(x-->0){x^x}) */
	if (b.is_zero())
	{
		c.set_d(1);
		return;
	}

	if (a.is_zero())
	{
		/* 1 / 0 */
		if (b.is_neg())
		{
			throw MPL_ERR_DIV_BY_ZERO;
		}

		/* 0^x = 0 */
		c.set_d(0);
		return;
	}

	if (a.is_abs_one() && !a.is_neg())
	{
		c.set_d(1);
		return;
	}

	if (b.exp >= 0)
	{
		mpf::pow_mpi(a, b, c);
		return;
	}

	if (a.mant.sign == -1)
	{
		/* 当底数是负数时要判定类似于 -1 ^ (1/2) 的情况，因为此时是没有实数解的
		可是关于 mpf 的奇偶性判定，由于其精度有限的特性，作者暂时还没有想出有效的办法
		因此这里一刀切，只要是底数是负数，而指数为非整数，那么就将抛出异常 */
		/* 个人觉得可行的一条路是，实现一个 mpr 类，该类由两个 mpf 组成，
		一个做为分子，一个做为分母，这样所有的有理数都能够精确地表示出来，使得对于奇偶的判定
		就能够相对简单地实现，不过这样就要重写一个类，工作量大，而且考虑使用十分频繁的加法，
		在 mpf 中只需要一次加法的复杂度，而如果按照像上述描述的那样，两个分数相加免不了要先进行通分
		这样一次 mpr 加法就要使用 3 次 mpf 乘法和 1 次 mpf 加法，时间将大大增加 */
		throw MPL_ERR_INVALID_VAL;
	}
	/* b 是不整数 a^b = e^bln(a) */
	c = a;
	mpf::ln(c, c);
	mpf::mul(c, b, c);
	mpf::e_expt(c, c);
}
MPL_END