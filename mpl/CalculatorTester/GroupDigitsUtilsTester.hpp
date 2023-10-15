#pragma once
#include "../CalculatorManagerLib/GroupDigitsUtils.h"
#include <iostream>
CM_BEGIN
class GroupDigitsUtilsTester
{
public:
	void Test()
	{
		std::vector<char const*> v =
		{
			"1",
			"1;2;3",
			"1;0",
			"1;0;0",
			"1;0;2",
			"1;0;0;2",
			"1;0;0;0;0",
			"1;;;2",
			"1;;2",
			"1;2",
			"1;2;",
			"1;2;0",
			"0;1;0;0;2;0",
			"0;0",
			"0;0;0",
			"0;0;3",
			"0",
			"0;",
			"00000015100000;000000156100000;00000156110000",
			"00000000;00000000000;000000000",
			"00000;000000",
			"000000",
			"0001313100;000000;00000",
			"00000;001231000;000000",
			"a;sdsd;00",
			"0000;0000a000;000",
			"16;123;499",
			"16;123;16",
			"000;0;0",
			"000;0",
			"3;00000"
		};
		for (auto c : v)
		{
			std::cout << c << std::endl;

			auto b = IsGroupStringInvalid(c);
			switch (b)
			{
			case GS_ERR_TOO_MANY_SC:
				std::cout << "太多的分号" << std::endl;
				break;
			case GS_ERR_ILLEGAL_CH_EXIST:
				std::cout << "存在存在非法字符" << std::endl;
				break;
			case GS_ERR_TOO_MANY_ZERO:
				std::cout << "太多连续的 0 了" << std::endl;
				break;
			case GS_ERR_NOT_ZERO_AFTER_ZERO:
				std::cout << "0 后面接的不是 0" << std::endl;
				break;
			case GS_ERR_NULL:
				std::cout << "没有错误" << std::endl;
				break;
			}
		}


		std::cout << GroupDigits("-1.23146884864e+6466464188481566486", GetGroupVectorFromString("3;0"), L',', L'.', true) << std::endl;
		std::cout << GroupDigits("-12314688486.46466464188481566486", GetGroupVectorFromString("3;0"), L',', L'.', true) << std::endl;
		std::cout << GroupDigits("-123146884864.6466464188e+481566486", GetGroupVectorFromString("3;0"), L',', L'.', true) << std::endl;
		std::cout << GroupDigits("-1231468848646466464188481566486", GetGroupVectorFromString("3;0"), L',', L'.', true) << std::endl;
		std::cout << GroupDigits("-1231468848646466464188481566486", GetGroupVectorFromString("5;3;2;0"), L',', L'.', true) << std::endl;
		std::cout << GroupDigits("-1231468848646466464188481566486", GetGroupVectorFromString("4;0"), L',', L'.', true) << std::endl;
	}
};
CM_END