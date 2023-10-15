#include "mpf.h"
#include <sstream>
MPL_BEGIN

std::string mpf::to_str(uint32_t radix, num_format format, int32_t precision) const
{
	if (mant.used == 0)
	{
		return "0";
	}
	mpf_radix num = to_radix(radix, precision);
	num.trimit(precision, true);
	int32_t length = int32_t(num.mant.size());

	/* 如果 exponent < 0 那么其绝对值表示需要补零的个数
	比如 23 * 10^{-3} 此时 exponent = -1 需要在 23 前面
	补 0 使其变为 0.023 精度此时提升为 3 位有效精度
	exponent > 0 那么表示整数部分的长度， exponent == 0
	表示，无整数部分全是小数，而且不需要补 0 */
	int32_t exponent = length + num.exp;

	/* precision 一般是正数（负数精度是啥意思啊，汗。。。）
	要满足 exponent > precision 的话 exponent 必须 > 0
	上面有提到，exponent 表示整数部分的位数，小数部分可以
	通过切割位数来表示 precision 位有效数字。比如，对于
	123456789 * 10^{-3} 可以表示为 123456.789 此时为 9 位有
	效数字，但如果输出的有效数字降为 7 位，将输出 123456.8
	进行了四舍五入。小数部分可以这样做，但对于整数部分
	这个办法却行不通，还是考虑 123456.789，如要将此数以
	4 位有效位数输出的话只能采用科学计数法输出 1.235 * 10^{6}
	而不是像处理小数部分那样，输出 1235 ，这明显是不对的 */
	if (exponent > precision || (exponent < 0 && -exponent > MPF_MAX_ZERO_AFTER_DECIMAL))
	{
		format = num_format::sci;
	}

	switch (format)
	{
	case num_format::flo:
		return num.to_flo_str();
		break;
	case num_format::sci:
		return num.to_sci_str();
		break;
	default:
		return "BAD FORMAT!!!";
		break;
	}
}

MPL_END