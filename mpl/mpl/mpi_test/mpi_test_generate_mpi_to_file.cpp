#include "mpi_test.h"
using namespace std;
MPL_BEGIN
vector<mpi> mpi_test::generate_mpi_to_file(int n, int opnd_length, string_view filepath, unsigned int seed)
{
	mpi::srand(seed);
	vector<mpi> vmpi(n, mpi::create_size(opnd_length));
	for (mpi& m : vmpi)
	{
		mpi::rand(m, opnd_length);
	}
	if (!filepath.empty())
	{
		output_mpi_to_file(vmpi, filepath);
	}
	return vmpi;
}

MPL_END