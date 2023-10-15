#include "mpi.h"
MPL_BEGIN
void mpi::cubet(mpi const& a, mpi& b)
{
	if (a.is_zero())
	{
		b.set_d(0);
		return;
	}

	mpi t1 = a, t2, tmp;
	t1.rsh_x(t1.used / 3 * 2);


	/* tmp = 2t1 */
	tmp = t1;
	tmp.lsh(1);

	/* t2 = a / t1^2 */
	mpi::sqr(t1, t2);
	mpi::div(a, t2, &t2, nullptr);

	mpi::add(t2, tmp, t2);
	mpi::div_3(t2, &t2, nullptr);

	do
	{
		t1 = t2;

		/* tmp = 2t1 */
		tmp = t1;
		tmp.lsh(1);

		/* t2 = a / t1^2 */
		mpi::sqr(t1, t2);
		mpi::div(a, t2, &t2, nullptr);

		mpi::add(t2, tmp, t2);
		mpi::div_3(t2, &t2, nullptr);
	} while (mpi::abs_cmp(t1, t2) == ord::gt);
	b.swap(t1);
}
MPL_END