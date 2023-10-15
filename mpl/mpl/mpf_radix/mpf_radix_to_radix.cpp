#include "mpf_radix.h"
MPL_BEGIN

static double_complex* a;
static int* rev;

static mpf_radix _radix_convert(mpf_radix const& input, size_t beg_idx, size_t end_idx, mpi_digit output_radix, int32_t precision)
{
	uint32_t len = uint32_t(end_idx - beg_idx);
	if (len <= MPF_RADIX_CONVERT_CUTOFF)
	{
		mpi ret(1, len, len, (mpi_digit*)(input.mant.data() + beg_idx));
		mpf_radix rett{ 1,ret.to_radix(output_radix),output_radix ,0 };
		ret.dp = nullptr;
		rett.trimit(precision);
		return rett;
	}
	len >>= 1;
	mpf_radix left = _radix_convert(input, beg_idx, beg_idx + len, output_radix, precision);
	mpf_radix right = _radix_convert(input, beg_idx + len, end_idx, output_radix, precision);
	mpf_radix input_radix(input.radix, output_radix);
	input_radix.pow_u32(len, precision, a, rev);
	right.mul(input_radix, precision, a, rev);
	left.abs_add(right, precision);
	return left;
}

mpf_radix mpf_radix::to_radix(mpi_digit output_radix, int32_t precision) const
{
	if (exp < 0)
	{
		throw MPL_ERR_INVALID_VAL; /* 仅支持整数转换 */
	}
	precision = precision + MPF_RADIX_EXTRA_PRECISION;

	int32_t n = int32_t(precision - 1);
	int32_t limit = 1;
	while (limit <= n + n)
	{
		limit <<= 1;
	}
	a = new double_complex[limit + 1]{};
	rev = new int[limit + 1]{};
	mpf_radix ret = _radix_convert(*this, 0, mant.size(), output_radix, precision + MPF_RADIX_EXTRA_PRECISION);
	if (exp > 0)
	{
		mpf_radix ret_pow(radix, output_radix);
		ret_pow.pow_u32(uint32_t(exp), precision, a, rev);
		ret.mul(ret_pow, precision, a, rev);
	}
	delete[] a;
	delete[] rev;
	a = nullptr;
	rev = nullptr;
	ret.sign = sign;
	return ret;
}
MPL_END