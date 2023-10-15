#include "mpf.h"
MPL_BEGIN
void mpf::root(mpf const& a, mpf const& b, mpf& c)
{
	mpf pb = b;
	int32_t bsign = b.mant.sign;
	pb.mant.sign = 1;
	if (mpf::cmp(pb, 2) == ord::eq)
	{
		mpf::sqrt(a, c);
	}
	else if (mpf::cmp(pb, 3) == ord::eq)
	{
		mpf::cubet(a, c);
	}
	else
	{
		mpf::inv(pb, pb);
		mpf::pow(a, pb, c);
	}
	if (bsign == -1)
	{
		mpf::inv(c, c);
	}
}
MPL_END