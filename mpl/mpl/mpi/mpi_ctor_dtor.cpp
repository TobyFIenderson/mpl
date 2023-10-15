#include "mpi.h"
MPL_BEGIN

mpi::~mpi()
{
	sign = 1;
	alloc = 0;
	used = 0;
	if (dp != nullptr)
	{
		delete[] dp;
		dp = nullptr;
	}
}

mpi::mpi() :
	sign(1),
	alloc(MPI_PREC),
	used(0),
	dp(new mpi_digit[MPI_PREC]{})
{}

mpi::mpi(uint32_t n) :
	sign(1),
	alloc(MPI_PREC),
	used(0),
	dp(new mpi_digit[MPI_PREC]{})
{
	while (n != 0)
	{
		dp[used++] = mpi_digit(n) & MPI_MASK;
		if constexpr (sizeof(uint32_t) * CHAR_BIT <= MPI_DIGIT_BIT)
		{
			break;
		}
		n >>= MPI_DIGIT_BIT;
	}
}

mpi::mpi(uint64_t n) :
	sign(1),
	alloc(MPI_PREC),
	used(0),
	dp(new mpi_digit[MPI_PREC]{})
{
	while (n != 0)
	{
		dp[used++] = mpi_digit(n) & MPI_MASK;
		if constexpr (sizeof(uint64_t) * CHAR_BIT <= MPI_DIGIT_BIT)
		{
			break;
		}
		n >>= MPI_DIGIT_BIT;
	}
}

mpi::mpi(int32_t n) :
	sign(n >= 0 ? 1 : -1),
	alloc(MPI_PREC),
	used(0),
	dp(new mpi_digit[MPI_PREC]{})
{
	if (n < 0)
	{
		n *= -1;
	}

	while (n != 0)
	{
		dp[used++] = mpi_digit(n) & MPI_MASK;
		if constexpr (sizeof(int32_t) * CHAR_BIT <= MPI_DIGIT_BIT)
		{
			break;
		}
		n >>= MPI_DIGIT_BIT;
	}
}

mpi::mpi(int64_t n) :
	sign(n >= 0 ? 1 : -1),
	alloc(MPI_PREC),
	used(0),
	dp(new mpi_digit[MPI_PREC]{})
{
	if (n < 0)
	{
		n *= -1;
	}

	while (n != 0)
	{
		dp[used++] = mpi_digit(n) & MPI_MASK;
		if constexpr (sizeof(int64_t) * CHAR_BIT <= MPI_DIGIT_BIT)
		{
			break;
		}
		n >>= MPI_DIGIT_BIT;
	}
}

mpi::mpi(mpi const& m) :
	sign(m.sign),
	alloc(m.used < MPI_PREC ? MPI_PREC : m.used),
	used(m.used),
	dp(new mpi_digit[m.used < MPI_PREC ? MPI_PREC : m.used]{})
{
	mpi_digit* tmpa = dp;
	mpi_digit* tmpb = m.dp;
	for (size_t i = 0; i < used; i++)
	{
		*tmpa++ = *tmpb++;
	}
}

mpi::mpi(mpi&& m) noexcept :
	sign(1),
	alloc(0),
	used(0),
	dp(nullptr)
{
	swap(m);
}

mpi::mpi(init_struct const& m) :
	sign(m.sign),
	alloc(m.used < MPI_PREC ? MPI_PREC : m.used),
	used(m.used),
	dp(new mpi_digit[m.used < MPI_PREC ? MPI_PREC : m.used]{})
{
	const mpi_digit* tmpa = m.dp;
	mpi_digit* tmpb = dp;
	for (size_t i = 0; i < m.used; i++)
	{
		*tmpb++ = *tmpa++;
	}
}

mpi::mpi(std::string_view str, uint32_t radix) :
	sign(1),
	alloc(0),
	used(0),
	dp(nullptr)
{
	*this = str_to_mpi(str, radix);
}

mpi::mpi(const char* str, uint32_t radix)
	:mpi(std::string_view(str), radix)
{}

mpi::mpi(int32_t sign, size_t alloc, size_t used, mpi_digit* dp) :
	sign(sign),
	alloc(alloc),
	used(used),
	dp(dp)
{}

MPL_END