#include "mpi.h"
MPL_BEGIN
void mpi::karatsuba_sqr(mpi const& a, mpi& b)
{
	size_t B = a.used >> 1;
	mpi x0 = create_size(B);
	mpi x1 = create_size(a.used - B);
	mpi t1 = create_size(a.used << 1);
	mpi t2 = create_size(a.used << 1);
	mpi x0x0 = create_size(B << 1);
	mpi x1x1 = create_size((a.used - B) << 1);

	size_t x = 0;
	mpi_digit* dst = nullptr;
	const mpi_digit* src = nullptr;

	src = a.dp;
	dst = x0.dp;
	for (x = 0; x < B; x++)
	{
		*dst++ = *src++;
	}

	dst = x1.dp;
	for (x = B; x < a.used; x++)
	{
		*dst++ = *src++;
	}

	x0.used = B;
	x1.used = a.used - B;
	x0.clamp();

	sqr(x0, x0x0); /* x_0x_0 = x_0 \times x_0 */
	sqr(x1, x1x1); /* x_1x_1 = x_1 \times x_1 */

	/* t_1 = (x_1 + x_0)^2 */
	abs_add(x1, x0, t1); /* t_1 =  x_0 + x_1 */
	sqr(t1, t1);         /* t_1 = (x_0 + x_1)^2 */

	/* t_1 = (x_1 + x_0)^2 - (x_0x_0 + x_1x_1) */
	abs_add(x0x0, x1x1, t2); /* t_2 = x_0x_0 + x_1x_1 */
	abs_sub(t1, t2, t1);     /* t_1 = (x_1 + x_0)^2 - (x_0x_0 + x_1x_1) */

	t1.lsh_x(B);         /*    t_1 = (x_0x_0 + x_1x_1 - (x_1 - x_0) \times (x_1 - x_0)) << B */
	x1x1.lsh_x(B << 1);  /* x_1x_1 = x_1x_1 << 2B */

	/* 将每一部分加起来 */
	add(x0x0, t1, t1);
	add(t1, x1x1, b);
}
MPL_END