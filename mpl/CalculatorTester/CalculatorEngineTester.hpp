#pragma once
#include "../CalculatorManagerLib/CalculatorEngine.h"
#include "ExpressionCollectorTester.hpp"
#include "InterfaceImpl.hpp"
#include <iomanip>
CM_BEGIN

class CalcualtorEngineTester
{
public:

	ResourceProviderImpl m_res_provider;
	CalculatorHistoryImpl m_calc_hist;
	SerializeCommandVisitorImpl m_serialize_cmd_display;
	CalculatorDisplayImpl m_calc_disp;

	CalculatorEngine m_stand{ CalculatorMode::Standard,&m_calc_disp,&m_calc_hist };
	CalculatorEngine m_scien{ CalculatorMode::Scientific,&m_calc_disp,&m_calc_hist };
	CalculatorEngine m_progr{ CalculatorMode::Programmer,&m_calc_disp,&m_calc_hist };
	ExpressionCollectorTester ect{};

	CalculatorEngine* ce = nullptr;

	CalcualtorEngineTester()
	{

		Resource::LoadRresource(&m_res_provider);
	}

	void SetExpressionDisplay(ExpressionCommands const& commands, ExpressionTokens const& tokens)
	{
		for (auto const& token : *tokens)
		{
			std::cout << token << ' ';
		}
	}

	void TestCalculateExpression()
	{

#define CALC_EXP(a) std::cout << a << std::endl;\
	ect.Analy(a);\
	ect.ec.CompleteEquation();\
	SetExpressionDisplay(ect.ec.GetCommands(), ect.ec.GetTokens());\
	std::cout << ce->CalculateExpression(ect.ec.GetCommands()).to_str() << std::endl;

		ce = &m_scien;
		ect.ec.SetCalculatorMode(CalculatorMode::Scientific);

		CALC_EXP("1 + 2 + 3");
		CALC_EXP("1 + 2 * 3 + 3");
		CALC_EXP("( 1 + 2 * 3 ) / 2 + 1");
		CALC_EXP("( 1 + 2 * ( 3 + 4 * 5 ) ) / ( 2 * 3 + 4 * 5 * 2 + 1 )");
		CALC_EXP("( 1 + 2 * 3 ) / Sin 29 + 2 * Sin 10 + 4 * 5 ) )");
		CALC_EXP("(");
		CALC_EXP("123456");
		CALC_EXP("( ( ( ( ( ( 123456");
		CALC_EXP("");
		CALC_EXP("( ( ( ( ( ( (");
		CALC_EXP("6 / 3 * 4")

			system("pause");

		CALC_EXP("41 / 6334 + 19169 + 11478 * 26962 + 5705 - 23281 / 9961 / 2995 * 4827 + 32391 + 3902 - 292 * 17421 + 19718 / 5447 * 14771 * 1869 + 25667 / 17035 * 28703 / 31322 - 17673 + 15141 / 28253 + 25547 + 32662 - 20037 / 8723 - 27529 * 12316 / 22190 * 288 * 9040 * 19264 + 27446 - 15890 - 24370 * 15006 - 24393 + 19629 / 24084 * 18756 + 4966 + 13931 + 16944 / 24626 / 5537 * 16118 * 22929 - 4833");
		std::cout << std::setprecision(128) << 41.0 / 6334.0 + 19169.0 + 11478.0 * 26962.0 + 5705.0 - 23281.0 / 9961.0 / 2995.0 * 4827.0 + 32391.0 + 3902.0 - 292.0 * 17421.0 + 19718.0 / 5447.0 * 14771.0 * 1869.0 + 25667.0 / 17035.0 * 28703.0 / 31322.0 - 17673.0 + 15141.0 / 28253.0 + 25547.0 + 32662.0 - 20037.0 / 8723.0 - 27529.0 * 12316.0 / 22190.0 * 288.0 * 9040.0 * 19264.0 + 27446.0 - 15890.0 - 24370.0 * 15006.0 - 24393.0 + 19629.0 / 24084.0 * 18756.0 + 4966.0 + 13931.0 + 16944.0 / 24626.0 / 5537.0 * 16118.0 * 22929.0 - 4833.0 << std::endl;
		CALC_EXP("30550 * 20890 + 30550 / 16736 * 19495 * 7074 - 6036 - 27418 + 27048 - 27865 + 31160 / 12662 + 28836 + 22890 * 10329 / 28859 - 1178 + 11736 * 18689 + 6084 + 4160 + 22391 / 18605 - 14466 - 23038 - 7409 / 22608 * 26741 - 26735 / 9923 - 14673 + 28983 - 14051 * 22461 + 23049 + 20721 * 14815 / 30085 * 16966 + 2787 / 13212 / 8468 * 20426 * 18401 / 22660 / 7992 * 17100 / 18129 + 17336 / 28944 * 6774");
		std::cout << std::setprecision(128) << 30550.0 * 20890.0 + 30550.0 / 16736.0 * 19495.0 * 7074.0 - 6036.0 - 27418.0 + 27048.0 - 27865.0 + 31160.0 / 12662.0 + 28836.0 + 22890.0 * 10329.0 / 28859.0 - 1178.0 + 11736.0 * 18689.0 + 6084.0 + 4160.0 + 22391.0 / 18605.0 - 14466.0 - 23038.0 - 7409.0 / 22608.0 * 26741.0 - 26735.0 / 9923.0 - 14673.0 + 28983.0 - 14051.0 * 22461.0 + 23049.0 + 20721.0 * 14815.0 / 30085.0 * 16966.0 + 2787.0 / 13212.0 / 8468.0 * 20426.0 * 18401.0 / 22660.0 / 7992.0 * 17100.0 / 18129.0 + 17336.0 / 28944.0 * 6774.0 << std::endl;

#undef CALC_EXP
	}

	bool Analy(std::string& str)
	{
		if (str.empty())
		{
			return false;
		}

		// ´óÐ´×ªÐ¡Ð´
		for (char& c : str)
		{
			if (isupper(c))
			{
				c += 32;
			}
		}
		if (str == "(" || str == "open brace")
		{
			ce->ProcessCommand(OPENP);
		}
		else if (str == ")" || str == "close brace")
		{
			ce->ProcessCommand(CLOSEP);
		}
		else if (str == "%" || str == "precent")
		{
			ce->ProcessCommand(PERCENT);
		}
		else if (str == "ce" || str == "center" || str == "clearenter")
		{
			ce->ProcessCommand(CENTR);
		}
		else if (str == "c" || str == "clear")
		{
			ce->ProcessCommand(CLEAR);
		}
		else if (str == "back" || str == "backspace")
		{
			ce->ProcessCommand(BACK);
		}
		else if (str == "ms")
		{
			ce->ProcessCommand(MSTORE);
		}
		else if (str == "mc")
		{
			ce->ProcessCommand(MCLEAR);
		}
		else if (str == "mr")
		{
			ce->ProcessCommand(MRECALL);
		}
		else if (str == "m+")
		{
			ce->ProcessCommand(MPLUS);
		}
		else if (str == "m-")
		{
			ce->ProcessCommand(MMINUS);
		}
		else if (str == "1/x" || str == "invert")
		{
			ce->ProcessCommand(INV);
		}
		else if (str == "x2" || str == "squ" || str == "square")
		{
			ce->ProcessCommand(SQU);
		}
		else if (str == "\\/x" || str == "sqr" || str == "sqrt" || str == "root")
		{
			ce->ProcessCommand(SQRT);
		}
		else if (str == "+" || str == "plus" || str == "add")
		{
			ce->ProcessCommand(ADD);
		}
		else if (str == "-" || str == "minus" || str == "subtract")
		{
			ce->ProcessCommand(SUB);
		}
		else if (str == "*" || str == "mu" || str == "multiply" || str == "¡Á")
		{
			ce->ProcessCommand(MUL);
		}
		else if (str == "/" || str == "div" || str == "divide" || str == "¡Â")
		{
			ce->ProcessCommand(DIV);
		}
		else if (str == "=" || str == "equ" || str == "equa")
		{
			ce->ProcessCommand(EQU);
		}
		else if (str == "." || str == "decimalpoint")
		{
			ce->ProcessCommand(PNT);
		}
		else if (str == "+/-" || str == "sign" || str == "negate")
		{
			ce->ProcessCommand(SIGN);
		}
		else if (str[0] <= '9' && str[0] >= '0')
		{
			for (char c : str)
			{
				if (c > '9' || c < '0')
				{
					return false;
				}
			}

			for (char c : str)
			{
				ce->ProcessCommand(int(c) - int('0') + NUM_0);
			}
		}
		else if (str == "cls")
		{
			system("cls");
		}
		else if (str == "exp")
		{
			ce->ProcessCommand(EXP);
		}
		else
		{
			return false;
		}
		return true;
	}

	void TestProcessCommand_StandardMode()
	{
		ce = &m_stand;
		std::string str;
		std::cout << ">>> ";
		std::getline(std::cin, str);
		while (str != "exit")
		{
			if (!str.empty())
			{
				bool f = Analy(str);
				if (f)
				{
					ce->SetExpressionDisplay();
					ce->SetPrimaryDisplay();
				}
				else
				{
					std::cout << "ERROR COMMAND!" << std::endl;
				}
			}
			std::cout << ">>> ";
			std::getline(std::cin, str);
		}
	}
	void TestProcessCommand_ScientificMode()
	{
		ce = &m_scien;
		std::string str;
		std::cout << ">>> ";
		std::getline(std::cin, str);
		while (str != "exit")
		{
			if (!str.empty())
			{
				bool f = Analy(str);
				if (f)
				{
					ce->SetExpressionDisplay();
					ce->SetPrimaryDisplay();
				}
				else
				{
					std::cout << "ERROR COMMAND!" << std::endl;
				}
			}
			std::cout << ">>> ";
			std::getline(std::cin, str);
		}
	}
};

CM_END