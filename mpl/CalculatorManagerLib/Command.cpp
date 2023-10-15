#include "Command.h"
#include <unordered_map>
using namespace std;
CM_BEGIN

CommandType GetCommandType(Command cmd)
{
	if (INRANGE(cmd, NUM_BEGIN, NUM_END))
	{
		return CommandType::NumberDigit;
	}
	else if (INRANGE(cmd, BIT_BEGIN, BIT_END))
	{
		return CommandType::BitPosition;
	}
	else if (INRANGE(cmd, UNARY_CMD_BEGIN, UNARY_CMD_END))
	{
		return CommandType::Unary;
	}
	else if (INRANGE(cmd, BINARY_CMD_BEGIN, BINARY_CMD_END))
	{
		return CommandType::Binary;
	}
	else if (INRANGE(cmd, GUI_CMD_BEGIN, GUI_CMD_END))
	{
		return CommandType::GUISetting;
	}
	return CommandType::Null;
}

#define CHOOSE_DIR(left,right) \
if(shift_direction == LSH) return left;\
else if(shift_direction == RSH) return right;\
else return CMD_NULL;

Command GetShiftCommand(ShiftType shift_type, Command shift_direction)
{
	switch (shift_type)
	{
	case ShiftType::Arthmetic:
		CHOOSE_DIR(LSHA, RSHA);
	case ShiftType::Logic:
		CHOOSE_DIR(LSHL, RSHL);
	case ShiftType::Rotate:
		CHOOSE_DIR(ROL, ROR);
	case ShiftType::RotateCircle:
		CHOOSE_DIR(ROLC, RORC);
	default:
		return CMD_NULL;
	}
}

#define CHOOSE_TRIG(x) \
if (hyp) \
{\
	return inv?AR##x##H:x##H;\
}\
else \
{\
	switch (angle_type)\
	{\
	case AngleType::Degree:\
		return inv ? x##_DEG : ARC##x##_DEG;\
	case AngleType::Radians:\
		return inv ? x##_RAD : ARC##x##_RAD;\
	case AngleType::Gradians:\
		return inv ? x##_GRAD : ARC##x##_GRAD;\
	default: return CMD_NULL;\
	}\
}

Command GetTrigonometricFunctionCommand(TrigonometricFunction tf, AngleType angle_type, bool inv, bool hyp)
{
	switch (tf)
	{
	case TrigonometricFunction::Sine:
		CHOOSE_TRIG(SIN);
		break;
	case TrigonometricFunction::Cosine:
		CHOOSE_TRIG(COS);
		break;
	case TrigonometricFunction::Tangent:
		CHOOSE_TRIG(TAN);
		break;
	case TrigonometricFunction::Secant:
		CHOOSE_TRIG(SEC);
		break;
	case TrigonometricFunction::Cosecant:
		CHOOSE_TRIG(CSC);
		break;
	case TrigonometricFunction::Cotangent:
		CHOOSE_TRIG(COT);
		break;
	default:
		return CMD_NULL;
	}
}

Command GetInvertCommand(Command cmd)
{
	switch (cmd)
	{
	case INT: return FRAC;
	case SQU: return CUB;
	case SQRT: return CUBT;
	case PWR: return YROOT;
	case PWR10: return PWR2;
	case LOG: return LOGBASEY;
	case LN: return PWRE;
	case FRAC: return INT;
	case CUB: return SQU;
	case CUBT: return SQRT;
	case YROOT: return PWR;
	case PWR2: return PWR10;
	case LOGBASEY: return LOG;
	case PWRE: return LN;
	}

	return cmd;
}

// 返回一个二元运算符的算术优先级，-1 为该操作符优先级未知。
int GetBinaryCommandPrecedence(Command cmd)
{
	static int command_precedence[] =
	{
		OR,0,XOR,0,
		AND,1,NAND,1,NOR,1,
		ADD,2,SUB,2,
		LSHA,3,LSHL,3,RSHA,3,RSHL,3,
		MUL,4,DIV,4,REM,4,MOD,4,
		PWR,5,YROOT,5,LOGBASEY,5
	};

	for (int i = 0; i < size(command_precedence); i += 2)
	{
		if (command_precedence[i] == cmd)
		{
			return command_precedence[i + 1];
		}
	}

	return -1;
}

int CompareBinaryCommandPrecedence(Command cmd1, Command cmd2)
{
	return GetBinaryCommandPrecedence(cmd1) - GetBinaryCommandPrecedence(cmd2);
}

CM_END