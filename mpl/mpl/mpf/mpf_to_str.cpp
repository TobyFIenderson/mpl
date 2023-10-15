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

	/* ��� exponent < 0 ��ô�����ֵ��ʾ��Ҫ����ĸ���
	���� 23 * 10^{-3} ��ʱ exponent = -1 ��Ҫ�� 23 ǰ��
	�� 0 ʹ���Ϊ 0.023 ���ȴ�ʱ����Ϊ 3 λ��Ч����
	exponent > 0 ��ô��ʾ�������ֵĳ��ȣ� exponent == 0
	��ʾ������������ȫ��С�������Ҳ���Ҫ�� 0 */
	int32_t exponent = length + num.exp;

	/* precision һ��������������������ɶ��˼��������������
	Ҫ���� exponent > precision �Ļ� exponent ���� > 0
	�������ᵽ��exponent ��ʾ�������ֵ�λ����С�����ֿ���
	ͨ���и�λ������ʾ precision λ��Ч���֡����磬����
	123456789 * 10^{-3} ���Ա�ʾΪ 123456.789 ��ʱΪ 9 λ��
	Ч���֣�������������Ч���ֽ�Ϊ 7 λ������� 123456.8
	�������������롣С�����ֿ�������������������������
	����취ȴ�в�ͨ�����ǿ��� 123456.789����Ҫ��������
	4 λ��Чλ������Ļ�ֻ�ܲ��ÿ�ѧ��������� 1.235 * 10^{6}
	����������С��������������� 1235 ���������ǲ��Ե� */
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