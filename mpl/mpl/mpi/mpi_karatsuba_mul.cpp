#include "mpi.h"
#include <algorithm>
MPL_BEGIN

void mpi::karatsuba_mul(mpi const& a, mpi const& b, mpi& c)
{
	size_t B = std::min(a.used, b.used) >> 1;
	mpi x0   = create_size(B);
	mpi y0   = create_size(B);
	mpi x1   = create_size(a.used - B);
	mpi y1   = create_size(b.used - B);
	mpi x0y0 = create_size(B << 1);
	mpi x1y1 = create_size(B << 1);
	mpi t1   = create_size(B << 1);

	x0.used = y0.used = B;
	x1.used = a.used - B;
	y1.used = b.used - B;

	const mpi_digit* tmpa = a.dp;
	const mpi_digit* tmpb = b.dp;
	mpi_digit* tmpx = x0.dp;
	mpi_digit* tmpy = y0.dp;
	for (size_t i = 0; i < B; i++)
	{
		*tmpx++ = *tmpa++;
		*tmpy++ = *tmpb++;
	}

	tmpx = x1.dp;
	for (size_t i = B; i < a.used; i++)
	{
		*tmpx++ = *tmpa++;
	}

	tmpy = y1.dp;
	for (size_t i = B; i < b.used; i++)
	{
		*tmpy++ = *tmpb++;
	}

	/* 在执行完这条语句之后，不再需要 x0 的值, x0 将当作 t2 使用 */
	mul(x0, y0, x0y0);   /* x_0y_0 = x_0 \times y_0  */
	mul(x1, y1, x1y1);   /* x_1y_1 = x_1 \times y_1  */

	abs_add(x1, x0, t1); /* t_1 = \left|x_1\right| + \left|x_0\right| */
	abs_add(y1, y0, x0); /* t_2 = \left|y_1\right| + \left|y_0\right| */
	mul(t1, x0, t1);     /* t_1 = (x_1 + x_0) \times (y_1 + y_0) */

	abs_add(x0y0, x1y1, x0); /* t_2 = x_0y_0 + x_1y_1 */
	abs_sub(t1, x0, t1); /* t_1 = (x_1 + x_0) \times (y_1 + y_0) - (x_1y_1 + x_0y_0) */

	t1.lsh_x(B);         /* t_1 = [(x_1 + x_0) \times (y_1 + y_0) - (x_1y_1 + x_0y_0)] << B */
	x1y1.lsh_x(B << 1);  /* x_1y_1 = x_1y_1 << 2B */
	abs_add(x0y0, t1, t1);   /* t1 = x_0y_0 + t_1 */
	abs_add(t1, x1y1, c);    /* result = x_0y_0 + t_1 + x_1y_1 */
}

MPL_END