#include "mpi.h"
MPL_BEGIN
void mpi::pow(mpi const& a, mpi const& b, mpi& c)
{
	mpi::k_ray_expt(a, b, c);
}
MPL_END