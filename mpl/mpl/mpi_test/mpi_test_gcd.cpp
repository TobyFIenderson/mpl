#include "mpi_test.h"
MPL_BEGIN
using namespace std;
static const char* opnd1_filepath = "mpi_test/gcd_test_operand_1";
static const char* opnd2_filepath = "mpi_test/gcd_test_operand_2";
static const char* test_result_filepath = "mpi_test/gcd_test_result";
static const char* run_py_script_cmd = "python mpi_test/gcd_test.py";
void mpi_test::test_gcd(int n_sample, int opnd1_length, int opnd2_length, unsigned int seed)
{
	cout << "接下来将测试[gcd]，共有 " << n_sample << " 个测试样例" << endl;
	auto opnd1 = generate_mpi_to_file(n_sample, opnd1_length, opnd1_filepath, seed);
	auto opnd2 = generate_mpi_to_file(n_sample, opnd2_length, opnd2_filepath, seed << 1);
	auto& minone = opnd1_length < opnd2_length ? opnd1 : opnd2;
	vector<mpi> result;
	for (int i = 0; i < n_sample; i++)
	{
		mpi::gcd(opnd1[i], opnd2[i], minone[i]);
		result.emplace_back(move(minone[i]));
	}
	output_mpi_to_file(result, test_result_filepath);
	system(run_py_script_cmd);
}
MPL_END