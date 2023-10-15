#include "mpi_test.h"
using namespace std;
MPL_BEGIN

void mpi_test::output_mpi_to_file(std::vector<mpi> const& vmpi, std::string_view filepath)
{
	fstream fs(filepath.data(), ios::out);
	if (!fs.is_open())
	{
		throw "打开文件失败！";
	}
	for (mpi const& m : vmpi)
	{
		fs << m.sign;
		for (size_t i = 0; i < m.used; i++)
		{
			fs << " " << m.dp[i];
		}
		if (m.used == 0)
		{
			fs << 0;
		}
		fs << endl;
	}
	fs.close();
}

MPL_END