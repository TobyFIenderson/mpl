#include "mpf.h"
MPL_BEGIN
static constexpr uint32_t F1_SIZE = 17;
static const mpi::init_struct F1_0 =
{
	1,
	1,
	{ 1,  }
};
static const mpi::init_struct F1_1 =
{
	1,
	1,
	{ 1,  }
};
static const mpi::init_struct F1_2 =
{
	1,
	1,
	{ 2,  }
};
static const mpi::init_struct F1_3 =
{
	1,
	1,
	{ 6,  }
};
static const mpi::init_struct F1_4 =
{
	1,
	1,
	{ 24,  }
};
static const mpi::init_struct F1_5 =
{
	1,
	1,
	{ 120,  }
};
static const mpi::init_struct F1_6 =
{
	1,
	1,
	{ 720,  }
};
static const mpi::init_struct F1_7 =
{
	1,
	1,
	{ 5040,  }
};
static const mpi::init_struct F1_8 =
{
	1,
	1,
	{ 40320,  }
};
static const mpi::init_struct F1_9 =
{
	1,
	1,
	{ 362880,  }
};
static const mpi::init_struct F1_10 =
{
	1,
	1,
	{ 3628800,  }
};
static const mpi::init_struct F1_11 =
{
	1,
	1,
	{ 39916800,  }
};
static const mpi::init_struct F1_12 =
{
	1,
	2,
	{ 210566144, 1,  }
};
static const mpi::init_struct F1_13 =
{
	1,
	2,
	{ 53005312, 23,  }
};
static const mpi::init_struct F1_14 =
{
	1,
	2,
	{ 205203456, 324,  }
};
static const mpi::init_struct F1_15 =
{
	1,
	2,
	{ 125261824, 4871,  }
};
static const mpi::init_struct F1_16 =
{
	1,
	2,
	{ 125140992, 77943,  }
};
// F1表的 dp 总长为 = 22
// 约占用 0.0859375 KB
static const mpi F1[F1_SIZE] =
{
	F1_0, F1_1, F1_2, F1_3, F1_4, F1_5, F1_6, F1_7,
	F1_8, F1_9, F1_10, F1_11, F1_12, F1_13, F1_14, F1_15,
	F1_16,
};

static constexpr uint32_t F2_SIZE = 34;
static const mpi::init_struct F2_0 =
{
	1,
	1,
	{ 1,  }
};
static const mpi::init_struct F2_1 =
{
	1,
	1,
	{ 1,  }
};
static const mpi::init_struct F2_2 =
{
	1,
	1,
	{ 2,  }
};
static const mpi::init_struct F2_3 =
{
	1,
	1,
	{ 3,  }
};
static const mpi::init_struct F2_4 =
{
	1,
	1,
	{ 8,  }
};
static const mpi::init_struct F2_5 =
{
	1,
	1,
	{ 15,  }
};
static const mpi::init_struct F2_6 =
{
	1,
	1,
	{ 48,  }
};
static const mpi::init_struct F2_7 =
{
	1,
	1,
	{ 105,  }
};
static const mpi::init_struct F2_8 =
{
	1,
	1,
	{ 384,  }
};
static const mpi::init_struct F2_9 =
{
	1,
	1,
	{ 945,  }
};
static const mpi::init_struct F2_10 =
{
	1,
	1,
	{ 3840,  }
};
static const mpi::init_struct F2_11 =
{
	1,
	1,
	{ 10395,  }
};
static const mpi::init_struct F2_12 =
{
	1,
	1,
	{ 46080,  }
};
static const mpi::init_struct F2_13 =
{
	1,
	1,
	{ 135135,  }
};
static const mpi::init_struct F2_14 =
{
	1,
	1,
	{ 645120,  }
};
static const mpi::init_struct F2_15 =
{
	1,
	1,
	{ 2027025,  }
};
static const mpi::init_struct F2_16 =
{
	1,
	1,
	{ 10321920,  }
};
static const mpi::init_struct F2_17 =
{
	1,
	1,
	{ 34459425,  }
};
static const mpi::init_struct F2_18 =
{
	1,
	1,
	{ 185794560,  }
};
static const mpi::init_struct F2_19 =
{
	1,
	2,
	{ 117858163, 2,  }
};
static const mpi::init_struct F2_20 =
{
	1,
	2,
	{ 226230272, 13,  }
};
static const mpi::init_struct F2_21 =
{
	1,
	2,
	{ 59102319, 51,  }
};
static const mpi::init_struct F2_22 =
{
	1,
	2,
	{ 145227776, 304,  }
};
static const mpi::init_struct F2_23 =
{
	1,
	2,
	{ 17176057, 1178,  }
};
static const mpi::init_struct F2_24 =
{
	1,
	2,
	{ 264241152, 7308,  }
};
static const mpi::init_struct F2_25 =
{
	1,
	2,
	{ 160965969, 29451,  }
};
static const mpi::init_struct F2_26 =
{
	1,
	2,
	{ 159383552, 190033,  }
};
static const mpi::init_struct F2_27 =
{
	1,
	2,
	{ 51113867, 795193,  }
};
static const mpi::init_struct F2_28 =
{
	1,
	2,
	{ 167772160, 5320940,  }
};
static const mpi::init_struct F2_29 =
{
	1,
	2,
	{ 140124863, 23060602,  }
};
static const mpi::init_struct F2_30 =
{
	1,
	2,
	{ 201326592, 159628218,  }
};
static const mpi::init_struct F2_31 =
{
	1,
	3,
	{ 48903457, 178007766, 2,  }
};
static const mpi::init_struct F2_32 =
{
	1,
	3,
	{ 0, 7829336, 19,  }
};
static const mpi::init_struct F2_33 =
{
	1,
	3,
	{ 3201345, 237111708, 87,  }
};
// F1表的 dp 总长为 = 34
// 约占用 0.132812 KB * 2
static const mpi F2[F2_SIZE] =
{
	F2_0, F2_1, F2_2, F2_3, F2_4, F2_5, F2_6, F2_7,
	F2_8, F2_9, F2_10, F2_11, F2_12, F2_13, F2_14, F2_15,
	F2_16, F2_17, F2_18, F2_19, F2_20, F2_21, F2_22, F2_23,
	F2_24, F2_25, F2_26, F2_27, F2_28, F2_29, F2_30, F2_31,
	F2_32, F2_33,
};

void mpf::fact(uint32_t n, mpf& b)
{
	if (n < F1_SIZE)
	{
		b = F1[n];
		return;
	}
	uint32_t k = n >> 1;
	/* 这里使用 vector 可能会存在一丢丢的效率问题，不过为了编码方便随便了~ */
	/* 提供个小小的优化思路，受限于 uint32_t 的数据类型，vi 的长度最多为 32 + 2 */
	std::vector<uint32_t> vi;
	vi.push_back(0);
	vi.push_back((n & 1) == 1 ? n : n - 1);
	while (k >= F1_SIZE)
	{
		vi.push_back((k & 1) == 1 ? k : k - 1);
		vi[0] += k;
		k >>= 1;
	}
	vi.push_back(k);
	vi[0] += k;

	b.set_2expt(vi[0]);
	mpf tfe = 1;
	for (size_t i = 1; i < vi.size() - 2; i++)
	{
		tfe.set_d(1);
		for (uint32_t j = vi[i]; j >= vi[i + 1] + 2; j -= 2)
		{
			mpf::mul_d(tfe, j, tfe);
		}
		/* 这里还也还可以再优化，
		 * 注意到 ans 是一个形如 a * b^2 * c^3 * d^4 * e^5 的式子
		 * 那么可以写成 ans = (((e*d)^2)*(c*b))^2*(e*c*a)
		 * 充分发挥了平方算法的作用，但是我懒，就这样吧 _^^ */
		mpf::expt_u32(tfe, uint32_t(i), tfe);
		mpf::mul(b, tfe, b);
	}
	mpf::expt_u32(F2[vi[vi.size() - 2]], uint32_t(vi.size() - 2), tfe);
	mpf::mul(b, tfe, b);
	mpf::mul(b, F1[vi.back()], b);
}
MPL_END