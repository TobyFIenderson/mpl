#include "mpi.h"
MPL_BEGIN

void mpi::pow_u32(mpi const& a, uint32_t b, mpi& c)
{
	if (b < 128) /* 128 只是一个十分粗略的估计，expt_d 和 k_ray_expt 的效率受多方面条件的影响，
				 比如指数二进制 1 0 的分布，还有底数的长度。比如不同长度的底数的乘法和平方的优化效率不同。 */
	{
		expt_u32(a, b, c);
	}
	else
	{
		k_ray_expt(a, b, c);
	}
}
MPL_END