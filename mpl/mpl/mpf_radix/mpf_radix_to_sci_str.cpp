#include "mpf_radix.h"
MPL_BEGIN
std::string mpf_radix::to_sci_str() const
{
	std::string str;

	if (sign == -1)
	{
		str += '-';
	}
	for (int32_t i = (int32_t)mant.size() - 1; i >= 0; i--)
	{
		str += MPI_DIGITS_STR[mant[i]];
	}
	int32_t _exp = exp + int32_t(mant.size()) - 1;
	if (str.length() > 1)
	{
		str.insert(str.begin() + (sign == -1 ? 2 : 1), MPF_DECIMAL_SEPARATOR);
	}
	while (!str.empty() && str.back() == '0')
	{
		str.pop_back();
	}
	if (!str.empty() && str.back() == MPF_DECIMAL_SEPARATOR)
	{
		str.pop_back();
	}
	if (_exp != 0)
	{
		str.push_back(radix == 10 ? 'e' : '^');
		if (_exp > 0)
		{
			str.push_back('+');
		}
		str += std::to_string(_exp);
	}
	return str;
}
MPL_END