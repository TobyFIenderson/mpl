#include "mpi.h"
MPL_BEGIN
int32_t mpi::to_i32() const
{
	uint32_t r = get_mag_32();
	return sign == -1 ? -int32_t(r) : int32_t(r);
}
MPL_END