#include "mpi.h"
MPL_BEGIN

mpi mpi::str_to_mpi(std::string_view str, uint32_t radix)
{
	/* 只允许 16 进制的输出 */
	if (radix < 2 || radix > 16)
	{
		throw MPL_ERR_INVALID_VAL;
	}

	if (str.empty())
	{
		return mpi(0);
	}
	int32_t sign = 1;
	if (str[0] == '-')
	{
		sign = -1;
		str = str.substr(1);
	}

	mpi_digit d = 0;
	/* 这一步只是进行非常粗略的估计 create_size() 会保证 alloc >= MPI_MINI_PREC */
	mpi r = create_size(str.length() / (size_t(get_radix_ratio(radix)) - 1));
	if ((radix & (radix - 1)) == 0)
	{
		uint32_t w = 0;
		while ((1u << w) < radix)
		{
			++w;
		}
		for (char c : str)
		{
			if (!(is_decnum(c) || is_hexnum(c)))
			{
				break;
			}
			d = c_to_u(c);
			r.lsh(w);
			add_d(r, d, r);
		}
	}
	else
	{
		for (char c : str)
		{
			if (!(is_decnum(c) || is_hexnum(c)))
			{
				break;
			}
			d = c_to_u(c);
			mul_d(r, mpi_digit(radix), r);
			add_d(r, d, r);
		}
	}
	r.sign = sign;
	r.clamp();
	return r;
}


MPL_END