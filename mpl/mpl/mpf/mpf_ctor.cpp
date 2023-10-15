#include "mpf.h"
MPL_BEGIN
mpf::mpf() : mant(), exp(0) {}
mpf::mpf(mpf&& m) noexcept : mant(std::move(m.mant)), exp(m.exp) {}
mpf::mpf(mpf const& m) : mant(m.mant), exp(m.exp) {}
mpf::mpf(mpi const& m, int32_t exp) : mant(m), exp(exp) { trimit(); }
mpf::mpf(mpi&& m, int32_t exp) noexcept : mant(std::move(m)), exp(exp) { trimit(); }
mpf::mpf(init_struct const& init) : exp(init.exp), mant(init.mant) {/* 为了确保常量的一些额外精度，这里不能 trimit() */ }
mpf::mpf(std::string_view str, uint32_t radix) : mpf(str_to_mpf(str, radix)) {}
mpf::mpf(const char* str, uint32_t radix) : mpf(str_to_mpf(str, radix)) {}
mpf::mpf(uint32_t n) : mant(n), exp(0) {}
mpf::mpf(uint64_t n) : mant(n), exp(0) {}
mpf::mpf(int32_t n) : mant(n), exp(0) {}
mpf::mpf(int64_t n) : mant(n), exp(0) {}
mpf::mpf(double d) : mpf(double_to_mpf(d)) {}
MPL_END