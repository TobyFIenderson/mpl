#include "mpf.h"
MPL_BEGIN
void mpf::scale_ang(mpf const& a, ang b, mpf& c)
{
	switch (b)
	{
	case mpl::ang::deg:
		mpf::mod(a, 360, c);
		break;
	case mpl::ang::rad:
		mpf::mod_2pi(a, c);
		break;
	default:
		throw MPL_ERR_INVALID_VAL;
		break;
	}
}
MPL_END