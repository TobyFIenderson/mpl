#include "mpi_test.h"
using namespace std;
MPL_BEGIN
void mpi_test::test_div_time(int n_sample, int opnd1_length, int opnd2_length, unsigned int seed)
{
	cout << "--------------------[除法]耗时测试--------------------" << endl;
	cout << "第一个操作数的长度为：" << opnd1_length * MPI_DIGIT_BIT << "bits" << endl;
	cout << "第二个操作数的长度为：" << opnd2_length * MPI_DIGIT_BIT << "bits" << endl;
	cout << "随机数种子为：0x" << hex << uppercase << seed << dec << endl;
	cout << "执行除法次数：" << n_sample << endl;
	auto opnd1 = generate_mpi_to_file(n_sample, opnd1_length, "", seed);
	auto opnd2 = generate_mpi_to_file(n_sample, opnd2_length, "", seed << 1);
	mpi r, q;
	MILLI_TIME(
		for (int i = 0; i < n_sample; i++)
		{
			mpi::div(opnd1[i], opnd2[i], &r, &q);
		}
	, 除法总耗时);
	cout << "平均每次除法耗时：" << double(time_count) / n_sample << "ms" << endl;
}
MPL_END