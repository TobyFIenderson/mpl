#include "mpi.h"
MPL_BEGIN

mpi& mpi::operator=(uint32_t n)
{
	size_t i = 0;
	while (n != 0)
	{
		dp[i++] = mpi_digit(n) & MPI_MASK;
		if constexpr (sizeof(uint32_t) * CHAR_BIT <= MPI_DIGIT_BIT)
		{
			break;
		}
		n >>= MPI_DIGIT_BIT;
	}
	used = i;
	sign = 1;
	MPI_ZERO_DIGITS(dp + used, alloc - used);
	return *this;
}

mpi& mpi::operator=(uint64_t n)
{
	size_t i = 0;
	while (n != 0)
	{
		dp[i++] = mpi_digit(n) & MPI_MASK;
		if constexpr (sizeof(uint64_t) * CHAR_BIT <= MPI_DIGIT_BIT)
		{
			break;
		}
		n >>= MPI_DIGIT_BIT;
	}
	used = i;
	sign = 1;
	MPI_ZERO_DIGITS(dp + used, alloc - used);
	return *this;
}

mpi& mpi::operator=(int32_t n)
{
	this->operator=(uint32_t(n < 0 ? -n : n));
	sign = n < 0 ? -1 : 1;
	return *this;
}

mpi& mpi::operator=(int64_t n)
{
	this->operator=(uint64_t(n < 0 ? -n : n));
	sign = n < 0 ? -1 : 1;
	return *this;
}

mpi& mpi::operator=(mpi const& m)
{
	if (this == &m)
	{
		return *this;
	}
	grow(m.used);
	const mpi_digit* tmpa = m.dp;
	mpi_digit* tmpb = dp;
	for (size_t i = 0; i < m.used; i++)
	{
		*tmpb++ = *tmpa++;
	}
	MPI_ZERO_DIGITS(tmpb, used - m.used);
	used = m.used;
	sign = m.sign;
	return *this;
}

mpi& mpi::operator=(mpi&& m) noexcept
{
	swap(m);
	return *this;
}

mpi& mpi::operator=(init_struct const& m)
{
	grow(m.used);
	const mpi_digit* tmpa = m.dp;
	mpi_digit* tmpb = dp;
	for (size_t i = 0; i < m.used; i++)
	{
		*tmpb++ = *tmpa++;
	}
	MPI_ZERO_DIGITS(tmpb, used - m.used);
	used = m.used;
	sign = m.sign;
	return *this;
}

bool operator>(mpi const& a, mpi const& b)
{
	return mpi::cmp(a, b) == ord::gt;
}

bool operator<(mpi const& a, mpi const& b)
{
	return mpi::cmp(a, b) == ord::lt;
}

bool operator==(mpi const& a, mpi const& b)
{
	return mpi::cmp(a, b) == ord::eq;
}

bool operator>=(mpi const& a, mpi const& b)
{
	return mpi::cmp(a, b) != ord::lt;
}

bool operator<=(mpi const& a, mpi const& b)
{
	return mpi::cmp(a, b) != ord::gt;
}

std::ostream& operator<<(std::ostream& os, mpi const& a)
{
	os << a.to_str(10);
	return os;
}
MPL_END