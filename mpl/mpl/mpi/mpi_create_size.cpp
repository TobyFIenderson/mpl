#include "mpi.h"
MPL_BEGIN

mpi mpi::create_size(size_t size)
{
	size = size < MPI_MIN_PREC ? MPI_MIN_PREC : size;
	return mpi(1, size, 0, new mpi_digit[size]{});
}

MPL_END