#pragma once
#ifndef __INCLUDE_MPF_H__
#define __INCLUDE_MPF_H__

#include "../mpf_radix/mpf_radix.h"

MPL_BEGIN

inline int32_t MPF_EXTRA_PRECISION = 2;
inline int32_t MPF_ORIGINAL_PRECISION = 128;
#define MPF_PRECISION (MPF_ORIGINAL_PRECISION + MPF_EXTRA_PRECISION)

inline bool MPF_ALLOW_INFINITE_PRECISION = false;
inline int32_t MPF_RADIX_RATIO = 1;

enum class ang { deg, rad, grad };

class mpf
{
public:

	struct init_struct
	{
		int32_t exp;
		mpi::init_struct mant;
	};

	mpf();
	mpf(mpf&& m) noexcept;
	mpf(mpf const& m);
	mpf(const mpi& m, int32_t exp = 0);
	mpf(mpi&& m, int32_t exp = 0) noexcept;
	mpf(init_struct const& init);
	mpf(std::string_view str, uint32_t radix = 10);
	mpf(const char* str, uint32_t radix = 10);
	mpf(uint32_t n);
	mpf(uint64_t n);
	mpf(int32_t n);
	mpf(int64_t n);
	mpf(double d);

	/* to_str 的 precision 和 to_radix 的 precision 都是指输出的长度 */
	std::string to_str(uint32_t radix = 10, num_format format = num_format::flo, int32_t precision = 1000) const;
	mpf_radix to_radix(uint32_t radix, int32_t precision) const;
	static mpf str_to_mpf(std::string_view str, uint32_t radix = 10);
	static mpf double_to_mpf(double d);
	mpi to_mpi() const;

	/* 获取整数部分 */
	mpf integer() const;
	/* 获取真分数部分（小数部分） */
	mpf fraction() const;

	void output_constant(std::ostream& os, std::string_view constant_name, std::string_view prefix = "") const;

	bool is_neg() const;
	void is_neg(bool isneg);
	bool is_zero() const;
	bool is_abs_one() const;
	bool is_small_enough() const;
	bool is_ignorable() const;
	void swap(mpf& m);

	mpf& operator=(uint32_t n);
	mpf& operator=(uint64_t n);
	mpf& operator=(int32_t n);
	mpf& operator=(int64_t n);
	mpf& operator=(mpf const& m);
	mpf& operator=(mpf&& m) noexcept;
	mpf& operator=(mpf::init_struct const& m);
	mpf& operator=(double d);

	bool friend operator> (mpf const& a, mpf const& b);
	bool friend operator< (mpf const& a, mpf const& b);
	bool friend operator<=(mpf const& a, mpf const& b);
	bool friend operator>=(mpf const& a, mpf const& b);
	bool friend operator==(mpf const& a, mpf const& b);

	friend std::ostream& operator<<(std::ostream& os, mpf const& a);

	static void neg(mpf const& a, mpf& c);
	static void abs(mpf const& a, mpf& c);
	static void ceil(mpf const& a, mpf& c);
	static void floor(mpf const& a, mpf& c);

	static ord abs_cmp(mpf const& a, mpf const& b);
	static ord cmp(mpf const& a, mpf const& b);

	static void abs_add(mpf const& a, mpf const& b, mpf& c);
	static void abs_sub(mpf const& a, mpf const& b, mpf& c);
	static void add(mpf const& a, mpf const& b, mpf& c);
	static void sub(mpf const& a, mpf const& b, mpf& c);
	static void mul(mpf const& a, mpf const& b, mpf& c);
	static void div(mpf const& a, mpf const& b, mpf& c);
	static void mod(mpf const& a, mpf const& b, mpf& c);
	static void mod_2pi(mpf const& a, mpf& c);
	static void mod_pi(mpf const& a, mpf& c);
	static void pow(mpf const& a, mpf const& b, mpf& c);
	static void root(mpf const& a, mpf const& b, mpf& c);
	static void ln(mpf const& a, mpf& c);
	static void inv(mpf const& a, mpf& c);
	static void sqr(mpf const& a, mpf& c);
	static void sqrt(mpf const& a, mpf& c);
	static void cube(mpf const& a, mpf& c);
	static void cubet(mpf const& a, mpf& c);
	static void e_expt(mpf const& a, mpf& c);


	static void mul_2(mpf const& a, mpf& c);
	static void div_2(mpf const& a, mpf& c);
	static void div_3(mpf const& a, mpf& c);
	static void mul_2d(mpf const& a, mpi_digit d, mpf& c);
	static void div_2d(mpf const& a, mpi_digit d, mpf& c);
	static void mul_d(mpf const& a, mpi_digit d, mpf& c);
	static void div_d(mpf const& a, mpi_digit d, mpf& c);

	static void sin(mpf const& a, ang b, mpf& c);
	static void cos(mpf const& a, ang b, mpf& c);
	static void tan(mpf const& a, ang b, mpf& c);
	static void cot(mpf const& a, ang b, mpf& c);
	static void sec(mpf const& a, ang b, mpf& c);
	static void csc(mpf const& a, ang b, mpf& c);

	static void asin(mpf const& a, ang b, mpf& c);
	static void acos(mpf const& a, ang b, mpf& c);
	static void atan(mpf const& a, ang b, mpf& c);
	static void acot(mpf const& a, ang b, mpf& c);
	static void asec(mpf const& a, ang b, mpf& c);
	static void acsc(mpf const& a, ang b, mpf& c);

	static void sinh(mpf const& a, mpf& c);
	static void cosh(mpf const& a, mpf& c);
	static void tanh(mpf const& a, mpf& c);
	static void coth(mpf const& a, mpf& c);
	static void sech(mpf const& a, mpf& c);
	static void csch(mpf const& a, mpf& c);

	static void asinh(mpf const& a, mpf& c);
	static void acosh(mpf const& a, mpf& c);
	static void atanh(mpf const& a, mpf& c);
	static void acoth(mpf const& a, mpf& c);
	static void asech(mpf const& a, mpf& c);
	static void acsch(mpf const& a, mpf& c);

	/* 使用泰勒展开计算 e^{x} 效率很低，调用时应确保 0 < c < 1 */
	static void e_expt_taylor(mpf const& a, mpf& c);
	/* ln(x) ,  1 <= x < 2  */
	static void ln_taylor(mpf const& a, mpf& c);

	/* 注意 sin_taylot 和 cos_taylor 函数中为了效率将 n 用 mpi_digit 类型表示了，
	迭代次数过多可能会造成溢出最大可以满足约 4000 次迭代计算*/
	static void sin_taylor(mpf const& a, mpf& c);
	static void cos_taylor(mpf const& a, mpf& c);

	static void asin_taylor(mpf const& a, mpf& c);
	static void atan_taylor(mpf const& a, mpf& c);

	static void sinh_taylor(mpf const& a, mpf& c);
	static void cosh_taylor(mpf const& a, mpf& c);

	static void asinh_taylor(mpf const& a, mpf& c);

	static void expt_u32(mpf const& a, uint32_t b, mpf& c);
	
	/* 注意这里的第二个参数的类型为 mpi */
	static void k_ray_expt(mpf const& a, mpi const& b, mpf& c);

	static void pow_mpi(mpf const& a, mpf const& b, mpf& c);

	/* 用于计算广义阶乘，不过这是未完成的版本，不要调用 */
	static void gamma_taylor(mpf const& x, mpf& y);

	static void fact(uint32_t a, mpf& c);

	void set_d(mpi_digit d);
	void set_2expt(uint32_t d);

	int32_t rdig() const;

	/* 切割至 MPF_PRECISION 精度 */
	void trimit();

	uint32_t cnt_tail_zero() const;

	/* 此函数语义和 mpi::clamp() 不一样，mpi::clamp() 是去除前导零，
	而这个是去除尾随的 0 ，例如 [0, 0, 1, 2, 3] exp = 0 ---> [1, 2, 3] exp = 2 */
	void clamp_tail();

	static void scale_ang(mpf const& a, ang b, mpf& c);


	static mpf const& const_pi();
	static mpf const& const_e();
	static mpf const& const_sqrt2();
	static mpf const& const_ln2();
	static mpf const& const_ln10();

	static mpf const& const_2pi();
	static mpf const& const_half_pi();

	static mpf const& const_rtod_ratio();
	static mpf const& const_dtor_ratio();

	static mpf const& const_smallest();

	static void test();

/* private: */public:
	mpi mant;
	int32_t exp; // mpf = m_i * MPI_BASE ^ exp
};

MPL_END

#endif // !__INCLUDE_MPF_H__