#include "mpi_test.h"
using namespace std;
MPL_BEGIN
void mpi_test::test_mul_time(int n_sample, int opnd1_length, int opnd2_length, unsigned int seed)
{
	cout << "--------------------[�˷�]��ʱ����--------------------" << endl;
	cout << "��һ���������ĳ���Ϊ��" << opnd1_length * MPI_DIGIT_BIT << "bits" << endl;
	cout << "�ڶ����������ĳ���Ϊ��" << opnd2_length * MPI_DIGIT_BIT << "bits" << endl;
	cout << "���������Ϊ��0x" << hex << uppercase << seed << dec << endl;
	cout << "ִ�г˷�������" << n_sample << endl;
	auto opnd1 = generate_mpi_to_file(n_sample, opnd1_length, "", seed);
	auto opnd2 = generate_mpi_to_file(n_sample, opnd2_length, "", seed << 1);
	mpi r;
	MILLI_TIME(
		for (int i = 0; i < n_sample; i++)
		{
			mpi::mul(opnd1[i], opnd2[i], r);
		}
	, �˷��ܺ�ʱ);
	cout << "ƽ��ÿ�γ˷���ʱ��" << double(time_count) / n_sample << "ms" << endl;
}
MPL_END