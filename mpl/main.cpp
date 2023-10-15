#pragma warning(push)
#pragma warning(disable:4003 4100)

#include "mpl/mpi_test/mpi_test.h"
#include "mpl/mpf/mpf.h"
#include "CalculatorManagerLib/NumberWidth.h"
#include "CalculatorTester/CalculatorInputTester.hpp"
#include "CalculatorTester/GroupDigitsUtilsTester.hpp"
#include "CalculatorTester/ExpressionCollectorTester.hpp"
#include "CalculatorTester/CalculatorEngineTester.hpp"
using namespace std;
using namespace mpl;
using namespace CalculatorManagerLib;
//#include "mpi/tune.hpp"
//int main(int argc, char** argv)
//{
//	
//
//	tune::_main_(argc, argv);
//
//	return 0;
//}
//
//struct test_mpf_init_struct
//{
//	int exp;
//	mpi::init_struct m;
//};
//const test_mpf_init_struct f{ 0,{1,2,{1,2}} };
void test_mpf()
{
	void (*p[4])(mpf const& a, mpf const& b, mpf & c) = { mpf::add,mpf::sub,mpf::mul,mpf::div };

	const char* str[4] = { "add","sub","mul","div" };

	for (int i = 0; i < 4; i++)
	{
		mpf rawa = mpi("1234567890987654321");
		rawa.exp = -2;
		mpf rawb = mpi("9876543210123456789");
		rawb.exp = -1;

		mpf a, b, c;
		// a a a
		a = rawa;
		p[i](a, a, a);
		cout << "a " << str[i] << " a ---> a" << endl;
		cout << a.to_str() << endl;

		// a a b
		a = rawa;
		p[i](a, a, b);
		cout << "a " << str[i] << " a ---> b" << endl;
		cout << b.to_str() << endl;

		// a b a
		a = rawa;
		b = rawb;
		p[i](a, b, a);
		cout << "a " << str[i] << " b ---> a" << endl;
		cout << a.to_str() << endl;

		// a b b
		a = rawa;
		b = rawb;
		p[i](a, b, b);
		cout << "a " << str[i] << " b ---> b" << endl;
		cout << b.to_str() << endl;

		// a b c
		a = rawa;
		b = rawb;
		p[i](a, b, c);
		cout << "a " << str[i] << " b ---> c" << endl;
		cout << c.to_str() << endl;
	}

}

void test_mpf_k_ray_expt()
{
	mpf r = mpi("10000000000001");
	mpf t13 = mpi("1000000000000");
	mpf::div(r, t13, r);
	uint32_t exp = 0;
	while (true)
	{
		cin >> exp;
		mpf rr;
		chrono::time_point t1 = chrono::steady_clock::now();
		mpf::expt_u32(r, exp, rr);
		chrono::time_point t2 = chrono::steady_clock::now();
		cout << "FAST EXP takes : " << chrono::duration_cast<chrono::nanoseconds>(t2 - t1) << endl;
		mpf rr2;
		t1 = chrono::steady_clock::now();
		mpf::k_ray_expt(r, exp, rr2);
		t2 = chrono::steady_clock::now();
		cout << "K_RAY EXP takes: " << chrono::duration_cast<chrono::nanoseconds>(t2 - t1) << endl;
		cout << boolalpha;
		cout << rr.mant << endl;
		cout << rr2.mant << endl;
	}
}

const char* mpl_err_to_string(mpl_err e)
{
	switch (e)
	{
	case MPL_ERR_INVALID_VAL: return "输入超限、无效输入！";
	case MPL_ERR_MAX_ITER_REACHED: return "达到最大迭代次数！";

	default: return "其他、未知错误";
	}
}
#define CLOCK_NOW std::chrono::system_clock::now().time_since_epoch().count()

#define MPF_CATCH(d)                                  \
	try                                               \
	{                                                 \
		d;                                            \
	}catch(mpl_err err)                               \
	{                                                 \
		std::cout<<mpl_err_to_string(err)<<std::endl; \
	}


void mpf_radix_convert_cutoff()
{
	for (int i = 100; i < 10000; i += 100)
	{
		cout << i << endl;
		mpi mi = mpi::rand(i);
		mi.sign = 1;
		mpf_radix mf{ mi.sign,vector(mi.dp,mi.dp + mi.used),MPI_BASE,0 };

		vector<mpi_digit> ii, ff;
		MILLI_TIME(ii = mi.to_radix(10), mpi takes);
		MILLI_TIME(ff = mf.to_radix(10, 100000).mant, mpf takes);

		if (ii.size() == ff.size())
		{
			bool b = true;
			for (size_t j = 0; j < ii.size(); j++)
			{
				if (ii[j] != ff[j])
				{
					b = false;
					break;
				}
			}
			cout << (b ? "True" : "False") << endl;
		}
		else
		{
			cout << "False" << endl;
		}
		if (i == 6600)
		{
			i = 6500;
		}
	}
}
void mpf_radix_convert_prec_cutoff()
{
	for (int i = 6500; i < 10000; i += 100)
	{
		cout << i << endl;
		mpi mi = mpi::rand(i);
		mi.sign = 1;
		mpf_radix mf{ mi.sign,vector(mi.dp,mi.dp + mi.used),MPI_BASE,0 };

		vector<mpi_digit> ii, ff;
		MILLI_TIME(ii = mi.to_radix(10), mpi takes);
		MILLI_TIME(ff = mf.to_radix(10, 100).mant, mpf takes);

		vector<mpi_digit>::reverse_iterator iir = ii.rbegin();
		vector<mpi_digit>::reverse_iterator ffr = ff.rbegin();
		int len = (int)ff.size();

		for (int j = 0; j < len; j++)
		{
			cout << *ffr;
			if (ffr == ff.rend())
			{
				break;
			}
			else
			{
				ffr++;
			}
		}
		cout << endl;
		for (int j = 0; j < len; j++)
		{
			cout << *iir;
			iir++;
		}
		cout << endl;

		if (i == 6600)
		{
			i = 1000;
		}
	}
}

void mpf::test()
{
}

void mpf_c_g(mpf const& a, string_view name)
{
	string s1 = "mpf_constant_";
	string s2 = "_init_struct";
	string s3 = "static const";
	a.output_constant(cout, s1 + string(name) + s2, s3);
}

void PrintNumberWidthMaxStrAndMinStr()
{
	for (int i = 0; i < 4; i++)
	{
		NumberWidth nw = NumberWidthType(i);
		for (int j = 0; j < 4; j++)
		{
			Radix r = RadixType(j);
			cout << nw.GetMaxNumberString(r) << endl;
			cout << nw.GetMinNumberString(r) << endl;
		}
		wcout << endl;
	}
}

mpf euler_mascheroni()
{
	int32_t sign = -1;
	mpf t = 0, tmp;
	mpi len = 1, i = 1, lgn = 0;
	mpi_digit n = 1;


	mpf::div_d(lgn, n, tmp);
	tmp.mant.sign = sign;
	sign *= -1;
	mpf::add(t, tmp, t);
	n++;
	i.incr();
	if (mpi::abs_cmp(i, len) == ord::gt)
	{
		i = 1;
		len.lsh(1);
		lgn.incr();
	}

	do
	{
		mpf::div_d(lgn, n, tmp);
		tmp.mant.sign = sign;
		sign *= -1;
		mpf::add(t, tmp, t);
		n++;
		i.incr();
		if (mpi::abs_cmp(i, len) == ord::gt)
		{
			i = 1;
			len.lsh(1);
			lgn.incr();
		}
		if (n % 5000 == 0)
		{
			cout << t << endl;
		}
	} while (!tmp.is_small_enough());
	return t;
}

void mpf_abs_add(mpf const& a, mpf const& b, mpf& c)
{
	const mpf* maxexp, * minexp;
	if (a.exp < b.exp)
	{
		maxexp = &b;
		minexp = &a;
	}
	else
	{
		maxexp = &a;
		minexp = &b;
	}

	size_t diff = static_cast<size_t>(maxexp->exp - minexp->exp);
	mpf r{ mpi::create_size(max(maxexp->mant.used + diff,minexp->mant.used) + 1),minexp->exp };

	size_t mintmp = min(diff, minexp->mant.used);
	mpi_digit* tmpr = r.mant.dp;
	const mpi_digit* tmpi = minexp->mant.dp;
	for (size_t i = 0; i < mintmp; i++)
	{
		*tmpr++ = *tmpi++;
	}

	mpi_digit u = 0;
	const mpi_digit* tmpa = maxexp->mant.dp;
	size_t i = 0;
	tmpr = r.mant.dp + diff;
	if (minexp->mant.used > diff)
	{
		mintmp = min(maxexp->mant.used, minexp->mant.used - diff);
		for (i = 0; i < mintmp; i++)
		{
			*tmpr = u + *tmpa++ + *tmpi++;
			u = *tmpr >> MPI_DIGIT_BIT;
			*tmpr++ &= MPI_MASK;
		}
	}

	if (i == maxexp->mant.used)
	{
		mintmp = minexp->mant.used - diff;
		while (i++ < mintmp)
		{
			*tmpr = u + *tmpi++;
			u = *tmpr >> MPI_DIGIT_BIT;
			*tmpr++ &= MPI_MASK;
		}
	}
	else
	{
		while (i++ < maxexp->mant.used)
		{
			*tmpr = u + *tmpa++;
			u = *tmpr >> MPI_DIGIT_BIT;
			*tmpr++ &= MPI_MASK;
		}
	}
	*tmpr = u;
	r.mant.used = r.mant.alloc;
	r.mant.clamp();
	r.trimit();
	r.swap(c);
}

void mpf_abs_add_fast(mpf const& a, mpf const& b, mpf& c)
{
	const mpf* maxexp, * minexp;
	if (a.exp < b.exp)
	{
		maxexp = &b;
		minexp = &a;
	}
	else
	{
		maxexp = &a;
		minexp = &b;
	}

	size_t diff = static_cast<size_t>(maxexp->exp - minexp->exp);

	size_t mu = max(int64_t(maxexp->mant.used + diff) - MPF_PRECISION, 0i64);

	if (mu >= minexp->mant.used)
	{
		c = *maxexp;
		return;
	}

	mpf r = mpi::create_size(min(max(maxexp->mant.used + diff, minexp->mant.used), size_t(MPF_PRECISION)) + 1);
	r.exp = minexp->exp;

	size_t mintmp = min(diff, minexp->mant.used);
	mpi_digit* tmpr = r.mant.dp;
	const mpi_digit* tmpi = minexp->mant.dp + mu;
	for (size_t i = mu; i < mintmp; i++)
	{
		*tmpr++ = *tmpi++;
	}

	mpi_digit u = 0;
	const mpi_digit* tmpa = maxexp->mant.dp;
	tmpr = r.mant.dp + diff - mu;
	size_t i = 0;
	if (minexp->mant.used > diff)
	{
		mintmp = min(maxexp->mant.used, minexp->mant.used - diff);
		for (i = 0; i < mintmp; i++)
		{
			*tmpr = u + *tmpa++ + *tmpi++;
			u = *tmpr >> MPI_DIGIT_BIT;
			*tmpr++ &= MPI_MASK;
		}
	}

	if (i == maxexp->mant.used)
	{
		mintmp = minexp->mant.used - diff;
		while (i++ < mintmp)
		{
			*tmpr = u + *tmpi++;
			u = *tmpr >> MPI_DIGIT_BIT;
			*tmpr++ &= MPI_MASK;
		}
	}
	else
	{
		while (i++ < maxexp->mant.used)
		{
			*tmpr = u + *tmpa++;
			u = *tmpr >> MPI_DIGIT_BIT;
			*tmpr++ &= MPI_MASK;
		}
	}
	*tmpr = u;
	r.mant.used = r.mant.alloc;
	r.mant.clamp();
	r.trimit();
	r.exp += int32_t(mu);
	r.swap(c);
}

void print_mpf(mpf const& c, string const& str = "")
{
	c.mant.print(str);
	cout << "exp: " << c.exp << endl;
}

void mpf_ln2(mpf const& a, mpf c)
{
	mpi n = 1;
	int sign = 1;
	mpf t = 0;
	mpf r = 0;
	do
	{
		mpf::inv(n, t);
		t.mant.sign = sign;
		sign *= -1;
		mpf::add(r, t, r);
		n.incr();
	} while (!t.is_small_enough());
	cout << "iteration n:" << n << endl;
}

void mpf_inv_atan(mpf const& a, mpf& c)
{
	c = a;
	mpf::inv(c, c);
	mpf::atan_taylor(c, c);
	mpf halfpi = mpf::const_half_pi();
	halfpi.trimit();
	mpf::sub(halfpi, c, c);
}

void mpf_xinv_atan(mpf const& a, mpf& c)
{
	c = a;
	mpf t = c;
	mpf::sqr(t, t);
	mpf::add(t, 1, t);
	mpf::sqrt(t, t);
	mpf::div(c, t, c);
	mpf::asin(c, ang::rad, c);
}

void mpf_sinh(mpf const& a, mpf& c)
{
	c = a;
	mpf t;
	mpf::e_expt(c, c);
	mpf::inv(c, t);
	mpf::sub(c, t, c);
	mpf::div_2(c, c);
}

void mpf_cosh(mpf const& a, mpf& c)
{
	c = a;
	mpf t;
	mpf::e_expt(c, c);
	mpf::inv(c, t);
	mpf::add(c, t, c);
	mpf::div_2(c, c);
}

void mpf_arsinh(mpf const& a, mpf& c)
{
	c = a;
	mpf t = c;
	mpf::sqr(t, t);
	mpf::add(t, 1, t);
	mpf::sqrt(t, t);
	mpf::add(t, c, c);
	mpf::ln(c, c);
}

static uint32_t maxn(uint32_t d)
{
	double f = d * log(10);
	double r = 0;
	uint32_t i = 0;
	do
	{
		i++;
		r += log(i);
	} while (r < f);
	return i;
}

static void split_euler(uint32_t n, uint32_t m, mpf& p, mpf& q)
{
	if (m - n <= 32)
	{/* 当 n 和 m 相差很小时直接计算 */
		p.set_d(1);
		q.set_d(1);
		for (uint32_t i = m; i > n; i--)
		{
			mpf::mul_d(q, i, q);
			mpf::add(p, q, p);
		}
		mpf::mul_d(q, n, q);
		return;
	}
	mpf p1, p2, q1, q2;
	split_euler(n, (n + m) >> 1, p1, q1);
	split_euler((n + m + 2) >> 1, m, p2, q2);
	mpf::mul(p1, q2, p);
	mpf::add(p, p2, p);
	mpf::mul(q1, q2, q);
}

void const_e()
{
	mpf p, q, e;
	double r = log(MPI_BASE) / log(10);
	uint32_t n = maxn(uint32_t(MPF_PRECISION * r));
	split_euler(1, n, p, q);
	mpf::add(p, q, p);
	mpf::div(p, q, p);
	e.swap(p);
}

void const_pi()
{
	mpf pi;

	/* quarter 只使用 1 位不用更新 */
	mpf a = 1;
	mpf b = mpf::const_sqrt2();
	mpf::inv(b, b);
	mpf t = "0.25";
	mpf p = 1;

	mpf m;
	do
	{
		m = a;
		mpf::add(a, b, a);
		mpf::div_2(a, a);

		mpf::mul(m, b, b);
		mpf::sqrt(b, b);

		mpf::sub(m, a, m);
		mpf::sqr(m, m);
		mpf::mul(p, m, m);
		mpf::sub(t, m, t);

		mpf::mul_2(p, p);
		mpf::sub(a, b, m);
	} while (!m.is_small_enough());

	mpf::add(a, b, a);
	mpf::sqr(a, a);
	mpf::mul_2d(t, 2, t);
	mpf::div(a, t, a);
	pi.swap(a);
}

static mpf agm(mpf const& a0, mpf const& b0)
{
	mpf a = a0, b = b0;
	mpf m;
	do
	{
		m = a;
		mpf::add(a, b, a);
		mpf::div_2(a, a);
		mpf::mul(m, b, b);
		mpf::sqrt(b, b);
		if (abs(a.rdig() - b.rdig()) == 0)
		{
			mpf::sub(a, b, m);
			if (m.is_small_enough())
			{
				break;
			}
		}

	} while (true);
	return a;
}

mpf const_ln2()
{
	uint32_t m = (MPF_PRECISION + 1) * MPI_DIGIT_BIT;
	m >>= 1;
	mpf m2 = 1;
	mpf::div_2d(m2, m - 2, m2);
	mpf A = agm(m2, 1);
	mpf::mul_2(A, A);
	mpf::mul_d(A, m, A);
	MILLI_TIME(mpf::const_pi(), mpf::const_pi);
	mpf::div(mpf::const_pi(), A, A);
	return A;
}

static void mpf_fast_add(mpf const& a, mpf const& b, mpf& c)
{
	if (a.is_zero())
	{
		c = b;
		return;
	}
	if (b.is_zero())
	{
		c = a;
		return;
	}

	if (a.mant.sign == b.mant.sign)
	{
		int32_t sign = a.mant.sign;
		mpf::abs_add(a, b, c);
		c.mant.sign = sign;
	}
	else
	{
		if (mpf::abs_cmp(a, b) == ord::lt)
		{
			int32_t sign = b.mant.sign;
			mpf::abs_sub(b, a, c);
			c.mant.sign = sign;
		}
		else
		{
			int32_t sign = a.mant.sign;
			mpf::abs_sub(a, b, c);
			c.mant.sign = sign;
		}
	}
	c.trimit();
}

int main0()
{
	for (size_t i = 0; i < 10; i++)
	{
		stringstream s1, s2;
		s1 << const_ln2();
		s2 << mpf::const_ln2();
		cout << "MPF_PRECISION = " << MPF_ORIGINAL_PRECISION << " " << (s1.str() == s2.str() ? "TRUE" : "FALSE") << endl;
		MPF_ORIGINAL_PRECISION <<= 1;
	}
	return 0;
}

int main1()
{
	MPF_ORIGINAL_PRECISION = 4096;

	mpf a, b, c, d;
	a = "0";
	c = "0.05";
	d = 1;
	int n = 3;
	while (mpf::cmp(a, 1) != ord::gt)
	{
		for (int i = 0; i < n; i++)
		{
			MILLI_TIME(mpf::ln_taylor(2, b), mpf::ln_taylor);
		}
		cout << "x = " << a << " takes " << double(time_count) / n << "ms" << endl;
		time_count = 0;
		mpf::add(a, c, a);
	}


	return 0;
}
int main2()
{
	MPF_ORIGINAL_PRECISION = 128;
	while (MPF_ORIGINAL_PRECISION <= 32768)
	{
		mpf f = mpi::rand(MPF_ORIGINAL_PRECISION);
		MILLI_TIME(f.to_radix(10, MPF_ORIGINAL_PRECISION), f.to_radix);
		MPF_ORIGINAL_PRECISION <<= 1;
	}

	return 0;
}
int main4()
{
	MPF_ORIGINAL_PRECISION = 32;
	mpf a, b, c, d;
	a = "0";
	c = "0.05";
	d = 1;
	int n = 7;
	unsigned long long total_count = 0;
	while (MPF_ORIGINAL_PRECISION <= 4096)
	{
		mpf::const_pi();
		for (int i = 0; i < n; i++)
		{
			MILLI_TIME(const_ln2(), const_ln2);
			total_count += time_count;
		}
		cout << "MPF_PRECISION = " << MPF_ORIGINAL_PRECISION << " takes " << double(total_count) / n << "ms" << endl;
		total_count = 0;
		MPF_ORIGINAL_PRECISION <<= 1;
	}


	return 0;
}
int main5()
{
	MPF_ORIGINAL_PRECISION = 16;
	mpf a, b, c, d;
	a = "0";
	c = "0.05";
	d = 1;

	int n = 7;
	unsigned long long total_count = 0;
	while (MPF_ORIGINAL_PRECISION <= 8192)
	{
		mpf::const_pi();
		for (int i = 0; i < n; i++)
		{
			mpf e = mpi::rand(MPF_ORIGINAL_PRECISION);
			MILLI_TIME(e.to_radix(10, MPF_ORIGINAL_PRECISION * 8), e.to_radix(10));
			total_count += time_count;
			cout << time_count << endl;
		}
		cout << "MPF_PRECISION = " << MPF_ORIGINAL_PRECISION << " takes " << double(total_count) / n << "ms" << endl;
		total_count = 0;
		MPF_ORIGINAL_PRECISION <<= 1;
	}


	return 0;
}
int main6()
{
	time_cout = false;
	srand(unsigned(time(NULL)));
	mpi::srand(time(NULL));
	for (int i = 0; i < 1000; i++)
	{
		mpf a = mpi::rand(rand() % MPF_ORIGINAL_PRECISION);
		a.mant.sign = rand() % 2;
		a.exp = rand() % 1000000;
		a.mant.sign = (rand() & 1) == 0 ? 1 : -1;

		mpf b = mpi::rand(rand() % MPF_ORIGINAL_PRECISION);
		b.exp = rand() % 1000000;
		b.mant.sign = (rand() & 1) == 0 ? 1 : -1;

		mpf c1, c2;

		MILLI_TIME(mpf::add(a, b, c1), mpf::add);
		MILLI_TIME(mpf_fast_add(a, b, c2), mpf_add_fast);

		cout << i + 1 << ". " << (mpf::cmp(c2, c2) == ord::eq ? "TRUE" : "FALSE") << endl;
	}

	mpf a = 1;
	mpf::sin(a, ang::rad, a);
	cout << a << endl;
	/*0.84147098*/
	/*0.84147098*/



	return 0;
}

void mpf_srand(unsigned int seed)
{
	mpi::srand(seed);
}

mpf mpf_rand(int mant_len = int MPF_PRECISION, int min_exp = 0, int max_exp = 0)
{
	mpf f = mpi::rand(size_t(mant_len));
	if (max_exp != 0)
	{
		f.exp = min_exp + f.mant.dp[0] % (max_exp - min_exp);
	}
	f.exp *= (f.mant.dp[1] & 1) == 0 ? 1 : -1;
	return f;
}

vector<mpf> generate_mpf(int n_sample, int opnd_length, int min_exp, int max_exp, unsigned int seed)
{
	mpf_srand(seed);
	vector<mpf> result;
	for (int i = 0; i < n_sample; i++)
	{
		result.emplace_back(mpf_rand(opnd_length, max_exp));
	}
	return result;
}

void test_mpf_binary_operation_time(int n_sample, int opnd1_length, int opnd2_length, int min_exp, int max_exp, unsigned int seed, string_view optr_name, void (*func)(mpf const& a, mpf const& b, mpf& c))
{
	cout << "--------------------[" << optr_name << "]耗时测试--------------------" << endl;
	cout << "第一个操作数的长度为：" << opnd1_length * MPI_DIGIT_BIT << "bits" << endl;
	cout << "第二个操作数的长度为：" << opnd2_length * MPI_DIGIT_BIT << "bits" << endl;
	cout << "随机数种子为：0x" << hex << uppercase << seed << dec << endl;
	cout << "执行[" << optr_name << "]次数：" << n_sample << endl;
	auto opnd1 = generate_mpf(n_sample, opnd1_length, min_exp, max_exp, seed);
	auto opnd2 = generate_mpf(n_sample, opnd2_length, min_exp, max_exp, seed << 1);
	mpf r;
	MILLI_TIME(
		for (int i = 0; i < n_sample; i++)
		{
			func(opnd1[i], opnd2[i], r);
		}
	, 乘法总耗时);
	cout << "平均每次[" << optr_name << "]耗时：" << double(time_count) / n_sample << "ms" << endl;
}

mpf mpf_rand_dec()
{
	mpf r = mpf_rand(MPF_ORIGINAL_PRECISION);
	r.exp = -MPF_ORIGINAL_PRECISION;
	r.mant.sign = 1;
	return r;
}

void test_mpf_unary_operation_time(int n_sample, mpf const& opnd, bool plus_m, string_view optr_name, void (*func)(mpf const& a, mpf& b))
{
	cout << "--------------------[" << optr_name << "]耗时测试--------------------" << endl;
	cout << "执行[" << optr_name << "]次数：" << n_sample << endl;
	cout << "x = " << opnd << endl;
	vector<mpf> opnds;
	mpf r;
	for (int i = 0; i < n_sample; i++)
	{
		if (plus_m)
		{
			mpf::add(opnd, mpf_rand_dec(), r);
			opnds.push_back(r);
		}
		else
		{
			opnds.push_back(opnd);
		}
	}
	MILLI_TIME(
		for (int i = 0; i < n_sample; i++)
		{
			func(opnds[i], r);
		}
	, 总耗时);
	cout << "平均每次[" << optr_name << "]耗时：" << double(time_count) / n_sample << "ms" << endl;
}

void test_mpf_trig_operation_time(int n_sample, mpf const& opnd, bool plus_m, string_view optr_name, void (*func)(mpf const& a, ang ang_t, mpf& b))
{
	cout << "--------------------[" << optr_name << "]耗时测试--------------------" << endl;
	cout << "执行[" << optr_name << "]次数：" << n_sample << endl;
	cout << "x = " << opnd << endl;
	vector<mpf> opnds;
	mpf r;
	for (int i = 0; i < n_sample; i++)
	{
		if (plus_m)
		{
			mpf::add(opnd, mpf_rand_dec(), r);
			opnds.push_back(r);
		}
		else
		{
			opnds.push_back(opnd);
		}
	}
	MILLI_TIME(
		for (int i = 0; i < n_sample; i++)
		{
			func(opnds[i], ang::rad, r);
		}
	, 总耗时);
	cout << "平均每次[" << optr_name << "]耗时：" << double(time_count) / n_sample << "ms" << endl;
}

void test_mpf_add_time()
{
	test_mpf_binary_operation_time(10000, 64, 64, 0, 100, unsigned int(CLOCK_NOW), "加法", mpf::add);
	test_mpf_binary_operation_time(10000, 128, 128, 0, 100, unsigned int(CLOCK_NOW), "加法", mpf::add);
	test_mpf_binary_operation_time(10000, 64, 64, 100, 10000, unsigned int(CLOCK_NOW), "加法", mpf::add);
	test_mpf_binary_operation_time(10000, 128, 128, 100, 10000, unsigned int(CLOCK_NOW), "加法", mpf::add);
	test_mpf_binary_operation_time(10000, 64, 64, 10000, 1000000, unsigned int(CLOCK_NOW), "加法", mpf::add);
	test_mpf_binary_operation_time(10000, 128, 128, 10000, 1000000, unsigned int(CLOCK_NOW), "加法", mpf::add);
}

void test_mpf_sub_time()
{
	test_mpf_binary_operation_time(10000, 64, 64, 0, 100, unsigned int(CLOCK_NOW), "减法", mpf::sub);
	test_mpf_binary_operation_time(10000, 128, 128, 0, 100, unsigned int(CLOCK_NOW), "减法", mpf::sub);
	test_mpf_binary_operation_time(10000, 64, 64, 100, 10000, unsigned int(CLOCK_NOW), "减法", mpf::sub);
	test_mpf_binary_operation_time(10000, 128, 128, 100, 10000, unsigned int(CLOCK_NOW), "减法", mpf::sub);
	test_mpf_binary_operation_time(10000, 64, 64, 10000, 1000000, unsigned int(CLOCK_NOW), "减法", mpf::sub);
	test_mpf_binary_operation_time(10000, 128, 128, 10000, 1000000, unsigned int(CLOCK_NOW), "减法", mpf::sub);
}

void sinh_expt(mpf const& a, mpf& c)
{
	c = a;
	mpf t;
	mpf::e_expt(c, c);
	mpf::inv(c, t);
	mpf::sub(c, t, c);
	mpf::div_2(c, c);
}

void test_extra_precision_out()
{
	MPF_ORIGINAL_PRECISION = 2;
	MPF_EXTRA_PRECISION = 1;
	mpf ff = 6;
	mpf r1;
	mpf::sinh_taylor(ff, r1);

	mpf r2;
	sinh_expt(ff, r2);

	MPF_ORIGINAL_PRECISION = 128;

	cout << r1.to_str(10, mpl::num_format::flo, 32) << endl;
	cout << r2.to_str(10, mpl::num_format::flo, 32) << endl;

	r1.output_constant(cout, "r1");
	r2.output_constant(cout, "r2");

}

mpi::init_struct i1 = { 1,4,{1,3,5,7} };
mpi::init_struct i2 = { 1,4,{2,4,6,8} };

int main()
{
	CalculatorDisplayImpl calc_display;
	CalculatorEngine ce(CalculatorMode::Scientific, &calc_display, nullptr);
	ce.ProcessCommand(FACT);
	ce.ProcessCommand(NUM_1);
	ce.ProcessCommand(NUM_0);
	ce.ProcessCommand(NUM_0);
	ce.ProcessCommand(NUM_0);
	ce.ProcessCommand(NUM_0);
	ce.ProcessCommand(NUM_0);
	ce.ProcessCommand(NUM_0);

	ce.SetExpressionDisplay();
	ce.ProcessCommand(EQU);
	ce.SetPrimaryDisplay();
	return 0;
}

#pragma warning(pop)
