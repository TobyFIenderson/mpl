#include "mpf.h"
MPL_BEGIN
void mpf::set_2expt(uint32_t d)
{
	set_d(0);
	exp = d / MPI_DIGIT_BIT;
	d %= MPI_DIGIT_BIT;
	mant.set_2expt(d);
}
MPL_END