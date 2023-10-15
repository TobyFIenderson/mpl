#include "mpf.h"
MPL_BEGIN
void mpf::cot(mpf const& a, ang b, mpf& c)
{
	mpf aa;
	switch (b)
	{
	case ang::deg:
		mpf::mod(a, 360, aa);
		mpf::mul(aa, mpf::const_dtor_ratio(), aa);
		break;
	case ang::rad:
		mpf::mod_pi(a, aa);
		break;
	default:
		throw MPL_ERR_INVALID_VAL;
		break;
	}
	mpf cc;
	mpf::cos_taylor(aa, cc);
	if (cc.is_ignorable())
	{
		c.set_d(0);
		return;
	}
	mpf::sin_taylor(aa, aa);
	if (aa.is_ignorable())
	{
		throw MPL_ERR_INVALID_VAL;
	}
	mpf::div(cc, aa, c);
}
MPL_END