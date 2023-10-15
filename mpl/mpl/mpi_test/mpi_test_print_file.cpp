#include "mpi_test.h"
using namespace std;
MPL_BEGIN

void mpi_test::print_file(string_view filepath)
{
	fstream fs(filepath.data(), ios::in);
	if (!fs.is_open())
	{
		throw "文件打开失败！";
	}
	string s;
	while (getline(fs, s))
	{
		cout << s << endl;
	}
	fs.close();
}

MPL_END