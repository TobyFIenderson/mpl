#include "mpf.h"
MPL_BEGIN

void mpf::pow(mpf const& a, mpf const& b, mpf& c)
{
	/*�ٶ� 0^0 = 1 (lim(x-->0){x^x}) */
	if (b.is_zero())
	{
		c.set_d(1);
		return;
	}

	if (a.is_zero())
	{
		/* 1 / 0 */
		if (b.is_neg())
		{
			throw MPL_ERR_DIV_BY_ZERO;
		}

		/* 0^x = 0 */
		c.set_d(0);
		return;
	}

	if (a.is_abs_one() && !a.is_neg())
	{
		c.set_d(1);
		return;
	}

	if (b.exp >= 0)
	{
		mpf::pow_mpi(a, b, c);
		return;
	}

	if (a.mant.sign == -1)
	{
		/* �������Ǹ���ʱҪ�ж������� -1 ^ (1/2) ���������Ϊ��ʱ��û��ʵ�����
		���ǹ��� mpf ����ż���ж��������侫�����޵����ԣ�������ʱ��û�������Ч�İ취
		�������һ���У�ֻҪ�ǵ����Ǹ�������ָ��Ϊ����������ô�ͽ��׳��쳣 */
		/* ���˾��ÿ��е�һ��·�ǣ�ʵ��һ�� mpr �࣬���������� mpf ��ɣ�
		һ����Ϊ���ӣ�һ����Ϊ��ĸ���������е����������ܹ���ȷ�ر�ʾ������ʹ�ö�����ż���ж�
		���ܹ���Լ򵥵�ʵ�֣�����������Ҫ��дһ���࣬�������󣬶��ҿ���ʹ��ʮ��Ƶ���ļӷ���
		�� mpf ��ֻ��Ҫһ�μӷ��ĸ��Ӷȣ����������������������������������������ⲻ��Ҫ�Ƚ���ͨ��
		����һ�� mpr �ӷ���Ҫʹ�� 3 �� mpf �˷��� 1 �� mpf �ӷ���ʱ�佫������� */
		throw MPL_ERR_INVALID_VAL;
	}
	/* b �ǲ����� a^b = e^bln(a) */
	c = a;
	mpf::ln(c, c);
	mpf::mul(c, b, c);
	mpf::e_expt(c, c);
}
MPL_END