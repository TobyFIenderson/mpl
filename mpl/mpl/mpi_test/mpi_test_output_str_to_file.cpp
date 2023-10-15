#include "mpi_test.h"
using namespace std;
MPL_BEGIN

void mpi_test::output_str_to_file(std::vector<std::string> const& vs, std::string_view filepath)
{
	fstream fs(filepath.data(), ios::out);
	if (!fs.is_open())
	{
		throw "打开文件失败！";
	}
	for (string const& str : vs)
	{
		fs << str << endl;
	}
	fs.close();
}

MPL_END