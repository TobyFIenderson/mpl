#include "mpf.h"
MPL_BEGIN
void mpf::swap(mpf& m)
{
	mant.swap(m.mant);
	int32_t tmpexp = exp;
	exp = m.exp;
	m.exp = tmpexp;
}
MPL_END