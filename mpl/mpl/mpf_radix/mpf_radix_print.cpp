#include "mpf_radix.h"
MPL_BEGIN
void mpf_radix::print() const
{
	if (sign == -1)
	{
		std::cout << '-';
	}
	for (int i = (int)mant.size() - 1; i >= 0; i--)
	{
		std::cout << MPI_DIGITS_STR[mant[i]];
	}

	if (exp != 0)
	{
		std::cout << " * " << radix << "^" << exp;
	}
	std::cout << std::endl;
}
MPL_END