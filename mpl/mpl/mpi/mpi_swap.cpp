#include "mpi.h"
MPL_BEGIN
void mpi::swap(mpi& m)
{
	int32_t tmpsign = sign;
	size_t tmpalloc = alloc;
	size_t tmpused = used;
	mpi_digit* tmpdp = dp;
	sign = m.sign;
	alloc = m.alloc;
	used = m.used;
	dp = m.dp;
	m.sign = tmpsign;
	m.alloc = tmpalloc;
	m.used = tmpused;
	m.dp = tmpdp;
}
MPL_END