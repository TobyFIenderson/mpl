#include "mpi.h"
MPL_BEGIN

void mpi::pow_u32(mpi const& a, uint32_t b, mpi& c)
{
	if (b < 128) /* 128 ֻ��һ��ʮ�ִ��ԵĹ��ƣ�expt_d �� k_ray_expt ��Ч���ܶ෽��������Ӱ�죬
				 ����ָ�������� 1 0 �ķֲ������е����ĳ��ȡ����粻ͬ���ȵĵ����ĳ˷���ƽ�����Ż�Ч�ʲ�ͬ�� */
	{
		expt_u32(a, b, c);
	}
	else
	{
		k_ray_expt(a, b, c);
	}
}
MPL_END