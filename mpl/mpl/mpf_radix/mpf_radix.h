#pragma once
#ifndef __INCLUDE_MPF_RADIX_H__
#define __INCLUDE_MPF_RADIX_H__

#include "../mpi/mpi.h"

MPL_BEGIN
inline constexpr int32_t MPF_MAX_ZERO_AFTER_DECIMAL = 3;
inline constexpr char MPF_DECIMAL_SEPARATOR = '.';
inline constexpr uint32_t MPF_MAX_PRECISION = 1000000; /* 最大支持一百万位精度的输出 */
inline constexpr uint32_t MPF_RADIX_CONVERT_CUTOFF = 1000;
inline constexpr int32_t MPF_RADIX_EXTRA_PRECISION = 10;

/* 用于快速傅里叶变换 */
struct double_complex
{
	double x, y;
	double_complex(double real = 0.0, double imag = 0.0) :x(real), y(imag) {}
	inline double_complex operator+(double_complex const& other) const
	{
		return double_complex(x + other.x, y + other.y);
	}

	inline double_complex operator-(double_complex const& other) const
	{
		return double_complex(x - other.x, y - other.y);
	}

	inline double_complex operator*(double_complex const& other) const
	{
		return double_complex(x * other.x - y * other.y, x * other.y + y * other.x);
	}
};

enum class num_format
{
	/* 浮点计数法，
	注意，在浮点计数法表示下如果过长的话，
	将采用科学计数法，比如 0.000000003 --> 3e-9
	还有就是，比如 1.00001 ^ 4096 如果精度太小
	得到的将是一个整数，这显然是错误的。这个时候
	也要采用科学计数法来提示用户，他们此时看到的
	不是精确的值而是舍入预设精度之后的结果。 */
	flo,
	sci,  /* 科学计数法 a * base ^ exp ， 1 <= |a| < base */
	eng
};


/* mpf_radix 类主要用于进制转换，以方便输出的功能，
未来可能会考虑重新实现这个类，不过优化应该会在答辩
结束之后再予以考虑了。
此类使用FFT加速进制转换过程，以此方便输出 */
class mpf_radix
{
public:

	mpf_radix(uint32_t d, uint32_t radix);
	mpf_radix(mpf_radix const& m);
	mpf_radix(mpf_radix&& m) noexcept;
	mpf_radix(int32_t sign, std::vector<mpi_digit>const& mant, uint32_t radix, int32_t exp);
	mpf_radix(int32_t sign, std::vector<mpi_digit>&& mant, uint32_t radix, int32_t exp) noexcept;

	mpf_radix& operator=(mpf_radix const& m);
	mpf_radix& operator=(mpf_radix&& m) noexcept;

	void set_d(uint32_t d);
	void abs_add(mpf_radix const& num, int32_t precision);
	void add_1();
	void mul_2();
	void mul(mpf_radix const& num, int32_t precision, double_complex* a, int* rev);
	void pow_u32(uint32_t pow, int32_t precision, double_complex* a, int* rev);
	void trimit(int32_t precision, bool round = false);
	void clamp();
	void print() const;
	std::string to_str(num_format fmt, int32_t precision) const;
	std::string to_sci_str() const;
	std::string to_flo_str() const;

	/* 仅支持整数的转换，若 exp < 0 将抛出异常 */
	mpf_radix to_radix(mpi_digit output_radix, int32_t precision) const;


/* private: */public:
	int32_t sign;
	std::vector<mpi_digit> mant;
	uint32_t radix;
	int32_t exp;
};

MPL_END
#endif // !__INCLUDE_MPF_RADIX_H__

