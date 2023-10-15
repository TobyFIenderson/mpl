#include "mpf.h"
MPL_BEGIN
/* 使用牛顿迭代计算立方根 */
void mpf::cubet(mpf const& a, mpf& c)
{
	if (a.is_zero())
	{
		c.set_d(0);
		return;
	}
	if (a.is_abs_one())
	{
		c = a;
		return;
	}

	mpf t1(a.mant, 0);
	int32_t lshd = ((a.exp % 3) + 3) % 3;
	t1.mant.lsh_x(size_t(lshd));
	int32_t _exp = (a.exp - lshd) / 3;
	mpf x = t1;
	mpi::cubet(t1.mant, t1.mant);

	mpf t2, tmp;

	tmp = t1;
	mpf::mul_2(tmp, tmp);
	tmp.mant.lsh(1);
	mpf::sqr(t1, t2);
	mpf::div(x, t2, t2);
	mpf::add(t2, tmp, t2);
	mpf::div_3(t2, t2);

	int32_t n = MPF_PRECISION * MPI_DIGIT_BIT;
	int32_t i = 0;
	while ((1 << i) <= n)
	{
		i++;
	}
	i += 2;
	do
	{
		t1 = t2;
		tmp = t1;
		mpf::mul_2(tmp, tmp);
		mpf::sqr(t1, t2);
		mpf::div(x, t2, t2);
		mpf::add(t2, tmp, t2);
		mpf::div_3(t2, t2);
		if (i-- < 0)
		{
			break;
		}
		mpf::sub(t1, t2, t1);
	} while (!t1.is_small_enough());
	c.swap(t2);
	c.exp += _exp;
}
MPL_END