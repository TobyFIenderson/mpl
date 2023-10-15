#include "mpi_test.h"
using namespace std;
MPL_BEGIN
static const char* opnd1_filepath = "mpi_test/to_str_test_operand_1";
static const char* test_result_filepath = "mpi_test/to_str_test_result";
static const char* run_py_script_cmd = "python mpi_test/to_str_test.py";
void mpi_test::test_to_str(int n_sample, int opnd1_length, uint32_t radix, unsigned int seed)
{
	cout << "接下来将测试[mpi::to_str( radix = " << radix << " )]，共有 " << n_sample << " 个测试样例" << endl;
	auto opnd1 = generate_mpi_to_file(n_sample, opnd1_length, opnd1_filepath, seed);
	vector<string> result;
	for (mpi const& m : opnd1)
	{
		result.emplace_back(m.to_str());
	}
	output_str_to_file(result, test_result_filepath);
	system(run_py_script_cmd);
}

MPL_END