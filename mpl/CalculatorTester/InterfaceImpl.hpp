#pragma once
#include "../CalculatorManagerLib/CalculatorManager.h"
#include "../CalculatorManagerLib/Resource.h"
#include "../CalculatorManagerLib/ResourceStringID.h"
CM_BEGIN
class ResourceProviderImpl : public IResourceProvider
{
public:
	std::string GetResourceString(std::string_view sid) override
	{
		static std::unordered_map<std::string_view, std::string> RESSTR
		{
			{RS_NUM_0, "0"},
			{RS_NUM_1, "1"},
			{ RS_NUM_2, "2" },
			{ RS_NUM_3, "3" },
			{ RS_NUM_4, "4" },
			{ RS_NUM_5, "5" },
			{ RS_NUM_6, "6" },
			{ RS_NUM_7, "7" },
			{ RS_NUM_8, "8" },
			{ RS_NUM_9, "9" },
			{ RS_NUM_A, "A" },
			{ RS_NUM_B, "B" },
			{ RS_NUM_C, "C" },
			{ RS_NUM_D, "D" },
			{ RS_NUM_E, "E" },
			{ RS_NUM_F, "F" },
			{ RS_ADD, "+" },
			{ RS_SUB, "-" },
			{ RS_MUL, "×" },
			{ RS_DIV, "÷" },
			{ RS_REM, "%" },
			{ RS_MOD, "mod" },
			{ RS_LSHA, "<<" },
			{ RS_LSHL, "<<" },
			{ RS_RSHA, ">>" },
			{ RS_RSHL, "<<" },
			{ RS_AND, "&" },
			{ RS_OR, "|" },
			{ RS_XOR, "^" },
			{ RS_NAND, "NAND" },
			{ RS_NOR, "NOR" },
			{ RS_PWR, "^" },
			{ RS_YROOT, "yroot" },
			{ RS_LOGBASEY, "logbase" },
			{ RS_ROR, "ROR" },
			{ RS_ROL, "ROL" },
			{ RS_RORC, "RORC" },
			{ RS_ROLC, "ROLC" },
			{ RS_NOT, "~" },
			{ RS_DMS, "DMS" },
			{ RS_DEGREE, "DEG" },
			{ RS_FRAC, "Fraction" },
			{ RS_INT, "Integer" },
			{ RS_SIN_DEG, "sin" },
			{ RS_SIN_RAD, "sin" },
			{ RS_SIN_GRAD, "sin" },
			{ RS_COS_DEG, "cos" },
			{ RS_COS_RAD, "cos" },
			{ RS_COS_GRAD, "cos" },
			{ RS_TAN_DEG, "tan" },
			{ RS_TAN_RAD, "tan" },
			{ RS_TAN_GRAD, "tan" },
			{ RS_SINH, "sinh" },
			{ RS_COSH, "cosh" },
			{ RS_TANH, "tanh" },
			{ RS_ARCSIN_DEG, "arcsin" },
			{ RS_ARCSIN_RAD, "arcsin" },
			{ RS_ARCSIN_GRAD, "arcsin" },
			{ RS_ARCCOS_DEG, "arccos" },
			{ RS_ARCCOS_RAD, "arccos" },
			{ RS_ARCCOS_GRAD, "arccos" },
			{ RS_ARCTAN_DEG, "arctan" },
			{ RS_ARCTAN_RAD, "arctan" },
			{ RS_ARCTAN_GRAD, "arctan" },
			{ RS_ARSINH, "arcsinh" },
			{ RS_ARCOSH, "arccosh" },
			{ RS_ARTANH, "arctanh" },
			{ RS_SEC_DEG, "sec" },
			{ RS_SEC_RAD, "sec" },
			{ RS_SEC_GRAD, "sec" },
			{ RS_CSC_DEG, "csc" },
			{ RS_CSC_RAD, "csc" },
			{ RS_CSC_GRAD, "csc" },
			{ RS_COT_DEG, "cot" },
			{ RS_COT_RAD, "cot" },
			{ RS_COT_GRAD, "cot" },
			{ RS_SECH, "sech" },
			{ RS_CSCH, "sech" },
			{ RS_COTH, "coth" },
			{ RS_ARCSEC_DEG, "arcsec" },
			{ RS_ARCSEC_RAD, "arcsec" },
			{ RS_ARCSEC_GRAD, "aecsec" },
			{ RS_ARCCSC_DEG, "arccsc" },
			{ RS_ARCCSC_RAD, "arccsc" },
			{ RS_ARCCSC_GRAD, "arccsc" },
			{ RS_ARCCOT_DEG, "arccot" },
			{ RS_ARCCOT_RAD, "arccot" },
			{ RS_ARCCOT_GRAD, "arccot" },
			{ RS_ARSECH, "arcsech" },
			{ RS_ARCSCH, "aeccsch" },
			{ RS_ARCOTH, "arccot" },
			{ RS_LN, "ln" },
			{ RS_LOG, "log" },
			{ RS_INV, "1/" },
			{ RS_SQU, "sqr" },
			{ RS_SQRT, "sqrt" },
			{ RS_CUB, "cube" },
			{ RS_CUBT, "cubet" },
			{ RS_PWR10, "10^" },
			{ RS_PWR2, "2^" },
			{ RS_PWRE, "e^" },
			{ RS_PERCENT, "%" },
			{ RS_ABS, "abs" },
			{ RS_FLOOR, "floor" },
			{ RS_CEIL, "ceil" },
			{ RS_NEG, "-" },
			{ RS_EXP, "exp" },
			{ RS_LSH, "<<" },
			{ RS_RSH, ">>" },
			{ RS_SHA, "SHA" },
			{ RS_SHL, "SHL" },
			{ RS_ROSH, "ROSH" },
			{ RS_RCSH, "RCSH" },
			{ RS_CLEAR, "C" },
			{ RS_CENTR, "CE" },
			{ RS_BACK, "BackSpace" },
			{ RS_OPENP, "(" },
			{ RS_CLOSEP, ")" },
			{ RS_EQU, "=" },
			{ RS_PNT, "." },
			{ RS_SIGN, "-" },
			{ RS_MSTORE, "MS" },
			{ RS_MRECALL, "MR" },
			{ RS_MPLUS, "M+" },
			{ RS_MMINUS, "M-" },
			{ RS_MCLEAR, "MC" },
			{ RS_BIN, "BIN" },
			{ RS_OCT, "OCT" },
			{ RS_DEC, "DEC" },
			{ RS_HEX, "HEX" },
			{ RS_BYTE, "BYTE" },
			{ RS_WORD, "WORD" },
			{ RS_DWORD, "DWORD" },
			{ RS_QWORD, "QWORD" },
			{ RS_DEG, "DEG" },
			{ RS_RAD, "RAD" },
			{ RS_GRAD, "GRAD" },
			{ RS_FLOFMT, "F-E" },
			{ RS_SCIFMT, "SCI" },
			{ RS_ENGFMT, "ENG" },
			{ RS_STDMODE, "标准" },
			{ RS_SCIMODE, "科学" },
			{ RS_PROMODE, "程序员" },
			{ RS_INVC, "2nd" },
			{ RS_HYP, "hyp" },
			{ RS_SIN, "sin" },
			{ RS_COS, "cos" },
			{ RS_TAN, "tan" },
			{ RS_SEC, "sec" },
			{ RS_CSC, "csc" },
			{ RS_COT, "cot" },
			{ RS_PI, "pi" },
			{ RS_EULER, "e" },
			{ RS_RAND, "rand" },
			{ RS_SET_RET, "=" },
			{ RS_CMD_NULL, "RS_CMD_NULL" },
			{ RS_MPL_ERR_INVALID_VAL, "输入超限、无效输入" },
			{ RS_MPL_ERR_OUT_OF_MEM, "堆内存溢出" },
			{ RS_MPL_ERR_MAX_ITER_REACHED, "达到最大迭代系数" },
			{ RS_MPL_ERR_BUF_OVERFLOW, "缓存溢出" },
			{ RS_MPL_ERR_UNKNOWN_ERR, "未知错误" },
			{ RS_MPL_ERR_RESULT_UNDEF, "结果未定义" },
			{ RS_MPL_ERR_DIV_BY_ZERO, "除数不能为零" },
			{ RS_MPL_ERR_RESULT_INF, "结果是无穷大" },
			{ RS_MPL_ERR_NULL, "RS_MPL_ERR_NULL" },
			{ RS_CE_ERR_INPUT_OVERFLOW, "输入过长" },
			{ RS_CE_ERR_OUTPUT_OVERFLOW, "生成的字符串过长" },
			{ RS_CE_ERR_UNKNOW_NUM_REPRESENTATION, "未知的数字表达" },
			{ RS_CE_ERR_WRONG_OPERATOR_CODE, "错误的运算符" },
			{ RS_CE_ERR_NULL, "RS_CE_ERR_NULL" },
			{ RS_GS_ERR_TOO_MANY_SC, "太多的分号了" },
			{ RS_GS_ERR_ILLEGAL_CH_EXIST, "存在非法字符" },
			{ RS_GS_ERR_TOO_MANY_ZERO, "太多的零了" },
			{ RS_GS_ERR_NOT_ZERO_AFTER_ZERO, "零的后面不是零" },
			{ RS_GS_ERR_NULL, "PS_GS_ERR_NULL" }
		};
		
		if (RESSTR.contains(sid))
		{
			return RESSTR[sid];
		}
		return "GET RESOURCE STRING FAILD.";
	}
};

class CalculatorHistoryImpl : public ICalculatorHistory
{
public:
	unsigned int AddToHistory(ExpressionCommands const& commands, ExpressionTokens const& tokens,/* expression 由tokens生成 */std::string_view result) override
	{
		for (auto const& s : *tokens.get())
		{
			std::cout << s << " ";
		}

		std::cout << result << std::endl;
		return 0;
	}
	void Clear() override
	{
		std::cout << "CalculatorHistoryImpl::Clear called." << std::endl;
	}
};

class SerializeCommandVisitorImpl : public ISerializeCommandVisitor
{
public:
	void Visit(OperandCommand& opndCmd) override
	{
		auto const& cmds = opndCmd.GetCommands();
		for (auto const& cmd : *cmds)
		{
			std::cout << Resource::GetCommandStringIDFromCommand(cmd);
		}
	}
	void Visit(UnaryCommand& unaryCmd) override
	{
		std::cout << Resource::GetCommandStringIDFromCommand(unaryCmd.GetCommand());
	}
	void Visit(BinaryCommand& binaryCmd) override
	{
		std::cout << Resource::GetCommandStringIDFromCommand(binaryCmd.GetCommand());
	}
	void Visit(ParenthesisCommand& paraCmd) override
	{
		std::cout << Resource::GetCommandStringIDFromCommand(paraCmd.GetCommand());
	}
};

class CalculatorDisplayImpl : public ICalculatorDisplay
{
public:
	SerializeCommandVisitorImpl m_cmds_disp;

	void SetPrimaryDisplay(_STD string const& primary_str, bool is_err) override
	{
		// std::cout << "_calc_disp::SetPrimaryDisplay called." << std::endl;
		std::cout << "Primary Display  : " << primary_str << std::endl;
		std::cout << "Is In Error State: " << (is_err ? "True" : "False") << std::endl;
	}

	void SetErrorState(bool is_err) override
	{
		std::cout << "_calc_disp::SetErrorState called." << std::endl;
	}

	void SetExpressionDisplay(ExpressionCommands const& commands, ExpressionTokens const& tokens) override
	{
		//std::cout << "_calc_disp::SetExpressionDisplay called." << std::endl;
		/*for (auto const& cmd : *commands)
		{
			cmd->Accept(m_cmds_disp);
			std::cout << ' ';
		}
		std::cout << std::endl;*/
		std::cout << "Expression Display: ";
		if (tokens != nullptr)
		{
			for (auto const& token : *tokens)
			{
				std::cout << token << " ";
			}
		}
		std::cout << std::endl;
	}

	void OnHistoryItemAdded(unsigned int added_item_index) override
	{
		std::cout << "_calc_disp::OnHistoryItemAdded called." << std::endl;
	}
};

CM_END