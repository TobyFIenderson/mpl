#include "CalculatorEngine.h"
using namespace std;
using namespace mpl;
CM_BEGIN
mpl::mpf CalculatorEngine::DoUnaryOperation(mpl::mpf const& rat, Command uncmd)
{
	mpl::mpf result{};
	try
	{
		switch (uncmd)
		{
		case ROR:
			result = RotateRightShift(rat, current_number_width);
			break;
		case ROL:
			result = RotateLeftShift(rat, current_number_width);
			break;
		case RORC:
			result = RotateRightShiftCircle(rat, current_number_width, carry_bit);
			break;
		case ROLC:
			result = RotateLeftShiftCircle(rat, current_number_width, carry_bit);
			break;
		case NOT:
			result = Not(rat, current_number_width);
			break;
		case DMS:
			result = DegreeToDegreeMinuteSecond(rat);
			break;
		case DEGREE:
			result = DegreeMinuteSecondToDegree(rat);
			break;
		case FRAC:
			result = Fraction(rat);
			break;
		case INT:
			result = Integer(rat);
			break;
		case SIN_DEG:
			result = Sin(rat, AngleType::Degree);
			break;
		case SIN_RAD:
			result = Sin(rat, AngleType::Radians);
			break;
		case SIN_GRAD:
			result = Sin(rat, AngleType::Gradians);
			break;
		case COS_DEG:
			result = Cos(rat, AngleType::Degree);
			break;
		case COS_RAD:
			result = Cos(rat, AngleType::Radians);
			break;
		case COS_GRAD:
			result = Cos(rat, AngleType::Gradians);
			break;
		case TAN_DEG:
			result = Tan(rat, AngleType::Degree);
			break;
		case TAN_RAD:
			result = Tan(rat, AngleType::Radians);
			break;
		case TAN_GRAD:
			result = Sin(rat, AngleType::Gradians);
			break;
		case SINH:
			result = SinH(rat);
			break;
		case COSH:
			result = CosH(rat);
			break;
		case TANH:
			result = TanH(rat);
			break;
		case ARCSIN_DEG:
			result = ArcSin(rat, AngleType::Degree);
			break;
		case ARCSIN_RAD:
			result = ArcSin(rat, AngleType::Radians);
			break;
		case ARCSIN_GRAD:
			result = ArcSin(rat, AngleType::Gradians);
			break;
		case ARCCOS_DEG:
			result = ArcCos(rat, AngleType::Degree);
			break;
		case ARCCOS_RAD:
			result = ArcCos(rat, AngleType::Radians);
			break;
		case ARCCOS_GRAD:
			result = ArcCos(rat, AngleType::Gradians);
			break;
		case ARCTAN_DEG:
			result = ArcTan(rat, AngleType::Degree);
			break;
		case ARCTAN_RAD:
			result = ArcTan(rat, AngleType::Radians);
			break;
		case ARCTAN_GRAD:
			result = ArcTan(rat, AngleType::Gradians);
			break;
		case ARSINH:
			result = ArSinH(rat);
			break;
		case ARCOSH:
			result = ArCosH(rat);
			break;
		case ARTANH:
			result = ArTanH(rat);
			break;
		case SEC_DEG:
			result = Sec(rat, AngleType::Degree);
			break;
		case SEC_RAD:
			result = Sec(rat, AngleType::Radians);
			break;
		case SEC_GRAD:
			result = Sec(rat, AngleType::Gradians);
			break;
		case CSC_DEG:
			result = Csc(rat, AngleType::Degree);
			break;
		case CSC_RAD:
			result = Csc(rat, AngleType::Radians);
			break;
		case CSC_GRAD:
			result = Csc(rat, AngleType::Gradians);
			break;
		case COT_DEG:
			result = Cot(rat, AngleType::Degree);
			break;
		case COT_RAD:
			result = Cot(rat, AngleType::Radians);
			break;
		case COT_GRAD:
			result = Cot(rat, AngleType::Gradians);
			break;
		case SECH:
			result = SecH(rat);
			break;
		case CSCH:
			result = CscH(rat);
			break;
		case COTH:
			result = CotH(rat);
			break;
		case ARCSEC_DEG:
			result = ArcSec(rat, AngleType::Degree);
			break;
		case ARCSEC_RAD:
			result = ArcSec(rat, AngleType::Radians);
			break;
		case ARCSEC_GRAD:
			result = ArcSec(rat, AngleType::Gradians);
			break;
		case ARCCSC_DEG:
			result = ArcCsc(rat, AngleType::Degree);
			break;
		case ARCCSC_RAD:
			result = ArcCsc(rat, AngleType::Radians);
			break;
		case ARCCSC_GRAD:
			result = ArcCsc(rat, AngleType::Gradians);
			break;
		case ARCCOT_DEG:
			result = ArcCot(rat, AngleType::Degree);
			break;
		case ARCCOT_RAD:
			result = ArcCot(rat, AngleType::Radians);
			break;
		case ARCCOT_GRAD:
			result = ArcCot(rat, AngleType::Gradians);
			break;
		case ARSECH:
			result = ArSecH(rat);
			break;
		case ARCSCH:
			result = ArCscH(rat);
			break;
		case ARCOTH:
			result = ArCotH(rat);
			break;
		case LN:
			result = Ln(rat);
			break;
		case LOG:
			result = Log(rat);
			break;
		case INV:
			result = Invert(rat);
			break;
		case SQU:
			result = Square(rat);
			break;
		case SQRT:
			result = SquareRoot(rat);
			break;
		case CUB:
			result = Cube(rat);
			break;
		case CUBT:
			result = CubeRoot(rat);
			break;
		case PWR10:
			result = Power10(rat);
			break;
		case PWR2:
			result = Power2(rat);
			break;
		case PWRE:
			result = Exp(rat);
			break;
		case PERCENT: // 特殊的指令
		{
			// 百分比函数是一个比较特殊的函数
			// 如果上一个运算符不为乘除那么执行以下运算：
			// X [op] Y % ---> X [op] ( X * Y% )
			// 不然的话：
			// X [op] Y % ---> X [op] Y%
			if (last_bin_command == DIV || last_bin_command == MUL)
			{
				mpf::div(rat, 100, result);
			}
			else
			{
				mpf::div(last_value, 100, result);
				mpf::mul(result, rat, result);
			}
		}
		break;
		case ABS:
			result = rat;
			result.mant.sign = 1;
			break;
		case FLOOR:
			result = Floor(rat);
			break;
		case CEIL:
			result = Ceil(rat);
			break;
		case NEG:
			result = rat;
			result.mant.sign *= -1;
			break;
		}
	}
	catch (ErrorCode err_code)
	{
		ProcessError(err_code);
		result = 0;
	}

	return result;
}

mpl::mpf CalculatorEngine::DoBinaryOperation(mpl::mpf const& x, mpl::mpf const& y, Command bincmd)
{
	mpl::mpf result{};
	try
	{
		switch (bincmd)
		{
		case ADD:
			mpf::add(x, y, result);
			break;
		case SUB:
			mpf::sub(x, y, result);
			break;
		case MUL:
			mpf::mul(x, y, result);
			break;
		case DIV:
			mpf::div(x, y, result);
			break;
		case REM:
			result = Remainder(x, y);
			break;
		case MOD:
			result = Mod(x, y);
			break;
		case LSHA:
			result = LeftShiftArithmetic(x, y, current_number_width);
			break;
		case LSHL:
			result = LeftShiftLogical(x, y, current_number_width);
			break;
		case RSHA:
			result = RightShiftArithmetic(x, y, current_number_width);
			break;
		case RSHL:
			result = RightShiftLogical(x, y, current_number_width);
			break;
		case AND:
			result = And(x, y, current_number_width);
			break;
		case OR:
			result = Or(x, y, current_number_width);
			break;
		case XOR:
			result = Xor(x, y, current_number_width);
			break;
		case NAND:
			result = Nand(x, y, current_number_width);
			break;
		case NOR:
			result = Nor(x, y, current_number_width);
			break;
		case PWR:
			result = Power(x, y);
			break;
		case YROOT:
			result = Root(x, y);
			break;
		case LOGBASEY:
			result = LogXY(x, y);
			break;
		}
	}
	catch (ErrorCode err_code)
	{
		ProcessError(err_code);
		result = 0;
	}

	return result;
}

mpl::mpf CalculatorEngine::ToggleBit(mpl::mpf const& rat, uint32_t bit_index)
{
	if (bit_index >= uint32_t(current_number_width))
	{
		throw MPL_ERR_INVALID_VAL;
	}

	/* 这里就不实现了。以后再说 */
	mpf todo = rat;
	// mpl::mpf result = (rat.is_zero() ? mpf(0) : rat).integer() ^ Power(mpl::mpf::Two, bit_index);

	//return move(result);
	return todo;
}
CM_END