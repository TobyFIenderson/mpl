#include "mpf_radix.h"
MPL_BEGIN
mpf_radix::mpf_radix(uint32_t d, uint32_t radix) :
	sign(1), mant{}, radix(radix), exp(0) {set_d(d); }
mpf_radix::mpf_radix(mpf_radix const& m) :
	sign(m.sign), mant(m.mant), radix(m.radix), exp(m.exp) {}
mpf_radix::mpf_radix(mpf_radix&& m) noexcept :
	sign(m.sign), mant(std::move(m.mant)), radix(m.radix), exp(m.exp) {}
mpf_radix::mpf_radix(int32_t sign, std::vector<mpi_digit>const& mant, uint32_t radix, int32_t exp) :
	sign(sign), mant(mant), radix(radix), exp(exp) {}
mpf_radix::mpf_radix(int32_t sign, std::vector<mpi_digit>&& mant, uint32_t radix, int32_t exp) noexcept :
	sign(sign), mant(std::move(mant)), radix(radix), exp(exp) {}
MPL_END