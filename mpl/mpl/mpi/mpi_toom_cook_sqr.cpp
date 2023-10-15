#include "mpi.h"
MPL_BEGIN
void mpi::toom_cook_sqr(mpi const& a, mpi& b)
{
	size_t B = a.used / 3;
	mpi a0 = create_size(B);
	mpi a1 = create_size(B);
	mpi a2 = create_size(B + (a.used - 3 * B)); /* 可以保证至少为 1B */
	mpi s;

	a0.used = B;
	a1.used = B;

	size_t i = 0;
	const mpi_digit* tmpa = a.dp;
	mpi_digit* tmpc = a0.dp;
	for (i = 0; i < B; i++)
	{
		*tmpc++ = *tmpa++;
	}
	tmpc = a1.dp;
	for (; i < (2 * B); i++)
	{
		*tmpc++ = *tmpa++;
	}
	tmpc = a2.dp;
	for (; i < a.used; i++)
	{
		*tmpc++ = *tmpa++;
		a2.used++;
	}
	a0.clamp();
	a1.clamp();
	a2.clamp();

	/* 具体注释可查看 toom_cook_mul() */
	sqr(a0, s);

	add(a0, a2, a0);
	sub(a0, a1, b);
	add(a0, a1, a0);
	sqr(a0, a0);
	sqr(b, b);

	mul(a1, a2, a1);
	a1.lsh(1);
	sqr(a2, a2);

	add(a0, b, b);
	b.rsh(1);

	sub(a0, b, a0);
	sub(a0, a1, a0);

	sub(b, a2, b);
	sub(b, s, b);

	a2.lsh_x(4 * B);
	a1.lsh_x(3 * B);
	b.lsh_x(2 * B);
	a0.lsh_x(1 * B);
	add(a2, a1, a2);
	add(a2, b, b);
	add(b, a0, b);
	add(b, s, b);
}
MPL_END