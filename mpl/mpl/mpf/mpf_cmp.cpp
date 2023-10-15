#include "mpf.h"
MPL_BEGIN
ord mpf::cmp(mpf const& a, mpf const& b)
{
	return a.mant.sign != b.mant.sign ?
		(a.mant.sign == -1 ? ord::lt : ord::gt) :
		(a.mant.sign == -1 ? abs_cmp(b, a) : abs_cmp(a, b));
}
MPL_END