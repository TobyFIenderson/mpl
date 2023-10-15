#include "MpfMath.h"
#include <random>
using namespace mpl;
using namespace std;
CM_BEGIN
mpf Sin(mpf const& x, AngleType angle)
{
	mpf y;
	mpf::sin(x, AngleTypeToAng(angle), y);
	return y;
}

mpf Cos(mpf const& x, AngleType angle)
{
	mpf y;
	mpf::cos(x, AngleTypeToAng(angle), y);
	return y;
}

mpf Tan(mpf const& x, AngleType angle)
{
	mpf y;
	mpf::tan(x, AngleTypeToAng(angle), y);
	return y;
}

mpf Sec(mpf const& x, AngleType angle)
{
	mpf y;
	mpf::sec(x, AngleTypeToAng(angle), y);
	return y;
}

mpf Csc(mpf const& x, AngleType angle)
{
	mpf y;
	mpf::csc(x, AngleTypeToAng(angle), y);
	return y;
}

mpf Cot(mpf const& x, AngleType angle)
{
	mpf y;
	mpf::cot(x, AngleTypeToAng(angle), y);
	return y;
}

mpf ArcSin(mpf const& x, AngleType angle)
{
	mpf y;
	mpf::asin(x, AngleTypeToAng(angle), y);
	return y;
}

mpf ArcCos(mpf const& x, AngleType angle)
{
	mpf y;
	mpf::acos(x, AngleTypeToAng(angle), y);
	return y;
}

mpf ArcTan(mpf const& x, AngleType angle)
{
	mpf y;
	mpf::atan(x, AngleTypeToAng(angle), y);
	return y;
}

mpf ArcSec(mpf const& x, AngleType angle)
{
	mpf y;
	mpf::asec(x, AngleTypeToAng(angle), y);
	return y;
}

mpf ArcCsc(mpf const& x, AngleType angle)
{
	mpf y;
	mpf::acsc(x, AngleTypeToAng(angle), y);
	return y;
}

mpf ArcCot(mpf const& x, AngleType angle)
{
	mpf y;
	mpf::acot(x, AngleTypeToAng(angle), y);
	return y;
}

mpf SinH(mpf const& x)
{
	mpf y;
	mpf::sinh(x, y);
	return y;
}

mpf CosH(mpf const& x)
{
	mpf y;
	mpf::cosh(x, y);
	return y;
}

mpf TanH(mpf const& x)
{
	mpf y;
	mpf::tanh(x, y);
	return y;
}

mpf SecH(mpf const& x)
{
	mpf y;
	mpf::sech(x, y);
	return y;
}

mpf CscH(mpf const& x)
{
	mpf y;
	mpf::csch(x, y);
	return y;
}

mpf CotH(mpf const& x)
{
	mpf y;
	mpf::coth(x, y);
	return y;
}

mpf ArSinH(mpf const& x)
{
	mpf y;
	mpf::asinh(x, y);
	return y;
}

mpf ArCosH(mpf const& x)
{
	mpf y;
	mpf::acosh(x, y);
	return y;
}

mpf ArTanH(mpf const& x)
{
	mpf y;
	mpf::atanh(x, y);
	return y;
}

mpf ArSecH(mpf const& x)
{
	mpf y;
	mpf::asech(x, y);
	return y;
}

mpf ArCscH(mpf const& x)
{
	mpf y;
	mpf::acsch(x, y);
	return y;
}

mpf ArCotH(mpf const& x)
{
	mpf y;
	mpf::acoth(x, y);
	return y;
}

mpf LogXY(mpf const& Y, mpf const& X)
{
	mpf x, y;
	mpf::ln(X, x);
	mpf::ln(Y, y);
	mpf::div(x, y, y);
	return y;
}

mpf Log2(mpf const& x)
{
	mpf y;
	mpf::ln(x, y);
	mpf::div(y, mpf::const_ln2(), y);
	return y;
}

mpf Root(mpf const& base, mpf const& pow)
{
	mpf y;
	mpf::root(base, pow, y);
	return y;
}

mpf Power(mpf const& base, mpf const& pow)
{
	mpf y;
	mpf::pow(base, pow, y);
	return y;
}

mpf Power10(mpf const& power)
{
	mpf y;
	mpf::pow(10, power, y);
	return y;
}

mpf Power2(mpf const& power)
{
	mpf y;
	mpf::pow(2, power, y);
	return y;
}

mpf Exp(mpf const& x)
{
	mpf y;
	mpf::e_expt(x, y);
	return y;
}

mpf Cube(mpf const& x)
{
	mpf y;
	mpf::cube(x, y);
	return y;
}

mpf CubeRoot(mpf const& x)
{
	mpf y;
	mpf::cubet(x, y);
	return y;
}

mpf Random()
{
	mpf y(mpi::rand(size_t(MPF_PRECISION)), -MPF_PRECISION);
	y.mant.sign = 1;
	return y;
}

mpf Ln(mpf const& x)
{
	mpf y;
	mpf::ln(x, y);
	return y;
}

mpf Log(mpf const& x)
{
	mpf y;
	mpf::ln(x, y);
	mpf::div(y, mpf::const_ln10(), y);
	return y;
}

mpf Invert(mpf const& x)
{
	mpf y;
	mpf::inv(x, y);
	return y;
}

mpf Square(mpf const& x)
{
	mpf y;
	mpf::sqr(x, y);
	return y;
}

mpf SquareRoot(mpf const& x)
{
	mpf y;
	mpf::sqrt(x, y);
	return y;
}

mpf Fraction(mpf const& x)
{
	return x.fraction();
}

mpf Integer(mpf const& x)
{
	return x.integer();
}

mpf Mod(mpf const& x, mpf const& y)
{
	mpf z;
	mpf::mod(x, y, z);
	if (z.mant.sign != y.mant.sign && !z.is_zero())
	{
		mpf::add(z, y, z);
	}
	return z;
}

mpf Remainder(mpf const& x, mpf const& y)
{
	mpf z;
	mpf::mod(x, y, z);
	return z;
}

mpf Floor(mpf const& x)
{
	mpf y;
	mpf::floor(x, y);
	return y;
}

mpf Ceil(mpf const& x)
{
	mpf y;
	mpf::ceil(x, y);
	return y;
}

void TrueValueToComplement(_Inout_ mpf& xx, NumberWidth const& number_width)
{
	mpi x = xx.to_mpi();
	if (x.is_neg())
	{
		x.sign = 1;
		mpi::xor_tc(x, number_width.GetChopNumber(), x);
		x.incr();
	}
	mpi::and_tc(x, number_width.GetChopNumber(), x);
	xx = move(x);
}

static bool GetMpiMsb(mpi const& x, NumberWidth const& number_width)
{
	int i = (int(number_width) - 1) / MPI_DIGIT_BIT;
	mpi_digit d = i >= x.used ? 0 : x.dp[i];
	i = (int(number_width) - 1 + MPI_DIGIT_BIT) % MPI_DIGIT_BIT;
	if (i != 0)
	{
		d >>= i;
	}
	return (d & 1) == 1;
}

void ComplementToTrueValue(_Inout_ mpf& xx, NumberWidth const& number_width)
{
	mpi x = xx.to_mpi();
	if (GetMpiMsb(x, number_width))
	{
		mpi::xor_tc(x, number_width.GetChopNumber(), x);
		x.incr();
		x.sign = -1;
	}
	mpi::and_tc(x, number_width.GetChopNumber(), x);
	xx = move(x);
}

mpf Not(mpf const& x, NumberWidth const& number_width)
{
	mpi y = x.to_mpi();
	mpi::not_tc(y, y);
	mpi::and_tc(y, number_width.GetChopNumber(), y);
	return y;
}

mpf And(mpf const& x, mpf const& y, NumberWidth const& number_width)
{
	mpi xx = x.to_mpi();
	mpi yy = y.to_mpi();
	mpi::and_tc(xx, yy, yy);
	mpi::and_tc(yy, number_width.GetChopNumber(), yy);
	return yy;
}

mpf Or(mpf const& x, mpf const& y, NumberWidth const& number_width)
{
	mpi xx = x.to_mpi();
	mpi yy = y.to_mpi();
	mpi::or_tc(xx, yy, yy);
	mpi::and_tc(yy, number_width.GetChopNumber(), yy);
	return yy;
}

mpf Xor(mpf const& x, mpf const& y, NumberWidth const& number_width)
{
	mpi xx = x.to_mpi();
	mpi yy = y.to_mpi();
	mpi::xor_tc(xx, yy, yy);
	mpi::and_tc(yy, number_width.GetChopNumber(), yy);
	return yy;
}

mpf Nand(mpf const& x, mpf const& y, NumberWidth const& number_width)
{
	mpf yy = And(x, y, number_width);
	mpi::not_tc(yy.to_mpi(), yy.mant);
	yy.exp = 0;
	return yy;
}

mpf Nor(mpf const& x, mpf const& y, NumberWidth const& number_width)
{
	mpf yy = Or(x, y, number_width);
	mpi::not_tc(yy.to_mpi(), yy.mant);
	yy.exp = 0;
	return yy;
}

/*  逻辑位移不考虑符号位 */
mpf RightShiftLogical(mpf const& x, mpf const& y, NumberWidth const& word_bit_width)
{
	if (y.is_neg() || y.exp < 0 || mpf::cmp(y, int(word_bit_width)) != ord::lt)
	{
		throw MPL_ERR_RESULT_UNDEF;
	}
	if (x.is_neg())
	{
		mpf xx = x;
		TrueValueToComplement(xx, word_bit_width);
		mpi r = xx.to_mpi();
		r.rsh(y.to_mpi().to_u64());
		return r;
	}
	else
	{
		mpi r = x.to_mpi();
		r.rsh(y.to_mpi().to_u64());
		return r;
	}
}

mpf LeftShiftLogical(mpf const& x, mpf const& y, NumberWidth const& word_bit_width)
{
	if (y.is_neg() || y.exp < 0 || mpf::cmp(y, int(word_bit_width)) != ord::lt)
	{
		throw MPL_ERR_RESULT_UNDEF;
	}
	mpi r;
	if (x.is_neg())
	{
		mpf xx = x;
		TrueValueToComplement(xx, word_bit_width);
		r = xx.to_mpi();
		r.lsh(y.to_mpi().to_u64());
	}
	else
	{
		r = x.to_mpi();
		r.rsh(y.to_mpi().to_u64());
	}
	mpf rr = move(r);
	ComplementToTrueValue(rr, word_bit_width);
	return rr;
}

mpf RightShiftArithmetic(mpf const& x, mpf const& y, NumberWidth const& word_bit_width)
{
	if (y.is_neg() || y.exp < 0 || mpf::cmp(y, int(word_bit_width)) != ord::lt)
	{
		throw MPL_ERR_RESULT_UNDEF;
	}
	mpi r = x.to_mpi();
	r.signed_rsh(y.to_mpi().to_u64());
	return r;
}

mpf LeftShiftArithmetic(mpf const& x, mpf const& y, NumberWidth const& word_bit_width)
{
	if (y.is_neg() || y.exp < 0 || mpf::cmp(y, int(word_bit_width)) != ord::lt)
	{
		throw MPL_ERR_RESULT_UNDEF;
	}
	mpf xx = x;
	TrueValueToComplement(xx, word_bit_width);
	mpi xi = xx.to_mpi();
	xi.lsh(y.to_mpi().to_u64());
	xx = move(xi);
	ComplementToTrueValue(xx, word_bit_width);
	return xx;
}

/* 这代码写得太垃圾了，以后优化一下 */
mpf RotateRightShift(mpf const& x, NumberWidth const& word_bit_width)
{
	mpf xx = x;
	TrueValueToComplement(xx, word_bit_width);
	mpi xi = xx.to_mpi();
	mpi_digit lsb = xi.is_zero() ? 0 : (xi.dp[0] & 1);

	mpi tmpxi = x.to_mpi();
	tmpxi.signed_rsh(1);

	mpf tmpxf = move(tmpxi);
	TrueValueToComplement(tmpxf, word_bit_width);
	tmpxi = tmpxf.to_mpi();
	if (lsb == 1)
	{
		xi.set_2expt(int(word_bit_width) - 1);
	}
	else
	{
		xi.set_d(0);
	}
	mpi::or_tc(tmpxi, xi, xi);
	xx = move(xi);
	ComplementToTrueValue(xx, word_bit_width);
	return xx;
}

mpf RotateLeftShift(mpf const& x, NumberWidth const& word_bit_width)
{
	mpf xx = x;
	TrueValueToComplement(xx, word_bit_width);
	mpi xi = xx.to_mpi();
	bool msb = GetMpiMsb(xi, word_bit_width);
	xi.lsh(1);
	mpi::or_tc(xi, mpi_digit(msb), xi);
	xx = move(xi);
	ComplementToTrueValue(xx, word_bit_width);
	return xx;
}

mpf RotateLeftShiftCircle(mpf const& x, NumberWidth const& word_bit_width, _Inout_ uint64_t& carry_bit)
{
	mpf xx = x;
	TrueValueToComplement(xx, word_bit_width);
	mpi xi = xx.to_mpi();
	bool msb = GetMpiMsb(xi, word_bit_width);
	xi.lsh(1);
	mpi::or_tc(xi, carry_bit, xi);
	carry_bit = msb;
	xx = move(xi);
	ComplementToTrueValue(xx, word_bit_width);
	return xx;
}

mpf RotateRightShiftCircle(mpf const& x, NumberWidth const& word_bit_width, _Inout_ uint64_t& carry_bit)
{
	mpf xx = x;
	TrueValueToComplement(xx, word_bit_width);
	mpi xi = xx.to_mpi();
	mpi_digit lsb = xi.is_zero() ? 0 : (xi.dp[0] & 1);

	mpi tmpxi = x.to_mpi();
	tmpxi.signed_rsh(1);

	mpf tmpxf = move(tmpxi);
	TrueValueToComplement(tmpxf, word_bit_width);
	tmpxi = tmpxf.to_mpi();
	if (carry_bit == 1)
	{
		xi.set_2expt(int(word_bit_width) - 1);
	}
	else
	{
		xi.set_d(0);
	}
	mpi::or_tc(tmpxi, xi, xi);
	carry_bit = lsb;
	xx = move(xi);
	ComplementToTrueValue(xx, word_bit_width);
	return xx;
}

mpf DegreeToDegreeMinuteSecond(mpf const& x)
{
	return x;
}

mpf DegreeMinuteSecondToDegree(mpf const& x)
{
	return x;
}
CM_END