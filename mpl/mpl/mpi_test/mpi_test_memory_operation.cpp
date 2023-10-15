#include "mpi_test.h"
using namespace std;
MPL_BEGIN

void mpi_test::test_memory_operation(unsigned int seed)
{
	mpi::srand(seed);
	cout << "1. ²âÊÔ¸³ÖµÓï¾ä" << endl;
	/*for (int i = 0; i < (1 << 28); i++)
	{
		mpi r0 = mpi::create_size(1 << 16);
		mpi r1 = r0;
	}*/
	cout << "2. ²âÊÔÓÒÖµÒýÓÃ" << endl;
	for (int i = 0; i < (1 << 28); i++)
	{
		mpi r0 = mpi::create_size(1 << 16);
		mpi r1 = move(r0);
		mpi r2 = move(r1);
		mpi r3 = move(r2);
		mpi r4, r5, r6, r7;
		r4 = move(r3);
		r5 = move(r4);
		r6 = move(r5);
		r7 = move(r6);
	}
}

MPL_END