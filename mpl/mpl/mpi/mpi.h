#pragma once
#ifndef __INCLUDE_MPI_H__
#define __INCLUDE_MPI_H__

#include <stdint.h>
#include <limits.h>
#include <string>
#include <string_view>
#include <iostream>
#include <vector>

#include "../mpl_err.h"
#include "../mpl_ns.h"

MPL_BEGIN

using mpi_digit = uint32_t;
using mpi_word = uint64_t;

enum class ord { eq, gt, lt };

inline constexpr uint32_t MPI_DIGIT_BIT = 28;
inline constexpr uint32_t MPI_WORD_BIT = sizeof(mpi_word) * CHAR_BIT;
inline constexpr mpi_digit MPI_BASE = mpi_digit(mpi_digit(1) << MPI_DIGIT_BIT);
inline constexpr mpi_digit MPI_MASK = MPI_BASE - mpi_digit(1);
inline constexpr uint32_t MPI_PREC = 32;
inline constexpr uint32_t MPI_MIN_PREC = (MPI_WORD_BIT + MPI_DIGIT_BIT - 1) / MPI_DIGIT_BIT;

inline constexpr uint32_t MPI_MAX_COMBA = 1u << (MPI_WORD_BIT - 2 * MPI_DIGIT_BIT);
inline constexpr uint32_t MPI_COMBA_ARRAY = MPI_MAX_COMBA << 1;

inline int32_t MPI_KARA_MUL_CUTOFF = 85;  /* ֱ�������ĸ��ָ��Ӧ������Ϊ������
										  ���� tune ������Ҫ��̬�ı���Щ������ֵ��Ѱ�Һ��ʵķָ�� */
inline int32_t MPI_KARA_SQR_CUTOFF = 160;
inline int32_t MPI_TOOM_MUL_CUTOFF = 110;
inline int32_t MPI_TOOM_SQR_CUTOFF = 265;

inline constexpr const char* MPI_DIGITS_STR = "0123456789ABCDEF";
inline static char to_upper(char c)
{
	return c >= 'a' && c <= 'z' ? c - 'a' + 'A' : c;
}
inline static bool is_decnum(char c)
{
	return c >= '0' && c <= '9';
}
inline static bool is_hexnum(char c)
{
	return (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f');
}
inline static unsigned int c_to_u(char c)
{
	c = to_upper(c);
	return is_decnum(c) ? c - '0' : c - 'A' + 10;
}

/* ��һ���ڴ�ռ��ÿһ digit ����Ϊ 0 ������ʹ�� memset �������档 */
#define MPI_ZERO_DIGITS(mem, digits)												\
	do																				\
	{																				\
		int64_t __unname_zdigits = (digits);										\
		mpi_digit* __unname_zmem = (mem);										    \
		while (__unname_zdigits-- > 0)												\
		{																			\
			*__unname_zmem++ = 0;													\
		}																			\
	} while (0)

class mpi
{
public:


#pragma warning(push)
#pragma warning(disable:4200)
	struct init_struct
	{
		int32_t sign;
		size_t used;
		mpi_digit dp[];
	};
#pragma warning(pop)


	~mpi();
	mpi();
	mpi(uint32_t   n);
	mpi(uint64_t   n);
	mpi(int32_t    n);
	mpi(int64_t    n);
	mpi(mpi const& m);
	mpi(mpi&& m) noexcept;
	mpi(init_struct const& m);
	mpi(std::string_view str, uint32_t radix = 10);
	mpi(const char* str, uint32_t radix = 10); /* Ϊ�˼��� mpi a = "1234556678" �������﷨ */
	mpi(int32_t sign, size_t alloc, size_t used, mpi_digit* dp);

	mpi& operator=(uint32_t   n);
	mpi& operator=(uint64_t   n);
	mpi& operator=(int32_t    n);
	mpi& operator=(int64_t    n);
	mpi& operator=(mpi const& m);
	mpi& operator=(mpi&& m) noexcept;
	mpi& operator=(init_struct const& m);

	friend std::ostream& operator<<(std::ostream& os, mpi const& a);

	/* �� MPI_BASE ���Ƶ���ת��Ϊ radix ���Ƶ���������������λ */
	std::vector<mpi_digit> to_radix(uint32_t radix) const;

	static mpi str_to_mpi(std::string_view str, uint32_t radix = 10);
	std::string to_str(uint32_t radix = 10) const;
	int32_t  to_i32() const;
	int64_t  to_i64() const;
	uint32_t to_u32() const;
	uint64_t to_u64() const;


	void swap(mpi& m);

	bool is_abs_one() const;
	bool is_neg() const;
	bool is_zero() const;
	void is_neg(bool isneg);

	void set_d(mpi_digit d);
	void set_2expt(uint32_t d);

	/* �����ڴ��ʹ�ã�ʹ alloc = used */
	void shrink();

	static void srand(uint64_t seed = 0xdeadbeef);
	static void rand(mpi& a, size_t size);
	static mpi  rand(size_t size);

	static uint32_t get_radix_ratio(uint32_t radix);

	void output_constant(std::ostream& os, std::string_view constant_name, std::string_view prefix = "");

	/* ���� DeBug */
	void print(std::string_view str = "") const;


/*private:*/public:

	void grow(size_t size);
	void clamp();
	void incr();
	void decr();
	/* ���� d λ��ע�������λ����Ļ������� 2 ������ basex �� */
	void lsh_x(uint64_t d);
	/* ���� d λ��ע�������λ����Ļ������� 2 ������ basex �� */
	void rsh_x(uint64_t d);
	/* �����Ƿ���λ�����ƣ��Ϳ��Ƿ���λûʲô���� */
	void lsh(uint64_t d);
	/* �����Ƿ���λ������ */
	void rsh(uint64_t d);
	/* �з������� */
	void signed_rsh(uint64_t d);

	/* ���ﲻ���� uint32_t ���� uint64_t
	ֻҪ��������װ�� 32 bit ���� 64 bit �����ݽṹ���� */
	uint32_t get_mag_32() const;
	uint64_t get_mag_64() const;

	/* �����ж��ٶ�����λ */
	uint64_t cnt_bits() const;
	/* ����ĩβ������ 0 �ĸ��� */
	uint64_t cnt_lsb() const;

	static void add(mpi const& a, mpi const& b, mpi& c);
	static void sub(mpi const& a, mpi const& b, mpi& c);
	static void mul(mpi const& a, mpi const& b, mpi& c);
	static void div(mpi const& a, mpi const& b, mpi* c, mpi* d);
	static void mod(mpi const& a, mpi const& b, mpi& c);
	static void pow(mpi const& a, mpi const& b, mpi& c);

	static void pow_u32(mpi const& a, uint32_t b, mpi& c);
	static void root_u32(mpi const& a, uint32_t b, mpi& c);

	static void gcd(mpi const& a, mpi const& b, mpi& c);
	static void lcm(mpi const& a, mpi const& b, mpi& c);

	static void sqr(mpi const& a, mpi& b);
	static void sqrt(mpi const& a, mpi& b); /* Ϊ root_u32 ��һ��������ʽ */
	static void cubet(mpi const& a, mpi& b); /* Ϊ root_u32 ��һ��������ʽ */

	static void add_d(mpi const& a, mpi_digit b, mpi& c);
	static void sub_d(mpi const& a, mpi_digit b, mpi& c);
	static void mul_d(mpi const& a, mpi_digit b, mpi& c);
	static void div_d(mpi const& a, mpi_digit b, mpi* c, mpi_digit* d);

	static ord abs_cmp(mpi const& a, mpi const& b);
	static ord cmp(mpi const& a, mpi const& b);

	static void abs_add(mpi const& a, mpi const& b, mpi& c);
	/* c = |a| - |b|�����ô˺���ʱ�������� |a| >= |b| */
	static void abs_sub(mpi const& a, mpi const& b, mpi& c);


	/* �Զ����ƵĲ���������� tc ---> two complement */
	static void and_tc(mpi const& a, mpi const& b, mpi& c);
	static void or_tc(mpi const& a, mpi const& b, mpi& c);
	static void xor_tc(mpi const& a, mpi const& b, mpi& c);
	static void not_tc(mpi const& a, mpi& c);
	static void two_complement(mpi const& a, mpi& c);

	static void baseline_mul(mpi const& a, mpi const& b, mpi& c);
	static void comba_mul(mpi const& a, mpi const& b, mpi& c);
	static void karatsuba_mul(mpi const& a, mpi const& b, mpi& c);
	static void toom_cook_mul(mpi const& a, mpi const& b, mpi& c);

	static void div_3(mpi const& a, mpi* c, mpi_digit* d);

	static void baseline_sqr(mpi const& a, mpi& b);
	static void comba_sqr(mpi const& a, mpi& b);
	static void karatsuba_sqr(mpi const& a, mpi& b);
	static void toom_cook_sqr(mpi const& a, mpi& b);

	static void k_ray_expt(mpi const& a, mpi const& b, mpi& c);
	static void expt_u32(mpi const& a, uint32_t b, mpi& c);
	static void fact(uint32_t a, mpi& b);

	static mpi create_size(size_t size);

/*private:*/public:

	int32_t sign;
	size_t alloc;
	size_t used;
	mpi_digit* dp;
};

MPL_END
#endif // !__INCLUDE_MPI_H__