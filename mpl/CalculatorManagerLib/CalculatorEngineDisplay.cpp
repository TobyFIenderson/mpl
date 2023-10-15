#include "CalculatorEngine.h"
#include "GroupDigitsUtils.h"
using namespace std;
CM_BEGIN
string CalculatorEngine::GetStringForDisplay(mpl::mpf const& num, Radix const& radix)
{
	// 十进制不存在补码的输出，且正数的补码是其本身。
	if (current_calculator_mode == CalculatorMode::Programmer && radix != 10 && num.is_neg())
	{
		mpl::mpf temp = num;
		TrueValueToComplement(temp, current_number_width);
		return temp.to_str(radix, current_number_format, current_precision);
	}
	else
	{
		return num.to_str(radix, current_number_format, current_precision);
	}
}

string CalculatorEngine::GroupDigitsPerRadix(string_view num_str, Radix const& radix)
{
	if (num_str.empty())
	{
		return string();
	}

	switch (radix)
	{
	case 10: return GroupDigits(num_str, group_vector, group_separator, decimal_separator, num_str.front() == L'-');
	case 8: return GroupDigits(num_str, { 3,0 }, L' ', decimal_separator, false);
	case 2:case 16: return GroupDigits(num_str, { 4,0 }, L' ', decimal_separator, false);
	default: return string(num_str);
	}
}

char CalculatorEngine::GetDecimalSeparator()
{
	return decimal_separator;
}

struct LastUpdateNumberStringInformation
{
	mpl::mpf value;
	int32_t precision;
	Radix radix;
	NumberFormat number_format;
	NumberWidth number_width;
	bool is_programmer_mode;
	bool is_recording;
	bool is_use_separator;
};
static LastUpdateNumberStringInformation LAST_UPDATE_INFO{ 0, -1, RadixType::Null, NumberFormat::Null, NumberWidthType::Null, false, false, false };

void CalculatorEngine::UpdateNumberString()
{
	// 只有在以下情况字符串才会更新：
	// -- 处于输入模式
	// -- 第一次调用 UpdateNumberString() 函数
	// -- 在 CalculatorEngine 中一些会影响输出字符串样式的设置被改变了
	if (is_recording || current_value != LAST_UPDATE_INFO.value || current_precision != LAST_UPDATE_INFO.precision || current_radix != LAST_UPDATE_INFO.radix ||
		current_number_format != LAST_UPDATE_INFO.number_format || !LAST_UPDATE_INFO.is_use_separator || LAST_UPDATE_INFO.number_width != current_number_width ||
		LAST_UPDATE_INFO.is_programmer_mode != (current_calculator_mode == CalculatorMode::Programmer))
	{
		LAST_UPDATE_INFO.precision = current_precision;
		LAST_UPDATE_INFO.radix = current_radix;
		LAST_UPDATE_INFO.number_format = current_number_format;
		LAST_UPDATE_INFO.number_width = current_number_width;
		LAST_UPDATE_INFO.is_programmer_mode = (current_calculator_mode == CalculatorMode::Programmer);
		LAST_UPDATE_INFO.is_recording = is_recording;
		LAST_UPDATE_INFO.is_use_separator = true;

		number_string = is_recording ? input.ToString() : GetStringForDisplay(current_value, current_radix);

		LAST_UPDATE_INFO.value = current_value;

		/* IsNumberInvalid 实在太慢对于 1000 位的输出居然要运行数秒 */
		if (current_radix == RadixType::Decimal && !ErrorCodeIsNull(IsNumberInvalid(number_string, MAX_EXP_LEN, current_precision, current_radix, decimal_separator)))
		{
			ProcessError(CE_ERR_INPUT_OVERFLOW);
		}
	}
}

void CalculatorEngine::SetPrimaryDisplay(string const& dis_str, bool is_err)
{
	if (calculator_display != nullptr)
	{
		calculator_display->SetPrimaryDisplay(dis_str, is_err);
		calculator_display->SetErrorState(is_err);
	}
}

void CalculatorEngine::SetExpressionDisplay()
{
	if (calculator_display != nullptr)
	{
		calculator_display->SetExpressionDisplay(expression_collector.GetCommands(), expression_collector.GetTokens());
	}
}

void CalculatorEngine::SetPrimaryDisplay()
{
	if (calculator_display != nullptr)
	{
		calculator_display->SetPrimaryDisplay(number_string, IsInErrorState());
	}
}

bool CalculatorEngine::IsInErrorState()
{
	return is_in_error_state;
}
CM_END