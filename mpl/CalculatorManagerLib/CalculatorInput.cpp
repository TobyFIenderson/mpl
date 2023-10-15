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
	// �Ƚ�����ת�����ַ���
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

		// ע�⣬С����ǰ�� 0 ��С���㱾����ռ�����볤�ȡ�
		if (has_decimal_point)
		{
			max_input_len++;
		}
		if (!num_str->value.empty() && num_str->value.front() == L'0')
		{
			max_input_len++;
		}
	}

	// �����ȵ��㡣
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
				// �����ǰ����ֵ�ĳ���ǡ��Ϊ��������� - 1 �Ļ�����Ҫע���ˡ�
				// ���磬���ֳ�Ϊ byte �£������ķ�ΧΪ -128 ~ 127 �����ʱ���е�����Ϊ 12
				// ��ô�������Ϊ������ô 0 ~ 8 ���ǿ���׷�ӵģ�����Ϊ�����Ļ���ֻ��׷�� 0 ~ 7 ��
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
				// �ڳ���Ա�������У����ж��������������䲹���ʾ����ʮ����ֱ���޷�������ƹҹ�
				// �ʣ�ʮ�������������Բ��� +- �ű�ʶ��������˽��ƣ�ʮ�����������Ķ�������������
				// ������Ժ�ֱ�Ӿ�ת���ɶ����Ƶ������ͽ�������������������
				// ���磬�� byte ģʽ�¶���������ɱ�ʾ�� 1111 1111 ���˽���Ϊ 377 ��ʮ����Ϊ -1 �� 
				// ����������еİ˽������ĵ�һλ���� 3 2 1 �Ļ�����ô����ֻ�����׷�� 1 λ�ˡ�
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
		/* �ǳ���Աģʽ���� radix == 2 ���� radix == 16 */
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
	// ��ʮ������������ת����λ����ʮ���Ƶ� SIGN ����������һ���󲹲���������ֹ���롣
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
			// -128 ���޷���� 128 �ġ�
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
	// ָ�����ֲ�������С����
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
	// Ϊ�˸�ʽͳһ��������õ�С���㣺
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

	// base �� ָ�� �ټ��� -1.e+100 �е� - . e +
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
		/* �������������и���Ч�ʵ�ʵ�� */
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
