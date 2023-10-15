#include "mpi.h"
MPL_BEGIN
std::string mpi::to_str(uint32_t radix) const
{
	/* 只允许 16 进制的输出 */
	if (radix < 2 || radix > 16)
	{
		throw MPL_ERR_INVALID_VAL;
	}
	if (is_zero())
	{
		return "0";
	}

	std::string str;
	std::vector<mpi_digit> vm = to_radix(radix);

	for (mpi_digit m : vm)
	{
		str.push_back(MPI_DIGITS_STR[m]);
	}
	if (sign == -1)
	{
		str.push_back('-');
	}
	std::reverse(str.begin(), str.end());
	return str;
}

MPL_END