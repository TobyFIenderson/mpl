#include "mpi.h"
MPL_BEGIN
uint32_t mpi::to_u32() const
{
	return uint32_t(to_i32());
}
MPL_END