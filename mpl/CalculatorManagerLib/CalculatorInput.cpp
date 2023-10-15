#include "CalculatorInput.h"
using namespace std;

CM_BEGIN

CalculatorInput::CalculatorInput(char dec_pt, Radix const& o_radix, NumberWidth const& number_width, CalculatorMode calc_mode, int32_t o_prec) :
	decimal_point(dec_pt),
	radix(o_radix),
	word_bit_width(number_width),
	precision(o_prec),
	has_decimal_point(false),
	has_exponent(false),
	decimal_point_index(0),
	exponent(),
	base(),
	calculator_mode(calc_mode)
{}

void CalculatorInput::Clear()
{
	has_exponent = false;
	has_decimal_point = false;
	decimal_point_index = 0;
	base.Clear();
	exponent.Clear();
}

void CalculatorInput::SetRadix(Radix const& new_radix)
{
	radix = new_radix;
	Clear();
}

void CalculatorInput::SetNumberWidth(NumberWidth const& new_num_width)
{
	word_bit_width = new_num_width;
	Clear();
}

void CalculatorInput::SetCalculatorMode(CalculatorMode calc_mode)
{
	calculator_mode = calc_mode;
	Clear();
}

void CalculatorInput::SetDecimalSymbol(char dec_pt)
{
	decimal_point = dec_pt;
	if (has_decimal_point)
	{
		base.value[decimal_point_index] = decimal_point;
	}
}

void CalculatorInput::SetPrecision(int32_t new_prec)
{
	precision = new_prec;
	Clear();
}

bool CalculatorInput::TryAddDigit(unsigned int value)
{
	// 先将数字转换成字符。
	char ch_digit = char((value < 10) ? ('0' + value) : ('A' + value - 10));

	InputString* num_str = nullptr;
	size_t max_input_len = 0;
	if (has_exponent)
	{
		num_str = &exponent;
		max_input_len = MAX_EXP_LEN;
	}
	else
	{
		num_str = &base;
		if (calculator_mode != CalculatorMode::Programmer)
		{
			max_input_len = GetMaxInputLength(calculator_mode);
		}
		else
		{
			max_input_len = word_bit_width.GetMaxNumberDigit(radix);
		}

		// 注意，小数点前的 0 和小数点本身不会占用输入长度。
		if (has_decimal_point)
		{
			max_input_len++;
		}
		if (!num_str->value.empty() && num_str->value.front() == L'0')
		{
			max_input_len++;
		}
	}

	// 忽略先导零。
	if (num_str->value.empty() && !value)
	{
		return true;
	}

	bool allow_extra_digit = false;
	if (calculator_mode == CalculatorMode::Programmer && (radix == 10 || radix == 8))
	{
		string const& max_num_str = word_bit_width.GetMaxNumberString(radix);

		if (num_str->value.size() < max_input_len - 1)
		{
			allow_extra_digit = true;
		}
		else if (num_str->value.size() == max_input_len - 1)
		{
			if (radix == 10)
			{
				// 如果当前已有值的长度恰好为最大允许长度 - 1 的话，就要注意了。
				// 例如，在字长为 byte 下，整数的范围为 -128 ~ 127 如果此时已有的数据为 12
				// 那么如果该数为负数那么 0 ~ 8 都是可以追加的，但是为正数的话就只能追加 0 ~ 7 。
				size_t first_greater = 0;
				for (first_greater = 0; first_greater < max_input_len - 1; first_greater++)
				{
					if (num_str->value[first_greater] > max_num_str[first_greater])
					{
						break;
					}
				}
				size_t first_less = 0;
				for (first_less = 0; first_less < max_input_len - 1; first_less++)
				{
					if (num_str->value[first_less] < max_num_str[first_less])
					{
						break;
					}
				}

				// ---(==)--fl--(<=)--fg---
				// 32767
				// 3189
				if (first_less < first_greater)
				{
					allow_extra_digit = true;
				}
				// ------------------------(fg)(fl)
				// 32767
				// 3276
				else if (first_less == first_greater)
				{
					char max_last_digit = num_str->is_negative ? max_num_str.back() + 1 : max_num_str.back();
					if (ch_digit <= max_last_digit)
					{
						allow_extra_digit = true;
					}
				}
			}
			else if (radix == 8)
			{
				// 在程序员计算器中，所有二进制数都采用其补码表示，但十进制直接无法与二进制挂钩
				// 故，十进制数的正负仍采用 +- 号标识，但是像八进制，十六进制这样的二进制衍生进制
				// 本身可以很直接就转换成二进制的数，就将他看作二进制数处理。
				// 例如，在 byte 模式下二进制数最长可表示成 1111 1111 ，八进制为 377 ，十进制为 -1 。 
				// 但是如果已有的八进制数的第一位不是 3 2 1 的话，那么他就只能最多追加 1 位了。
				switch (word_bit_width % 3)
				{
				case 1: // Word QWord
					allow_extra_digit = (num_str->value.front() == L'1');
					break;
				case 2: // Byte DWord
					allow_extra_digit = (num_str->value.front() <= L'3');
					break;
				}
			}
		}
	}
	else if (num_str->value.size() < max_input_len)
	{
		/* 非程序员模式或者 radix == 2 或者 radix == 16 */
		allow_extra_digit = true;
	}

	if (allow_extra_digit)
	{
		num_str->value += ch_digit;
	}

	return allow_extra_digit;
}

bool CalculatorInput::TryToggleSign()
{
	// 非十进制数不允许翻转符号位，非十进制的 SIGN 操作看成是一次求补操作，会终止输入。
	if (radix != 10)
	{
		return false;
	}

	if (base.value.empty())
	{
		base.is_negative = false;
		exponent.is_negative = false;
	}
	else if (has_exponent)
	{
		exponent.is_negative = !exponent.is_negative;
	}
	else
	{
		if (calculator_mode == CalculatorMode::Programmer && base.is_negative)
		{
			// -128 是无法变成 128 的。
			if (base.value == word_bit_width.GetMinNumberString(radix))
			{
				return false;
			}
		}
		base.is_negative = !base.is_negative;
	}

	return true;
}

bool CalculatorInput::TryAddDecimalPoint()
{
	// 指数部分不允许是小数。
	if (has_decimal_point || has_exponent)
	{
		return false;
	}

	if (base.value.empty())
	{
		base.value += L'0';
	}

	decimal_point_index = base.value.size();
	base.value += decimal_point;
	has_decimal_point = true;

	return true;
}

bool CalculatorInput::TryAddExponentBegin()
{
	// 为了格式统一，加入后置的小数点：
	// 1 exp 100 --> 1.e+100
	TryAddDecimalPoint();

	if (has_exponent)
	{
		return false;
	}
	has_exponent = true;

	return true;
}

bool CalculatorInput::IsEmpty()
{
	return base.value.empty() && !has_exponent && exponent.value.empty() && !has_decimal_point;
}

bool CalculatorInput::HasDecimalPoint()
{
	return has_decimal_point;
}

bool CalculatorInput::IsBinaryBaseNegative()
{
	if (base.value.empty())
	{
		return false;
	}

	if (radix == 10)
	{
		return base.is_negative;
	}
	else if (base.value.size() == word_bit_width.GetMaxNumberDigit(radix))
	{
		switch (radix)
		{
		case 2: return base.value.front() == L'1';
		case 8:
			switch (word_bit_width % 3)
			{
			case 1: return base.value.front() == L'1';
			case 2: return base.value.front() == L'2' || base.value.front() == L'3';
			}
		case 16: return base.value.front() >= L'8';
		}
	}
	return false;
}

string CalculatorInput::ToString()
{
	if (base.value.size() > MAX_STR_LEN || (has_exponent && exponent.value.size() > MAX_STR_LEN))
	{
		return string();
	}

	string ret{};

	if (base.is_negative)
	{
		ret += L'-';
	}

	if (base.value.empty())
	{
		ret += L'0';
	}
	else
	{
		ret += base.value;
	}

	if (has_exponent)
	{
		if (!has_decimal_point)
		{
			ret += decimal_point;
		}

		ret += (radix == 10 ? L'e' : L'^');
		ret += (exponent.is_negative ? L'-' : L'+');

		if (exponent.value.empty())
		{
			ret += L'0';
		}
		else
		{
			ret += exponent.value;
		}
	}

	// base 和 指数 再加上 -1.e+100 中的 - . e +
	if (ret.size() > MAX_STR_LEN * 2 + 4)
	{
		return string();
	}

	return ret;
}

mpl::mpf CalculatorInput::ToMpf()
{
	mpl::mpf rat = 0;
	if (base.value.empty())
	{
		if (exponent.value.empty())
		{
			rat = 0;
		}
		else
		{
			rat = 1;
		}
	}
	else
	{
		rat = mpl::mpf(base.value, radix);
	}

	mpl::mpf _exp = 0;
	if (!exponent.value.empty())
	{
		_exp = mpl::mpf(exponent.value, radix);
	}
	mpl::mpf::pow(int(radix), _exp, _exp);
	mpl::mpf::mul(rat, _exp, rat);
	if (exponent.is_negative)
	{
		mpl::mpf::inv(rat, rat);
	}
	if (base.is_negative)
	{
		rat.mant.sign *= -1;
	}

	if (radix != 10 && IsBinaryBaseNegative())
	{
		mpl::mpi t = rat.to_mpi();
		mpl::mpi::xor_tc(t, word_bit_width.GetChopNumber(), t);
		t.incr();
		/* 这个与操作可以有更高效率的实现 */
		mpl::mpi::and_tc(t, word_bit_width.GetChopNumber(), t);
		t.sign *= -1;
		rat = move(t);
	}
	return rat;
}

void CalculatorInput::Backspace()
{
	if (has_exponent)
	{
		if (!exponent.value.empty())
		{
			exponent.value.pop_back();

			if (exponent.value.empty())
			{
				exponent.Clear();
			}
		}
		else
		{
			has_exponent = false;
		}
	}
	else
	{
		if (!base.value.empty())
		{
			base.value.pop_back();
			if (base.value == "0")
			{
				base.value.pop_back();
			}
		}

		if (base.value.size() <= decimal_point_index)
		{
			has_decimal_point = false;
			decimal_point_index = 0;
		}

		if (base.value.empty())
		{
			base.Clear();
		}
	}
}


CM_END
