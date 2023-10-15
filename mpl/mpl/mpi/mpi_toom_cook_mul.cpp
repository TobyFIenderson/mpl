#include "mpi.h"
#include <algorithm>
MPL_BEGIN

void mpi::toom_cook_mul(mpi const& a, mpi const& b, mpi& c)
{
	size_t B = std::min<size_t>(a.used, b.used) / 3; /* B 为 0 的话该算法也是可行的 */
	mpi a0 = create_size(B);
	mpi a1 = create_size(B);
	mpi a2 = create_size(a.used - 2 * B);
	mpi b0 = create_size(B);
	mpi b1 = create_size(B);
	mpi b2 = create_size(b.used - 2 * B);
	mpi s1, s2, t1;

	const mpi_digit* tmpa = a.dp;
	const mpi_digit* tmpb = b.dp;
	mpi_digit* tmpdp = a0.dp;
	for (size_t i = 0; i < B; i++)
	{
		*tmpdp++ = *tmpa++;
	}
	a0.used = B;
	a0.clamp();

	tmpdp = a1.dp;
	for (size_t i = 0; i < B; i++)
	{
		*tmpdp++ = *tmpa++;
	}
	a1.used = B;
	a1.clamp();

	tmpdp = a2.dp;
	for (size_t i = 2 * B; i < a.used; i++)
	{
		*tmpdp++ = *tmpa++;
	}
	a2.used = a.used - 2 * B;
	a2.clamp();

	tmpdp = b0.dp;
	for (size_t i = 0; i < B; i++)
	{
		*tmpdp++ = *tmpb++;
	}
	b0.used = B;
	b0.clamp();

	tmpdp = b1.dp;
	for (size_t i = 0; i < B; i++)
	{
		*tmpdp++ = *tmpb++;
	}
	b1.used = B;
	b1.clamp();

	tmpdp = b2.dp;
	for (size_t i = 2 * B; i < b.used; i++)
	{
		*tmpdp++ = *tmpb++;
	}
	b2.used = b.used - 2 * B;
	b2.clamp();

	/* s_1 = (a_2 + a_1 + a_0) \times (b_2 + b_1 + b_0)  */
	add(a2, a1, t1);  /* t_1 = a_2 + a_1 */
	add(t1, a0, s2);  /* s_2 = t_1 + a_0 */
	add(b2, b1, c);   /*   c = b_2 + b_1 */
	add(c, b0, s1);   /* s_1 =  c  + b_0 */
	mul(s1, s2, s1);  /* s_1 = s_1 \times s_2 */

	/* s_2 = (4 \times a_2 + 2 \times a_1 + a_0) \times (4 \times b_2 + 2 \times b_1 + b_0) */
	add(t1, a2, t1);  /* t_1 = t_1 + a_2 */
	t1.lsh(1);        /* t_1 = t_1 << 1  */
	add(t1, a0, t1);  /* t_1 = t_1 + a_0 */
	add(c, b2, c);    /*   c =  c  + b_2 */
	c.lsh(1);         /*   c =  c  << 1  */
	add(c, b0, c);    /*   c =  c  + b_0 */
	mul(t1, c, s2);   /* s_2 = t_1 \times c */

	/* s_3 = (a_2 - a_1 + a_0) \times (b_2 - b_1 + b_0) */
	sub(a2, a1, a1);  /* a_1 = a_2 - a_1 */
	add(a1, a0, a1);  /* a_1 = a_1 + a_0 */
	sub(b2, b1, b1);  /* b_1 = b_2 - b_1 */
	add(b1, b0, b1);  /* b_1 = b_1 + b_0 */
	mul(a1, b1, a1);  /* a_1 = a_1 \times b_1 */

	mul(a2, b2, b1);  /* b_1 = a_2 \times b_2 */

	/* s_2 = (s_2 - s_3) \div 3 */
	sub(s2, a1, s2);         /* s_2 = s_2 - a_1 */
	div_3(s2, &s2, nullptr); /* s_2 = s_2 \div 3 */

	sub(s1, a1, a1);  /* a_1 = s_1 - a_1 */
	a1.rsh(1);        /* a_1 = a_1 >> 1  */
	mul(a0, b0, a0);  /* a_0 = a_0 \times b_0 */
	sub(s1, a0, s1);  /* s_1 = s_1 - a_0 */
	sub(s2, s1, s2);  /* s_2 = s_2 - s_1 */
	s2.rsh(1);        /* s_2 = s_1 >> 1  */
	sub(s1, a1, s1);  /* s_1 = s_1 - a_1 */
	sub(s1, b1, s1);  /* s_1 = s_1 - b_1 */
	t1 = b1; t1.lsh(1);  /* t_1 = b_1 << 1 */
	sub(s2, t1, s2);  /* s_2 = s_2 - t_1 */
	sub(a1, s2, a1);  /* a_1 = a_1 - s_2 */

	/* p = b_1 x^4 + s_2 x^3 + s_1 x^2 + a_1 x + a_0 */
	b1.lsh_x(4 * B);
	s2.lsh_x(3 * B);
	add(b1, s2, b1);
	s1.lsh_x(2 * B);
	add(b1, s1, b1);
	a1.lsh_x(1 * B);
	add(b1, a1, b1);
	add(b1, a0, c);
}

MPL_END