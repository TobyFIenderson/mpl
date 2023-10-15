#include "mpf.h"
MPL_BEGIN
void mpf::set_d(mpi_digit d)
{
	mant.set_d(d);
	exp = 0;
}
MPL_END