#pragma once
#ifndef __INCLUDE_MPI_TEST_H__
#define __INCLUDE_MPI_TEST_H__

#include "../mpi/mpi.h"
#include <random>
#include <chrono>
#include <fstream>
#include <vector>
#include <string>
MPL_BEGIN
inline unsigned long long time_count = 0;
inline bool time_cout = true;
#define MPI_TEST_CLOCK_NOW std::chrono::system_clock::now().time_since_epoch().count()
#define TIME(d, str, dura)                                                                       \
	do                                                                                           \
	{                                                                                            \
		std::chrono::time_point t1 = std::chrono::steady_clock::now();                           \
		d;                                                                                       \
		std::chrono::time_point t2 = std::chrono::steady_clock::now();                           \
		if(time_cout)                                                                            \
		{                                                                                        \
			std::cout << #str << " " << std::chrono::duration_cast<dura>(t2 - t1) << std::endl;  \
		}                                                                                        \
		time_count = std::chrono::duration_cast<dura>(t2 - t1).count();                          \
	} while (0)

#define MILLI_TIME(d, str) TIME(d, str, std::chrono::milliseconds)
#define NANO_TIME(d, str) TIME(d, str, std::chrono::nanoseconds)

/*
* 对于一般情况（去除某些特殊情况，比如：1 + 9999，9999 * 9999），
* 高精度运算的结果是难以在一开始就知道结果的，因此要借助其他工具来检验算法的正确性
* 这里采用的工具是 Python ，Python 自带高精还是很好用的 _^^
*/
class mpi_test
{
public:
	/* 生成 n 个操作数，每个操作数的长度都为 opnd_length ，并且都会输出到 filename 文件中 */
	static std::vector<mpi> generate_mpi_to_file(int n, int opnd_length, std::string_view filepath, unsigned int seed);
	static std::string get_contents_from_file(std::string_view filepath);
	static void print_file(std::string_view filepath);
	/* 需要注意的是，每一次调用 output 函数都会清空 filepath 文件的内容！！！ */
	static void output_mpi_to_file(std::vector<mpi> const& vmpi, std::string_view filepath);
	static void output_str_to_file(std::vector<std::string> const& vs, std::string_view filepath);

	/* 测试与内存相关的操作，比如赋值，右值引用，构造和析构 */
	static void test_memory_operation(unsigned int seed);

	static void test_add(int n_sample, int opnd1_length, int opnd2_length, unsigned int seed);
	static void test_sub(int n_sample, int opnd1_length, int opnd2_length, unsigned int seed);
	static void test_mul(int n_sample, int opnd1_length, int opnd2_length, unsigned int seed);
	static void test_div(int n_sample, int opnd1_length, int opnd2_length, unsigned int seed);
	static void test_gcd(int n_sample, int opnd1_length, int opnd2_length, unsigned int seed);
	static void test_root(int n_sample, int opnd1_length, int opnd2_range, unsigned int seed);
	
	static void test_add_time(int n_sample, int opnd1_length, int opnd2_length, unsigned int seed);
	static void test_sub_time(int n_sample, int opnd1_length, int opnd2_length, unsigned int seed);
	static void test_mul_time(int n_sample, int opnd1_length, int opnd2_length, unsigned int seed);
	static void test_div_time(int n_sample, int opnd1_length, int opnd2_length, unsigned int seed);

	static void test_to_str(int n_sample, int opnd1_length, uint32_t radix, unsigned int seed);
	static void test_str_to_mpi(int n_sample, int str_length, uint32_t radix, unsigned int seed);

	/* 这里面的都是一些离散的测试例程过程 */
	static void test_discrete();

private:
};


MPL_END

#endif // !__INCLUDE_MPI_TEST_H__
