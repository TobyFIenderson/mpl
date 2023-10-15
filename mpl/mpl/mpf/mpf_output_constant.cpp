#include "mpf.h"
using namespace std;
MPL_BEGIN
void mpf::output_constant(ostream& os, string_view constant_name, string_view prefix) const
{
	os << prefix << " mpf::init_struct " << constant_name << " = " << endl;
	os << "{" << endl;
	os << "    " << exp << "," << endl;
	os << "    {" << endl;
	os << "        " << mant.sign << "," << endl;
	os << "        " << mant.used << "," << endl;
	os << "        {" << endl;
	os << "            ";
	for (size_t i = 0; i < mant.used; i++)
	{
		os << mant.dp[i] << ", ";
		if ((i + 1) != mant.used && (i + 1) % 8 == 0)
		{
			os << endl << "            ";
		}
	}
	os << endl << "        }" << endl;
	os << "    }" << endl;
	os << "};" << endl;

}

MPL_END