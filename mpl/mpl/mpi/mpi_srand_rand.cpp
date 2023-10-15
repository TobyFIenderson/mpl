#include "mpi.h"
#include <random>
MPL_BEGIN

static std::mt19937 m;

void mpi::srand(uint64_t seed)
{
	m.seed(unsigned int(seed));
}

void mpi::rand(mpi& a, size_t size)
{
	a.grow(size);
	mpi_digit* tmpa = a.dp;
	for (size_t i = 0; i < size; i++)
	{
		*tmpa++ = mpi_digit(m()) & MPI_MASK;
	}
	MPI_ZERO_DIGITS(tmpa, a.used - size);
	a.used = size;
	a.sign = m() & 1 ? 1 : -1;
	a.clamp();
}

mpi mpi::rand(size_t size)
{
	mpi a = create_size(size);
	rand(a, size);
	return a;
}

MPL_END