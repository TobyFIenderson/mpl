#pragma once
#include "InterfaceImpl.hpp"
#include <iostream>
#include <sstream>
#include "../mpl/mpf/mpf.h"
CM_BEGIN

class ExpressionCollectorTester
{
public:
	ExpressionCollector ec;
	ResourceProviderImpl m_res_provider;
	CalculatorHistoryImpl m_calc_hist;
	SerializeCommandVisitorImpl m_serialize_cmd_display;
	CalculatorDisplayImpl m_calc_disp;

	ExpressionCollectorTester() : ec(&m_calc_disp, &m_calc_hist, L'.', CalculatorMode::Null)
	{
		Resource::LoadRresource(&m_res_provider);
	}

	void Analy(const char* wstr)
	{
		ec.Clear();
		std::istringstream wstrs(wstr);
		std::string temp;
		while (wstrs >> temp)
		{
			if (temp[0] == L'(')
			{
				ec.AddOpenBrace();
			}
			else if (temp[0] == L')')
			{
				ec.AddCloseBrace();
			}
			else if (temp[0] == L'+')
			{
				ec.AddBinaryCommand(ADD);
			}
			else if (temp[0] == L'-')
			{
				ec.AddBinaryCommand(SUB);
			}
			else if (temp[0] == L'*')
			{
				ec.AddBinaryCommand(MUL);
			}
			else if (temp[0] == L'/')
			{
				ec.AddBinaryCommand(DIV);
			}
			else if (temp[0] == L'^')
			{
				ec.AddBinaryCommand(PWR);
			}
			else if (temp[0] == L'S')
			{
				ec.AddUnaryCommand(SIN_DEG);
			}
			else if (temp[0] == L'n')
			{
				ec.AddUnaryCommand(NEG);
			}
			else if (temp[0] >= L'0' && temp[0] <= L'9')
			{
				ec.AddOperandCommand(temp, mpl::mpf(temp, Radix(RadixType::Decimal)));
			}
		}
	}


#define ANALY(a) std::cout << a << std::endl;Analy(a); ec.CompleteEquation(); ec.SetExpressionDisplay();
	// 对于最基本的表达式收集进行测试
	void Test()
	{

		/*
		标准模式：会根据后加入的二元运算符的优先级来决定会不会插入括号。
		--没有括号操作
		*/
		ec.SetCalculatorMode(CalculatorMode::Standard);

		ANALY("1 + 246655656 + 3");
		ANALY("1 + 2 * 3");
		ANALY("1 + 2 * 3 ^ 448948946");
		ANALY("1 + 2 Sin");
		ANALY("1 + Sin");
		ANALY("1 + 2 + * + * + * + *");
		ANALY("1 * 2 + 3 * 4");
		ANALY("( ( 1 + 2 ) - ( 3 + 4 ) * 5");
		ANALY("( 1 + ( 1 + 2 ) - ( 3 + 4 ) * 5");


		std::cout << "-------------Scientific--------------" << std::endl;
		/*
		科学模式。
		*/
		ec.SetCalculatorMode(CalculatorMode::Scientific);

		ANALY("1 + 2 + 3");
		ANALY("1 + 2 * 3");
		ANALY("1 + 2 * 3 ^ 4");
		ANALY("Sin 1 + 2 * 3");
		ANALY("( ( 1 + 2 ) - ( 3 + 4 ) * 5");
		ANALY("1 + 345643513 Sin 1 + 2 - 3 Sin 44894651 ) ^ 5");
		ANALY("1 + 3 Sin 1 + 2 - 3 Sin 4 ) ^ 5 Sin 1");
		ANALY("( ( 1 + 2 ) - ( 3 + 4 ) negate * 5");
		ANALY("( ( 1 + 2 ) - ( 3 + 448946351 ) negate * 5 ) 5");
		ANALY("1 + 3 Sin 1561453 + 2 - 3 Sin 4 ) 5");
		ANALY("1 + 3 Sin 1 + 2 ) 5");
		ANALY("1 + 3 Sin ) + 5");
		ANALY("(");
		ANALY("( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( ( (");
		ANALY("1 + ( ( ( ( ( ( ( ( ( ( ( 1");
		ANALY("");
		ANALY("+ + + + + + + +");
		ANALY("1 + 2 +");
		ANALY("1 + 2 + ( ( 4 + 5 +");

	}
};
CM_END