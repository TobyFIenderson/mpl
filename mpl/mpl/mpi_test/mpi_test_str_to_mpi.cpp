#include "mpi_test.h"
using namespace std;
MPL_BEGIN
void mpi_test::test_str_to_mpi(int n_sample, int str_length, uint32_t radix, unsigned int seed)
{
	if (radix < 2 || radix > 16)
	{
		cout << "mpi_test::test_str_to_mpi bad radix!!!" << endl;
	}
	cout << "接下来将测试[mpi::str_to_mpi( radix = " << radix << " )]，共有 " << n_sample << " 个测试样例" << endl;
	mt19937 mt(seed);
	for (int i = 0; i < n_sample; i++)
	{
		cout << i + 1 << ". ";
		string s;
		if (mt() % 2 == 0)
		{
			s += '-';
		}
		for (int j = 0; j < str_length; j++)
		{
			s += MPI_DIGITS_STR[mt() % radix];
		}

		int j = s[0] == '-' ? 1 : 0;

		while (s[j] == '0')
		{
			s[j] = MPI_DIGITS_STR[mt() % radix];
		}

		string r = mpi::str_to_mpi(s, radix).to_str(radix);
		if (s == r)
		{
			cout << "True" << endl;
		}
		else
		{
			cout << "False" << endl;
			cout << "Input :" << s << endl;
			cout << "Output:" << r << endl;
		}
	}
}

MPL_END