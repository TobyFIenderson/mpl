#include "mpi.h"
MPL_BEGIN
void mpi::gcd(mpi const& a, mpi const& b, mpi& c)
{
	if (a.is_zero())
	{
		c = b;
		c.sign = 1;
		return;
	}
	if (b.is_zero())
	{
		c = a;
		c.sign = 1;
		return;
	}

	mpi u = a;
	mpi v = b;
	u.sign = v.sign = 1;

	size_t u_lsb = u.cnt_lsb();
	size_t v_lsb = v.cnt_lsb();
	size_t k = std::min(u_lsb, v_lsb);

	if (k > 0)
	{
		u.rsh(k);
		v.rsh(k);
	}

	if (u_lsb != k)
	{
		u.rsh(u_lsb - k);
	}

	if (v_lsb != k)
	{
		v.rsh(v_lsb - k);
	}

	while (!v.is_zero())
	{
		if (abs_cmp(u, v) == ord::gt)
		{
			u.swap(v);
		}

		abs_sub(v, u, v);
		v.rsh(v.cnt_lsb());
	}

	c = u;
	c.lsh(k);
	c.sign = 1;
}
MPL_END