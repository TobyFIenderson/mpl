#include "mpf.h"
MPL_BEGIN

mpf& mpf::operator=(uint32_t n)
{
	mant = n;
	exp = 0;
	return *this;
}

mpf& mpf::operator=(uint64_t n)
{
	mant = n;
	exp = 0;
	return *this;
}

mpf& mpf::operator=(int32_t n)
{
	mant = n;
	exp = 0;
	return *this;
}

mpf& mpf::operator=(int64_t n)
{
	mant = n;
	exp = 0;
	return *this;
}

mpf& mpf::operator=(mpf const& m)
{
	if (this != &m)
	{
		mant = m.mant;
		exp = m.exp;
	}

	return *this;
}

mpf& mpf::operator=(mpf&& m) noexcept
{
	mant = std::move(m.mant);
	exp = m.exp;
	return *this;
}

mpf& mpf::operator=(mpf::init_struct const& m)
{
	mant = m.mant;
	exp = m.exp;
	return *this;
}

mpf& mpf::operator=(double d)
{
	*this = mpf(d);
	return *this;
}

bool operator>(mpf const& a, mpf const& b)
{
	return mpf::cmp(a, b) == ord::gt;
}

bool operator<=(mpf const& a, mpf const& b)
{
	return mpf::cmp(a, b) != ord::gt;
}

bool operator>=(mpf const& a, mpf const& b)
{
	return mpf::cmp(a, b) != ord::lt;
}

bool operator==(mpf const& a, mpf const& b)
{
	return mpf::cmp(a, b) == ord::eq;
}

bool operator<(mpf const& a, mpf const& b)
{
	return mpf::cmp(a, b) == ord::lt;
}

std::ostream& operator<<(std::ostream& os, mpf const& a)
{
	double radix_ratio = log10(MPI_BASE);
	int32_t precision = MPF_ORIGINAL_PRECISION * int32_t(radix_ratio);
	os << a.to_str(10, num_format::flo, precision);
	return os;
}

MPL_END