#include "mpi.h"
MPL_BEGIN

void mpi::sqrt(mpi const& a, mpi& b)
{
	if (a.sign == -1)
	{
		throw MPL_ERR_INVALID_VAL;
	}
	if (a.is_zero())
	{
		b.set_d(0);
		return;
	}
	mpi t1 = a;
	mpi t2;

	t1.rsh_x(t1.used / 2);
	div(a, t1, &t2, nullptr);
	add(t1, t2, t1);
	t1.rsh(1);
	do
	{
		div(a, t1, &t2, nullptr);
		add(t1, t2, t1);
		t1.rsh(1);
	} while (abs_cmp(t1, t2) == ord::gt);

	b.swap(t1);
}

MPL_END