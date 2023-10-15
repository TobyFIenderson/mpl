#include "mpi.h"
MPL_BEGIN

ord mpi::cmp(mpi const& a, mpi const& b)
{
	return a.sign != b.sign ?
		(a.sign == -1 ? ord::lt : ord::gt) :
		(a.sign == -1 ? abs_cmp(b, a) : abs_cmp(a, b));
}

MPL_END