#include "mpi_test.h"
using namespace std;
MPL_BEGIN

string mpi_test::get_contents_from_file(std::string_view filepath)
{
	string contents = "";
	fstream fs(filepath.data(), ios::in);
	if (!fs.is_open())
	{
		throw "文件打开失败！";
	}
	string s;
	while (getline(fs, s))
	{
		contents += s;
		contents += '\n';
	}
	fs.close();
	return contents;
}

MPL_END