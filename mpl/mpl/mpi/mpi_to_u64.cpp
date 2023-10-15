#include "mpi.h"
MPL_BEGIN
uint64_t mpi::to_u64() const
{
	return uint64_t(to_i64());
}
MPL_END