#include "mpi.h"
MPL_BEGIN
static constexpr uint32_t TAB_SIZE = 256;
#pragma warning(push)
#pragma warning(disable:4334) 
/*
本人才疏学浅，此代码会在函数的最后一行，
第 150 行抛出 C4334: 32 位移位的结果被隐式转换为 64 位(是否希望进行 64 位移位?) 这个警告。
是的，没看错是最后一行，这一行只有一个大括号 '}' ！！！
一个大括号为啥会报警告。。。百思不得其解，权当编译器抽风了。。。
*/
void mpi::k_ray_expt(mpi const& a, mpi const& b, mpi& c)
{
	if (b.is_zero())
	{
		c.set_d(1);
		return;
	}
	if (b.sign == -1)
	{
		throw MPL_ERR_INVALID_VAL;
	}
	if (b.is_abs_one())
	{
		c = a;
		return;
	}
	/* M[i] = a**i */
	mpi* M[TAB_SIZE]{}, r = create_size(b.dp[0] * a.used);
	size_t bit_count, k;

	for (size_t i = 0; i < TAB_SIZE; i++)
	{
		M[i] = nullptr;
	}

	bit_count = b.cnt_bits();
	if (bit_count <= 7)
	{
		k = 2;
	}
	else if (bit_count <= 36)
	{
		k = 3;
	}
	else if (bit_count <= 140)
	{
		k = 4;
	}
	else if (bit_count <= 450)
	{
		k = 5;
	}
	else if (bit_count <= 1303)
	{
		k = 6;
	}
	else if (bit_count <= 3529)
	{
		k = 7;
	}
	else /* 超过 3529 时认定 k 值为 8 */
	{
		k = 8;
	}
	r.set_d(1);
	/* M[1] = a */
	M[1] = new mpi(a);

	/* k 位二进制的最高位只能是 1 */
	for (size_t i = 1ui64 << (k - 1); i < (1ui64 << k); i++)
	{
		M[i] = new mpi();
	}
	*(M[1 << (k - 1)]) = *(M[1]);

	/* 先计算 M[10...0b] */
	for (size_t i = 0; i < k - 1; i++)
	{
		sqr(*(M[1 << (k - 1)]), *(M[1 << (k - 1)]));
	}

	/* 再计算剩下的 M */
	for (size_t i = (1ui64 << (k - 1)) + 1; i < (1ui64 << k); i++)
	{
		mul(*(M[i - 1]), *(M[1]), *(M[i]));
	}

	size_t bitcnt = 1,					/* 记录当前数位还剩下多少二进制位 */
		digit_idx = b.used - 1,    	    /* index */
		bit = 0,
		mode = 0,
		bitbuf = 0,
		bitcntk = 0;
	mpi_digit buf = 0;

	while (1)
	{
		if (--bitcnt == 0)
		{
			if (digit_idx == -1)
			{
				break;
			}
			buf = b.dp[digit_idx--];
			bitcnt = (size_t)MPI_DIGIT_BIT;
		}

		/* bit 为 buf 的最高位 */
		bit = (buf >> (mpi_digit(MPI_DIGIT_BIT - 1))) & 1;
		buf <<= 1;

		if (mode == 0 && bit == 0)
		{
			continue;
		}

		if (bit == 0 && mode == 1)
		{
			sqr(r, r);
			continue;
		}

		bitbuf |= (bit << (k - ++bitcntk));
		mode = 2;
		if (bitcntk == k)
		{
			for (size_t i = 0; i < k; i++)
			{
				sqr(r, r);
			}
			if (M[bitbuf] != nullptr)
			{
				mul(r, *(M[bitbuf]), r);
			}

			bitcntk = 0;
			bitbuf = 0;
			mode = 1;
		}
	}

	/* 现在处理剩余的部分 */
	if (mode == 2 && bitcntk > 0)
	{
		for (size_t i = 0; i < bitcntk; i++)
		{
			sqr(r, r);

			bitbuf <<= 1ui64;
			if ((bitbuf & (1ui64 << k)) != 0)
			{
				mul(r, *(M[1]), r);
			}
		}
	}
	r.swap(c);

	delete M[1];
	M[1] = nullptr;
	for (int i = 1 << (k - 1); i < (1 << k); i++)
	{
		delete M[i];
		M[i] = nullptr;
	}
}
#pragma warning(pop)
MPL_END