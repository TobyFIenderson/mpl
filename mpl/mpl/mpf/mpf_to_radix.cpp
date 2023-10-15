#include "mpf.h"
#include <math.h>
MPL_BEGIN

mpf_radix mpf::to_radix(uint32_t radix, int32_t precision) const
{
	if (mant.used == 0)
	{
		return mpf_radix{ 1,{},radix,0 };
	}

	double radix_ratio = std::max(log(MPI_BASE) / log(radix), 1.0);
	int32_t max_prec = int32_t(MPF_ORIGINAL_PRECISION * radix_ratio); /* 支持的最大精度 */
	if (precision > max_prec) /* 不能超过最大精度 */
	{
		precision = max_prec;
	}

	if (exp >= 0) /* 整数只能通过计算得出结果 */
	{
		mpf_radix ret{ mant.sign,std::vector<mpi_digit>(mant.dp,mant.dp + mant.used), MPI_BASE, exp };
		return ret.to_radix(radix, precision);
	}

	int32_t rdigit = rdig() - 1; /* 减一是为了去除当最高位不够大时对精度的影响 */
	int32_t alprec = int32_t(rdigit * radix_ratio);
	int32_t needprec = precision - alprec;
	if (needprec <= 0) /* 已有的整数部分已经可以满足精度的要求了 */
	{
		mpi _mant = to_mpi();
		mpf_radix ret{ _mant.sign,std::vector<mpi_digit>(_mant.dp,_mant.dp + _mant.used), MPI_BASE, 0 };
		return ret.to_radix(radix, precision);
	}
	/* 已有的整数部分还无法满足精度要求 */
	int32_t needdigit = int32_t(needprec / radix_ratio);
	int32_t radix_pow_exp = int32_t((needdigit + 1) * radix_ratio) + 1;
	mpi _mant = mant;
	mpi radix_pow = radix;
	mpi::pow(radix_pow, uint32_t(radix_pow_exp), radix_pow);
	mpi::mul(_mant, radix_pow, _mant);
	_mant.rsh_x(uint32_t(-exp)); /* 注意这里调用的是 mpi::rsh_x() 用于将 _mant 表示为整数 */
	mpf_radix ret{ _mant.sign,std::vector<mpi_digit>(_mant.dp,_mant.dp + _mant.used), MPI_BASE, 0 };
	ret = ret.to_radix(radix, precision);
	ret.exp -= radix_pow_exp;
	ret.sign = mant.sign;
	return ret;
}
MPL_END