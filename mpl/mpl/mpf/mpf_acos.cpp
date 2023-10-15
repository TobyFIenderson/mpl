#include "mpf.h"
MPL_BEGIN
void mpf::acos(mpf const& a, ang b, mpf& c)
{
	try
	{
		asin(a, b, c);
	}
	catch (mpl_err e)
	{
		throw e;
	}
	mpf halfpi = b == ang::rad ? mpf::const_half_pi() : 90;
	halfpi.trimit();
	mpf::sub(halfpi, c, c);
}
MPL_END