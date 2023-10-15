#pragma once
#include "../CalculatorManagerLib/CalculatorInput.h"
#include "../CalculatorManagerLib/MpfMath.h"
#include "../CalculatorManagerLib/GroupDigitsUtils.h"
CM_BEGIN
class CalculatorInputTester
{
public:
	CalculatorInputTester() :input(dec, radix, numw, calm, 128) {}

	void Test()
	{
		PrintSetting();
		while (true)
		{
			PrintMenu();
		}
	}

	void PrintSetting()
	{
		std::cout << "当前设置为：" << std::endl;
		std::cout << "Decimal Pointer : \t" << dec << std::endl;
		std::cout << "Radix : \t\t" << radix << std::endl;
		std::cout << "Word Bit Width : \t" << numw << std::endl;
		std::cout << "Calculatoe Mode : \t";
		switch (calm)
		{
		case  CalculatorMode::Standard:
			std::cout << "Standard" << std::endl;
			break;
		case  CalculatorMode::Scientific:
			std::cout << "Scientific" << std::endl;
			break;
		case  CalculatorMode::Programmer:
			std::cout << "Programmer" << std::endl;
			break;
		case  CalculatorMode::Statistical:
			std::cout << "Statistica" << std::endl;
			break;
		default:
			std::cout << "Nul" << std::endl;
			break;
		}
	}

	void PrintFourRadixMpf()
	{
		mpl::mpf rat = input.ToMpf();
		mpl::mpf temp = rat;
		std::string ipu_str = input.ToString();
		std::string str{};

		TrueValueToComplement(rat, numw);
		std::cout << "-----------------------------------------------------" << std::endl;

		str = radix == 2 ? ipu_str : rat.to_str(2);
		std::cout << "BIN " << GroupDigits(str, { 4,0 }, ' ', dec, false);
		if (radix == 2) std::cout << "  *" << std::endl;
		else std::cout << std::endl;

		str = radix == 8 ? ipu_str : rat.to_str(8);
		std::cout << "OCT " << GroupDigits(str, { 3,0 }, ' ', dec, false);
		if (radix == 8) std::cout << "  *" << std::endl;
		else std::cout << std::endl;

		str = radix == 10 ? ipu_str : temp.to_str(10);
		std::cout << "DEC " << GroupDigits(str, { 3,0 }, ',', dec, temp.is_neg());
		if (radix == 10) std::cout << "  *" << std::endl;
		else std::cout << std::endl;

		str = radix == 16 ? ipu_str : rat.to_str(16);
		std::cout << "HEX " << GroupDigits(str, { 4,0 }, ' ', dec, false);
		if (radix == 16) std::cout << "  *" << std::endl;
		else std::cout << std::endl;

		std::cout << "-----------------------------------------------------" << std::endl;
	}

	void PrintMenu()
	{
		std::cout << "*****************************************************" << std::endl;
		std::cout << "1.追加位数" << std::endl;
		std::cout << "2.翻转符号" << std::endl;
		std::cout << "3.退格" << std::endl;
		if (calm == CalculatorMode::Programmer)
		{
			std::cout << "4.设置基数" << std::endl;
			std::cout << "5.设置字长" << std::endl;
		}
		else
		{
			std::cout << "4.加入指数" << std::endl;
			std::cout << "5.加入小数" << std::endl;
		}
		std::cout << "6.设置模式" << std::endl;
		std::cout << "7.输出当前模式" << std::endl;
		std::cout << "8.退出" << std::endl;
		std::cout << "*****************************************************" << std::endl;
		int choice = 0;
		do
		{
			std::cout << "输入指令：";
			std::cin >> choice;
		} while (choice > 8 || choice < 1);
		ProcessCommand(choice);
	}

	void ProcessCommand(int command)
	{
		switch (command)
		{
		case 1:
		{
			std::cout << "输入新位数：";
			char new_digit;
			std::cin >> new_digit;

			if (new_digit >= '0' && new_digit <= '9')
			{
				new_digit -= '0';
			}
			else if (new_digit >= 'a' && new_digit <= 'f')
			{
				new_digit -= 'a';
				new_digit += 10;
			}
			else if (new_digit >= 'A' && new_digit <= 'F')
			{
				new_digit -= 'A';
				new_digit += 10;
			}
			else
			{
				std::cout << "Error 未知的数字表达" << std::endl;
				break;
			}
			if (new_digit >= int(radix))
			{
				std::cout << "Error 输入的数字大于当前基数" << std::endl;
			}
			else if (!input.TryAddDigit(new_digit))
			{
				std::cout << "Error 插入位数失败" << std::endl;
			}
			break;
		}
		case 2:
		{
			if (radix != 10)
			{
				std::cout << "Error 非十进制不允许翻转符号" << std::endl;
			}
			else if (!input.TryToggleSign())
			{
				std::cout << "Error 翻转符号失败" << std::endl;
			}
			break;
		}
		case 3:
		{
			input.Backspace();
			break;
		}
		case 4:
		{
			if (calm == CalculatorMode::Programmer)
			{
				int new_radix;
				std::cout << "输入一个新基数：";
				std::cin >> new_radix;
				if (new_radix != 10 && new_radix != 8 && new_radix != 2 && new_radix != 16)
				{
					std::cout << "Error 无法处理此基数";
				}
				else
				{
					switch (new_radix)
					{
					case 2:radix = RadixType::Binary; break;
					case 8:radix = RadixType::Octal; break;
					case 10:radix = RadixType::Decimal; break;
					case 16:radix = RadixType::Hexadecimal; break;
					}
					input.SetRadix(radix);
				}
			}
			else if (!input.TryAddExponentBegin())
			{
				std::cout << "Error 开始加入指数失败";
			}
			break;
		}
		case 5:
		{
			if (calm == CalculatorMode::Programmer)
			{
				int new_nw;
				std::cout << "输入新字长：";
				std::cin >> new_nw;
				if (new_nw != 8 && new_nw != 16 && new_nw != 32 && new_nw != 64)
				{
					std::cout << "无法处理此字长";
				}
				else
				{
					switch (new_nw)
					{
					case 8:numw = NumberWidthType::Byte; break;
					case 16:numw = NumberWidthType::Word; break;
					case 32:numw = NumberWidthType::DWord; break;
					case 64:numw = NumberWidthType::QWord; break;
					}
					input.SetNumberWidth(numw);
				}
			}
			else if (!input.TryAddDecimalPoint())
			{
				std::cout << "Error 加入小数点失败";
			}
			break;
		}
		case 6:
		{
			std::cout << "*****************************************************" << std::endl;
			std::cout << "1.Standred" << std::endl;
			std::cout << "2.Scientific" << std::endl;
			std::cout << "3.Programmer" << std::endl;
			std::cout << "4.Statistica" << std::endl;
			std::cout << "*****************************************************" << std::endl;
			std::cout << "输入新的模式：";
			int i;
			std::cin >> i;
			calm = CalculatorMode(i - 1);
			input.SetCalculatorMode(calm);
			break;
		}
		case 7:
		{
			PrintSetting();
			break;
		}
		case 8: exit(0);
		}

		if (!input.IsEmpty())
		{
			if (calm == CalculatorMode::Programmer)
			{
				PrintFourRadixMpf();
			}
			else
			{
				std::cout << "-----------------------------------------------------" << std::endl;
				mpl::mpf rat = input.ToMpf();
				std::cout << "Input.to_str : " << GroupDigits(input.ToString(), { 3,0 }, ',', '.', rat.is_neg()) << std::endl;
				std::cout << "Ratio.to_str : " << GroupDigits(rat.to_str(10), { 3,0 }, ',', '.', rat.is_neg()) << std::endl;
				std::cout << "-----------------------------------------------------" << std::endl;
			}
		}
		else
		{
			std::cout << "Input Is Empty" << std::endl;
			PrintMenu();
		}
	}

	char dec = '.';
	Radix radix = RadixType::Decimal;
	NumberWidth numw = NumberWidthType::QWord;
	CalculatorMode calm = CalculatorMode::Programmer;
	CalculatorInput input;
};

CM_END