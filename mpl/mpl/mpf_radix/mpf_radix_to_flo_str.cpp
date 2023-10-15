#include "mpf_radix.h"
MPL_BEGIN
std::string mpf_radix::to_flo_str() const
{
	std::string str;
	if (sign == -1)
	{
		str += '-';
	}

	if (exp >= 0)
	{
		for (int32_t i = int32_t(mant.size()) - 1; i >= 0; i--)
		{
			str += MPI_DIGITS_STR[mant[i]];
		}
		for (int32_t i = 0; i < exp; i++)
		{
			str += MPI_DIGITS_STR[0];
		}
	}
	else
	{
		int32_t _exp = exp + int32_t(mant.size());

		if (_exp <= 0)
		{
			str += MPI_DIGITS_STR[0];
			str += MPF_DECIMAL_SEPARATOR;

			for (int32_t i = 0; i < -_exp; i++)
			{
				str += MPI_DIGITS_STR[0];
			}
		}
		for (int32_t i = int32_t(mant.size()) - 1; i >= 0; i--)
		{
			str += MPI_DIGITS_STR[mant[i]];
		}
		if (_exp > 0)
		{
			str.insert(str.end() + exp, MPF_DECIMAL_SEPARATOR);
		}
		while (!str.empty() && str.back() == '0')
		{
			str.pop_back();
		}
		if (!str.empty() && str.back() == MPF_DECIMAL_SEPARATOR)
		{
			str.pop_back();
		}

	}
	return str;
}
MPL_END