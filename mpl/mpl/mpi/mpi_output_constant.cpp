#include "mpi.h"
using namespace std;
MPL_BEGIN

void mpi::output_constant(ostream& os, string_view constant_name, string_view prefix)
{
	os << prefix << " mpi::init_struct " << constant_name << " = " << endl;
	os << "{" << endl;
	os << "    " << sign << "," << endl;
	os << "    " << used << "," << endl;
	os << "    { ";
	for (size_t i = 0; i < used; i++)
	{
		os << dp[i] << ", ";
		if ((i + 1) % 8 == 0)
		{
			os << endl;
		}
	}
	os << " }" << endl;
	os << "};" << endl;
}

MPL_END