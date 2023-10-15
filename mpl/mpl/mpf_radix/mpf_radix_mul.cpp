#include "mpf_radix.h"
MPL_BEGIN

static const double PI = acos(-1);

static void _FFT(int limit, double_complex* cps, int* rev, int inv)
{
	for (int i = 0; i < limit; i++)
	{
		if (i < rev[i])
		{
			std::swap(cps[i], cps[rev[i]]);
		}
	}
	for (int mid = 1; mid < limit; mid <<= 1) // 待合并区间的长度的一半
	{
		// w(k,n) w(1,n)
		double_complex wn(cos(PI / mid), inv * sin(PI / mid));
		for (int r = mid << 1, j = 0; j < limit; j += r) // r 代表区间长度
		{
			// w(0,n)
			double_complex w(1, 0);
			for (int k = 0; k < mid; k++, w = w * wn) // 枚举左半部分
			{
				double_complex x = cps[j + k], y = w * cps[j + mid + k];
				cps[j + k] = x + y;
				cps[j + mid + k] = x - y;
			}
		}
	}

	// cps[i] 表示原多项式在 x 取 w(i,n) 时的点值。
}

void mpf_radix::mul(mpf_radix const& num, int32_t precision, double_complex* a, int* rev)
{
	if (radix != num.radix)
	{
		throw MPL_ERR_INVALID_VAL;
	}

	int n = int(mant.size() - 1);
	int m = int(num.mant.size() - 1);
	int limit = 1, bits = 0;
	while (limit <= n + m)
	{
		limit <<= 1;
		bits++;
	}

	memset(a, 0, sizeof(double_complex) * (static_cast<unsigned long long>(limit) + 1));
	memset(rev, 0, sizeof(int) * (static_cast<unsigned long long>(limit) + 1));

	for (int i = 0; i <= n; i++)
	{
		a[i].x = mant[i];
	}
	for (int i = 0; i <= m; i++)
	{
		a[i].y = num.mant[i];
	}


	for (int i = 0; i <= limit; i++)
	{
		// 数位dp
		rev[i] = (rev[i >> 1] >> 1) | ((i & 1) << (bits - 1));
	}

	_FFT(limit, a, rev, 1);
	for (int i = 0; i <= limit; i++)
	{
		a[i] = a[i] * a[i];
	}
	_FFT(limit, a, rev, -1);

	std::vector<mpi_digit> ans;
	for (int i = 0; i <= n + m; i++)
	{
		ans.push_back((mpi_digit)(a[i].y / 2 / limit + 0.5));
	}
	for (size_t i = 0; i < ans.size() - 1; i++)
	{
		ans[i + 1] += ans[i] / radix;
		ans[i] %= radix;
	}
	int tmp = ans.back();
	ans.pop_back();
	while (tmp != 0)
	{
		ans.push_back(tmp % radix);
		tmp /= radix;
	}
	mant = std::move(ans);
	sign *= num.sign;
	exp += num.exp;
	clamp();
	trimit(precision);
}
MPL_END