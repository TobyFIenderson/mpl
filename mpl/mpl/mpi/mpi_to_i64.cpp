#include "mpi.h"
MPL_BEGIN
int64_t mpi::to_i64() const
{
	uint64_t r = get_mag_64();
	return sign == -1 ? -int64_t(r) : int64_t(r);
}
MPL_END