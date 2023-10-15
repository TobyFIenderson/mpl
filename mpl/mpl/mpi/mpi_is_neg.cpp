#include "mpi.h"
MPL_BEGIN
bool mpi::is_neg() const
{
	return sign == -1;
}
void mpi::is_neg(bool isneg)
{
	sign = isneg ? -1 : 1;
}
MPL_END