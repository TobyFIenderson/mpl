#include "CalculatorEngine.h"
using namespace mpl;
using namespace std;
CM_BEGIN
void CalculatorEngine::SetRadixAndNumberWidth(Radix const& radix, NumberWidth const& number_width)
{
	if (radix != RadixType::Null)
	{
		current_radix = radix;
		input.SetRadix(radix);
	}

	if (number_width != NumberWidthType::Null)
	{
		input.SetNumberWidth(number_width);
		// 当要将字长修改为更短时 0001 0101 1000 1010(WORD) ---> 1000 1010(BYTE) 需要进行截断。
		if (current_calculator_mode == CalculatorMode::Programmer && current_number_width > number_width)
		{
			current_value = And(current_value, number_width.GetChopNumber(), number_width);
		}
		current_number_width = number_width;
	}

	UpdateMaxIntegerDigit();
	UpdateNumberString();
}

void CalculatorEngine::SetPrecision(int new_precision)
{
	if (new_precision <= 0)
	{
		return;
	}
	current_precision = new_precision;
	input.SetPrecision(new_precision);
	int ratio = max(int(radix_ratio), 1);
	MPF_ORIGINAL_PRECISION = current_precision / ratio;
}

string CalculatorEngine::GetCurrentValueForRadix(Radix const& radix, bool group_digit_per_radix)
{
	mpf rat = is_recording ? input.ToMpf() : current_value;
	string num_str = GetStringForDisplay(rat, radix);
	return group_digit_per_radix ? GroupDigitsPerRadix(num_str, radix) : num_str;
}

Radix const& CalculatorEngine::GetCurrentRadix()
{
	return current_radix;
}

mpf const& CalculatorEngine::GetMemorizedNumber()
{
	return memorized_number;
}
void CalculatorEngine::UpdateMaxIntegerDigit()
{
	if (current_radix == 10)
	{
		if (current_calculator_mode == CalculatorMode::Programmer)
		{
			current_max_int_digit = static_cast<int>(current_number_width.GetMaxNumberString(current_radix).length() - 1);
		}
		else
		{
			current_max_int_digit = current_precision;
		}
	}
	else
	{
		current_max_int_digit = int(current_number_width / log2(int(current_radix)));
	}
}

void CalculatorEngine::SetMemorizedNumber(mpf const& mem_rat)
{
	memorized_number = mem_rat;
}

CalculatorMode CalculatorEngine::GetCalculatorMode() const
{
	return current_calculator_mode;
}
CM_END