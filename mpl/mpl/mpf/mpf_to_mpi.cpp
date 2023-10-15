#include "mpf.h"
MPL_BEGIN
mpi mpf::to_mpi() const
{
	mpi r = mant;
	if (exp > 0)
	{
		r.lsh_x(exp);
	}
	else
	{
		r.rsh_x(-exp);
	}
	return r;
}
MPL_END