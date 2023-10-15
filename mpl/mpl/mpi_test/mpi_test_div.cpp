#include "mpi_test.h"
using namespace std;
MPL_BEGIN
/* ����ӷ����˷�������ȷ�Ļ��������մ˷���֤ a / b = c �� d  ---> c * b + d = a */
void mpi_test::test_div(int n_sample, int opnd1_length, int opnd2_length, unsigned int seed)
{
	cout << "������������[����]������ " << n_sample << " ����������" << endl;
	auto opnd1 = generate_mpi_to_file(n_sample, opnd1_length, "", seed);
	auto opnd2 = generate_mpi_to_file(n_sample, opnd2_length, "", seed << 4);
	for (int i = 0; i < n_sample; i++)
	{
		mpi c, d, e;
		mpi::div(opnd1[i], opnd2[i], &c, &d);
		mpi::mul(c, opnd2[i], e);
		mpi::add(e, d, e);
		cout << i + 1 << ". ";
		if (mpi::cmp(e, opnd1[i]) == ord::eq)
		{
			cout << "True" << endl;
		}
		else
		{
			cout << "False" << endl;
			opnd1[i].print();
			opnd2[i].print();
			c.print();
			d.print();
			e.print();
		}
	}
}

MPL_END