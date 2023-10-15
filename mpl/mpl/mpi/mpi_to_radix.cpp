#include "mpi.h"
MPL_BEGIN

std::vector<mpi_digit> mpi::to_radix(uint32_t radix) const
{
	/* 只允许 16 进制的输出 */
	if (radix < 2 || radix > 16)
	{
		throw MPL_ERR_INVALID_VAL;
	}
	if (is_zero())
	{
		return {};
	}

	std::vector<mpi_digit> vm;
	mpi_digit d = 0;

	if ((radix & (radix - 1)) == 0)
	{
		uint32_t w = 0;
		while ((1u << w) < radix)
		{
			++w;
		}
		uint32_t mask = (1 << w) - 1;
		uint32_t b = 0, t = 0, already = 0;
		for (uint32_t i = 0; i < used; i++)
		{
			d = dp[i];
			uint32_t ww = MPI_DIGIT_BIT;

			if (b)
			{
				if (ww >= b)
				{
					t |= (d & ((1 << b) - 1)) << already;
					vm.push_back(t);
					ww -= b;
					d >>= b;
					b = 0;
					t = 0;
				}
				else
				{
					t |= d << already;
					b -= ww;
					already += ww;
					ww = 0;
				}
			}

			while (ww)
			{
				if (ww >= w)
				{
					vm.push_back(d & mask);
					ww -= w;
					d >>= w;
				}
				else
				{
					t = d & ((1 << ww) - 1);
					already = ww;
					b = w - ww;
					ww = 0;
				}
			}
		}

		if (t)
		{
			vm.push_back(t);
		}
	}
	else
	{
		mpi t = *this;
		t.sign = 1;
		while (t.used > 0)
		{
			div_d(t, radix, &t, &d);
			vm.push_back(d);
		}
	}

	while (!vm.empty() && vm.back() == 0)
	{
		vm.pop_back();
	}

	return vm;
}

MPL_END