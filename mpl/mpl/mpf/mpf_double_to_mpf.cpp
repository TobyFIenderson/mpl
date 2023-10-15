#include "mpf.h"
#include <sstream>
#include <iomanip>
using namespace std;
MPL_BEGIN
mpf mpf::double_to_mpf(double d)
{
	stringstream ss;
	ss << setprecision(16) << d;
	return str_to_mpf(ss.view(), 10);
}
MPL_END