#include "mpi.h"
MPL_BEGIN
void mpi::mod(mpi const& a, mpi const& b, mpi& c)
{
	div(a, b, nullptr, &c);
}
MPL_END