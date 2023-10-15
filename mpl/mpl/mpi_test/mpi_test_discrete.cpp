#include "mpi_test.h"
#include <Windows.h>
MPL_BEGIN
using namespace std;
using millisec = chrono::milliseconds;
using microsec = chrono::microseconds;
using time_point = chrono::steady_clock::time_point;
#define dura_cast chrono::duration_cast
#define STEADY_CLOCK_NOW std::chrono::steady_clock::now()

static void base_fact(uint32_t a, mpi& b)
{
	b.set_d(1);
	for (uint32_t i = 1; i <= a; i++)
	{
		mpi::mul_d(b, i, b);
	}
}

void mpi_test::test_discrete()
{
	mpi_test::test_add(64, 1024, 1024, unsigned int(MPI_TEST_CLOCK_NOW));
	mpi_test::test_sub(64, 1024, 1024, unsigned int(MPI_TEST_CLOCK_NOW));
	mpi_test::test_mul(64, 1024, 1024, unsigned int(MPI_TEST_CLOCK_NOW));
	mpi_test::test_div(64, 2048, 1024, unsigned int(MPI_TEST_CLOCK_NOW));
	mpi_test::test_to_str(64, 1024, 16, unsigned int(MPI_TEST_CLOCK_NOW));
	mpi_test::test_to_str(64, 1024, 10, unsigned int(MPI_TEST_CLOCK_NOW));
	mpi_test::test_to_str(64, 1024, 8, unsigned int(MPI_TEST_CLOCK_NOW));
	mpi_test::test_to_str(64, 1024, 2, unsigned int(MPI_TEST_CLOCK_NOW));
	mpi_test::test_str_to_mpi(1024, 4096, 16, unsigned int(MPI_TEST_CLOCK_NOW));
	mpi_test::test_str_to_mpi(1024, 4096, 10, unsigned int(MPI_TEST_CLOCK_NOW));
	mpi_test::test_str_to_mpi(1024, 4096, 8, unsigned int(MPI_TEST_CLOCK_NOW));
	mpi_test::test_str_to_mpi(1024, 4096, 2, unsigned int(MPI_TEST_CLOCK_NOW));

	for (size_t i = 0; i < 4096; i++)
	{
		mpi a = mpi::rand(1024);
		mpi b = a;
		mpi::sqr(a, a);
		mpi::mul(b, b, b);
		cout << i + 1 << ". " << (mpi::cmp(a, b) == ord::eq ? "True" : "False") << endl;
	}

	for (size_t i = 0; i < 4096; i++)
	{
		mpi a = mpi::rand(10);
		mpi b = 1000;
		mpi c;
		mpi::k_ray_expt(a, b, c);
		mpi e;
		mpi::expt_u32(a, 1000, e);
		cout << i + 1 << ". " << (mpi::cmp(c, e) == ord::eq ? "True" : "False") << endl;
	}

	/* 可以看出 k-ray 比传统的快速幂要快上不少 */
	for (size_t i = 0; i < 4096; i++)
	{
		using ms = millisec;
		uint32_t p = 2022;
		mpi a = mpi::rand(32);
		mpi b = p;
		mpi c;
		time_point t1 = STEADY_CLOCK_NOW;
		mpi::k_ray_expt(a, b, c);
		time_point t2 = STEADY_CLOCK_NOW;
		mpi e;
		time_point t3 = STEADY_CLOCK_NOW;
		mpi::expt_u32(a, p, e);
		time_point t4 = STEADY_CLOCK_NOW;
		ms k_ray = dura_cast<ms>(t2 - t1);
		ms exptd = dura_cast<ms>(t4 - t3);
		cout << i + 1 << ". " << endl;
		cout << "k-ray : " << k_ray.count() << "ms" << endl;
		cout << "exptd : " << exptd.count() << "ms" << endl;
		bool bb = mpi::cmp(c, e) == ord::eq;
		cout << (bb ? "True" : "False") << endl;
		if (!bb)
		{
			a.print("a base ");
			c.print("c k_ray");
			e.print("e exptd");
			Sleep(unsigned(-1));
		}
	}

	mpi::srand(chrono::steady_clock::now().time_since_epoch().count());
	for (int i = 0; i < 10; i++)
	{
		mpi a = mpi::rand(50000);
		mpi b = mpi::rand(50000);
		mpi c;
		chrono::time_point t1 = chrono::steady_clock::now();
		mpi::mul(a, b, c);
		chrono::time_point t2 = chrono::steady_clock::now();
		cout << chrono::duration_cast<chrono::milliseconds>(t2 - t1).count() << endl;
	}

	for (size_t i = 0; i < 1024; i++)
	{
		cout << i + 1 << ". ";
		mpi a = mpi::rand(128);
		a.sign = 1;
		mpi b, c;
		mpi::root_u32(a, 2, b);
		mpi::sqrt(a, c);
		if (mpi::cmp(b, c) == ord::eq)
		{
			cout << "True";
		}
		else
		{
			cout << "False";
		}
		cout << endl;
	}

	/* 测试 base_fact 和 mpi::fact 的效率 */
	mpi basefact, fastfact;
	for (uint32_t i = 1; i <= 65536; i++)
	{
		cout << i << ". " << endl;
		chrono::time_point t1 = chrono::steady_clock::now();
		mpi::fact(i, fastfact);
		chrono::time_point t2 = chrono::steady_clock::now();
		cout << "Fast Fact takes : " << chrono::duration_cast<chrono::nanoseconds>(t2 - t1).count() << "ns" << endl;

		t1 = chrono::steady_clock::now();
		base_fact(i, basefact);
		t2 = chrono::steady_clock::now();
		cout << "Base Fact takes : " << chrono::duration_cast<chrono::nanoseconds>(t2 - t1).count() << "ns" << endl;
		cout << (mpi::cmp(fastfact, basefact) == ord::eq ? "True" : "False") << endl;
	}
}
MPL_END