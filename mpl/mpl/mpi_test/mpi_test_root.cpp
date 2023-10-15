#include "mpi_test.h"
using namespace std;
MPL_BEGIN
void mpi_test::test_root(int n_sample, int opnd1_length, int opnd2_range, unsigned int seed)
{
	cout << "接下来将测试[root]，共有 " << n_sample << " 个测试样例" << endl;
	auto opnd1 = generate_mpi_to_file(n_sample, opnd1_length, "", seed);
	vector<uint32_t> opnd2;
	mt19937 mt(seed << 7);

	for (int i = 0; i < n_sample; i++)
	{
		opnd2.push_back(mt() % opnd2_range + 1);
	}
	for (int i = 0; i < n_sample; i++)
	{
		if ((opnd2[i] & 1) == 0)
		{
			opnd1[i].sign = 1;
		}
		mpi r1, r2, r3, r4;
		mpi::root_u32(opnd1[i], opnd2[i], r1);
		mpi::expt_u32(r1, opnd2[i], r2);
		if (r1.is_neg())
		{
			mpi::sub_d(r1, 1, r3);
		}
		else
		{
			mpi::add_d(r1, 1, r3);
		}
		mpi::expt_u32(r3, opnd2[i], r4);
		cout << i + 1 << ". ";
		if (mpi::cmp(r2, opnd1[i]) == ord::eq || mpi::abs_cmp(r4, opnd1[i]) == ord::gt)
		{
			cout << "True" << endl;
		}
		else
		{
			cout << "False" << endl;
		}
	}
}
MPL_END