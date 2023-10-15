#include "mpi.h"
MPL_BEGIN
bool mpi::is_zero() const
{
	return used == 0;
}
MPL_END