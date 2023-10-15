#include "mpi.h"
MPL_BEGIN

void mpi::grow(size_t size)
{
	if (size <= alloc)
	{
		return;
	}

	mpi_digit* newdp = new mpi_digit[size]{};
	mpi_digit* tmpnewdp = newdp;
	const mpi_digit* olddp = dp;
	for (size_t i = 0; i < used; i++)
	{
		*tmpnewdp++ = *olddp++;
	}

	delete[] dp;
	dp = newdp;
	alloc = size;
}

MPL_END