#include "mpi.h"
MPL_BEGIN

void mpi::signed_rsh(uint64_t d)
{
	if (sign == 1)
	{
		rsh(d);
		return;
	}
	this->incr();
	rsh(d);
	this->decr();
}

MPL_END