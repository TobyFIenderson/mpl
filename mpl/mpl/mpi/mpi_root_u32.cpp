#include "mpi.h"
MPL_BEGIN
void mpi::root_u32(mpi const& a, uint32_t b, mpi& c)
{
	/* 负数没有偶次方根 */
	if (b == 0)
	{
		throw MPL_ERR_INVALID_VAL;
	}
	if ((b & 1) == 0 && a.is_neg())
	{
		throw MPL_ERR_INVALID_VAL;
	}
	if (a.is_zero())
	{
		c.set_d(0);
		return;
	}
	if (b == 1)
	{
		c = a;
		return;
	}

	mpi t1, t2, t3;
	uint64_t ilog2 = a.cnt_bits();
	if (ilog2 < b)
	{
		c.set_d(1);
		c.sign = a.sign;
		return;
	}
	ilog2 /= b;
	ilog2 += 2;
	t2.set_2expt(uint32_t(ilog2));
	mpi _a(1, a.alloc, a.used, a.dp); /* 浅拷贝 */
	do
	{
		t1 = t2;
		/* t_2 = t_1 - (t_1^b - a) \div (b \times t_1^{b - 1}) */
		expt_u32(t1, b - 1, t3);  /* t_3 = t_1^{b - 1} */
		mul(t3, t1, t2);          /* t_2 = t_1^b */
		sub(t2, _a, t2);          /* t_2 = t1^b - a */
		mul_d(t3, b, t3);         /* t_3 = t_1^b - a */
		div(t2, t3, &t3, nullptr);/* t_3 = (t_1^b - a) \div (b \times t_1^{b - 1}) */
		sub(t1, t3, t2);          /* t_2 = t_1 - (t_1^b - a) \div (b \times t_1^{b - 1}) */
		/* 达到最大迭代次数，手动退出 */
		if (ilog2-- == 0)
		{
			break;
		}
	} while (cmp(t1, t2) != ord::eq);

	while (true)
	{
		expt_u32(t1, b, t2);
		ord o = cmp(t2, _a);
		if (o == ord::eq)
		{
			goto CLEAN;
		}
		if (o == ord::lt)
		{
			add_d(t1, 1, t1); /* 手动调整 */
		}
		else
		{
			break;
		}
	}

	while (true)
	{
		expt_u32(t1, b, t2);
		if (cmp(t2, _a) == ord::gt)
		{
			sub_d(t1, 1, t1); /* 手动调整 */
		}
		else
		{
			break;
		}
	}

CLEAN:
	c.swap(t1);
	c.sign = a.sign;
	_a.dp = nullptr; /* 浅拷贝 */
}
MPL_END