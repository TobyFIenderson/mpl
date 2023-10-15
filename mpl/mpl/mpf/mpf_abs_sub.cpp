#include "mpf.h"
MPL_BEGIN

void mpf::abs_sub(mpf const& a, mpf const& b, mpf& c)
{
	if (a.exp > b.exp)
	{
		size_t diff = static_cast<size_t>(a.exp - b.exp);
		size_t mu = std::max(int64_t(a.mant.used + diff) - MPF_PRECISION - 1, 0i64);

		bool need_sub_1 = false;

		for (size_t i = 0; i < mu && i < b.mant.used; i++)
		{
			if (b.mant.dp[i] != 0)
			{
				need_sub_1 = true;
				break;
			}
		}

		mpf ta = a;
		size_t lshd = mu == 0 ? diff : size_t MPF_PRECISION - ta.mant.used + 1;
		ta.mant.lsh_x(lshd);
		ta.exp -= int32_t(lshd);

		if (mu >= b.mant.used)
		{
			if (need_sub_1)
			{
				mpi::abs_sub(ta.mant, 1, ta.mant);
			}
			ta.trimit();
			return;
		}

		size_t tused = size_t(b.mant.used) - mu;
		mpi t = mpi::create_size(tused);
		mpi_digit* tmpt = t.dp;
		const mpi_digit* tmpb = b.mant.dp + mu;
		for (size_t i = 0; i < tused; i++)
		{
			*tmpt++ = *tmpb++;
		}
		t.used = tused;
		
		mpi::abs_sub(ta.mant, t, ta.mant);
		if (need_sub_1)
		{
			mpi::abs_sub(ta.mant, 1, ta.mant);
		}
		ta.trimit();
		c.swap(ta);
	}
	else if (a.exp < b.exp)
	{
		size_t diff = a.mant.used - b.mant.used;
		mpf t = b;
		t.mant.lsh_x(diff);
		t.exp -= int32_t(diff);
		mpf::abs_sub(a, t, c);
	}
	else
	{
		mpi::abs_sub(a.mant, b.mant, c.mant);
	}

}
MPL_END