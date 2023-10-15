#include "mpi.h"
MPL_BEGIN
void mpi::shrink()
{
	mpi(*this).swap(*this);
}
MPL_END