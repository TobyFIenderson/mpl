#pragma once
#ifndef __INCLUDE_MPF_RADIX_H__
#define __INCLUDE_MPF_RADIX_H__

#include "../mpi/mpi.h"

MPL_BEGIN
inline constexpr int32_t MPF_MAX_ZERO_AFTER_DECIMAL = 3;
inline constexpr char MPF_DECIMAL_SEPARATOR = '.';
inline constexpr uint32_t MPF_MAX_PRECISION = 1000000; /* ���֧��һ����λ���ȵ���� */
inline constexpr uint32_t MPF_RADIX_CONVERT_CUTOFF = 1000;
inline constexpr int32_t MPF_RADIX_EXTRA_PRECISION = 10;

/* ���ڿ��ٸ���Ҷ�任 */
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
	/* �����������
	ע�⣬�ڸ����������ʾ����������Ļ���
	�����ÿ�ѧ������������ 0.000000003 --> 3e-9
	���о��ǣ����� 1.00001 ^ 4096 �������̫С
	�õ��Ľ���һ������������Ȼ�Ǵ���ġ����ʱ��
	ҲҪ���ÿ�ѧ����������ʾ�û������Ǵ�ʱ������
	���Ǿ�ȷ��ֵ��������Ԥ�辫��֮��Ľ���� */
	flo,
	sci,  /* ��ѧ������ a * base ^ exp �� 1 <= |a| < base */
	eng
};


/* mpf_radix ����Ҫ���ڽ���ת�����Է�������Ĺ��ܣ�
δ�����ܻῼ������ʵ������࣬�����Ż�Ӧ�û��ڴ��
����֮�������Կ����ˡ�
����ʹ��FFT���ٽ���ת�����̣��Դ˷������ */
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

	/* ��֧��������ת������ exp < 0 ���׳��쳣 */
	mpf_radix to_radix(mpi_digit output_radix, int32_t precision) const;


/* private: */public:
	int32_t sign;
	std::vector<mpi_digit> mant;
	uint32_t radix;
	int32_t exp;
};

MPL_END
#endif // !__INCLUDE_MPF_RADIX_H__

