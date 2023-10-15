#include "CalculatorEngine.h"
#include <stack>
using namespace std;
using namespace mpl;
CM_BEGIN

void CalculatorEngine::ProcessCommand(Command command)
{
	CommandType command_t = GetCommandType(command);

	if (command_t != CommandType::GUISetting)
	{
		last_command = hold_command;
		hold_command = command;
	}

	if (is_in_error_state)
	{
		if (command == CLEAR || command == CENTR)
		{
			command = CLEAR;
		}
		else
		{
			ProcessErrorCommand(command);
			return;
		}
	}

	if (is_recording)
	{
		bool allow_input =
			command_t == CommandType::NumberDigit ||
			command == BACK ||
			command == EXP ||
			command == PNT ||
			command == CENTR ||
			(command == SIGN && current_radix == RadixType::Decimal);/*��10���Ƶķ��ſ���һ���󲹲���������ֹ����*/
		if (!allow_input)
		{
			is_recording = false;
			last_value = current_value;
			current_value = input.ToMpf();
			UpdateNumberString();
			expression_collector.AddOperandCommand(number_string, current_value);
		}
	}
	else if (command_t == CommandType::NumberDigit || command == PNT)
	{
		is_recording = true;
		input.Clear();
	}

	if (command_t == CommandType::NumberDigit)
	{
		unsigned int val = static_cast<unsigned int>(command - NUM_0);

		if (val >= unsigned(current_radix))
		{
			ProcessErrorCommand(command);
			return;
		}

		if (!input.TryAddDigit(val))
		{
			// ׷��λ��ʧ�����������
			// 1- �ﵽ��������������λ����
			// 2- �ڳ���Աģʽ�£�����������׷��λ����
			ProcessErrorCommand(command);
			return;
		}
		UpdateNumberString();
		return;
	}

	if (command_t == CommandType::Binary)
	{
		bool change_last_bin_op = expression_collector.AddBinaryCommand(command);

		if (current_calculator_mode == CalculatorMode::Standard)
		{
			// 1+2+* ---> (1+2)*
			if (!change_last_bin_op && last_bin_command != CMD_NULL)
			{
				current_value = DoBinaryOperation(last_value, current_value, last_bin_command);
				if (!is_in_error_state)
				{
					UpdateNumberString();
				}
				last_value = current_value;
			}
		}
		last_bin_command = command;
		return;
	}

	if (command_t == CommandType::Unary)
	{
		if (current_calculator_mode == CalculatorMode::Standard)
		{
			// NUM_1 ADD NUM_2 ADD SQRT --->
			// 1 + 2 + sqrt(3)
			// ����
			// NUM_1 ADD NUM_2 ADD EQU --->
			// 1 + 2 = 3
			if (GetCommandType(last_command) == CommandType::Binary)
			{
				current_value = last_value;
			}

			// PRECENT ��һ�������һԪ�������
			// �������ڱ��ʽ����ʾ % ���ţ�����ֱ�ӽ������ʾ����
			// ���� NUM_1NUM_0NUM_0 + NUM_2 PERCENT ---> 100 + 2. (100 * 2%)
			if (command != PERCENT)
			{
				// NUM_1 ADD NUM_2 ADD SQRT --->
				// 1 + 2 + sqrt(3)
				if (!expression_collector.LastExpressionIsOperand())
				{
					expression_collector.AddOperandCommand(number_string, current_value);
				}
				expression_collector.AddUnaryCommand(command);
			}

			// �����Ǻ���
			if (INRANGE(command, SIN_DEG, ARCOTH) && current_value >= max_trigonometric_value)
			{
				current_value = 0;
				ProcessError(MPL_ERR_INVALID_VAL);
				return;
			}

			current_value = DoUnaryOperation(current_value, command);

			if (is_in_error_state)
			{
				return;
			}

			UpdateNumberString();

			if (command == PERCENT)
			{
				expression_collector.AddOperandCommand(number_string, current_value);
			}
		}
		else
		{
			expression_collector.AddUnaryCommand(command);
		}

		return;
	}

	if (command_t == CommandType::BitPosition)
	{
		try
		{
			current_value = ToggleBit(current_value, command - BIT_BEGIN);
		}
		catch (...)
		{
			return;
		}

		UpdateNumberString();
		return;
	}

	switch (command)
	{
	case CLEAR: /* ������У���Ӧ��ť [C] */
		current_value = 0;
		last_value = 0;
		expression_collector.Clear();
		is_in_error_state = false;
		last_command = CMD_NULL;
		hold_command = CMD_NULL;
		UpdateNumberString();
		break;
	case CENTR: /* ������룬��Ӧ��ť [CE] */
		input.Clear();
		is_recording = true;
		UpdateNumberString();
		break;
	case BACK: /* �˸񣬶�Ӧ��ť [<x] */
		if (is_recording)
		{
			input.Backspace();
			UpdateNumberString();
		}
		else
		{
			ProcessErrorCommand(command);
		}
		break;
	case EQU: /* ���ڣ���Ӧ��ť [=] */
		if (current_calculator_mode == CalculatorMode::Standard)
		{
			/* Ҫ���ʣ��ļ���*/
			if (expression_collector.LastExpressionIsOperand() && last_bin_command != CMD_NULL)
			{
				current_value = DoBinaryOperation(last_value, current_value, last_bin_command);
			}
		}
		expression_collector.CompleteEquation();
		if (current_calculator_mode != CalculatorMode::Standard)
		{
			current_value = CalculateExpression(expression_collector.GetCommands());
		}


		if (!is_in_error_state)
		{
			UpdateNumberString();
			expression_collector.CompleteHistoryLine(GroupDigitsPerRadix(number_string, current_radix));
		}
		last_value = current_value = 0;
		break;
	case OPENP: /* �����ţ���Ӧ��ť [(] */
	case CLOSEP: /* �����ţ���Ӧ��ť [)] */
		// ��׼ģʽ����û�����ȼ���
		if (current_calculator_mode == CalculatorMode::Standard)
		{
			ProcessErrorCommand(command);
		}
		if (command == OPENP)
		{
			expression_collector.AddOpenBrace();
		}
		else
		{
			expression_collector.AddCloseBrace();
		}
		UpdateNumberString();
		break;
	case BIN:
	case OCT:
	case DEC:
	case HEX:
		SetRadixAndNumberWidth(RadixType(command - BIN), NumberWidthType::Null);
		expression_collector.UpdateCollectedExpression(current_radix, current_precision);
		break;
	case BYTE:
	case WORD:
	case DWORD:
	case QWORD:
		// �޸��ֿ���ж����롣
		if (is_recording)
		{
			current_value = input.ToMpf();
			is_recording = false;
		}
		SetRadixAndNumberWidth(RadixType::Null, NumberWidthType(command - BYTE));
		break;
	case SIGN: /* ��ת���ţ���Ӧ��ť [+/-] */
		if (is_recording)
		{
			// ����Ա�������µķ�10���Ƶ� SIGN ָ��ᱻ�����󲹲���������ֹ���롣
			if (current_calculator_mode == CalculatorMode::Programmer && current_radix != 10)
			{
				current_value = input.ToMpf();
				current_value = DoUnaryOperation(current_value, NEG);
				if (!is_in_error_state)
				{
					UpdateNumberString();
				}
				expression_collector.AddOperandCommand(number_string, current_value);
				expression_collector.AddUnaryCommand(NEG);
			}
			else
			{
				if (input.TryToggleSign())
				{
					UpdateNumberString();
				}
				else
				{
					ProcessErrorCommand(command);
				}
			}
		}
		else
		{
			if (current_calculator_mode == CalculatorMode::Standard)
			{
				if (GetCommandType(last_command) == CommandType::Binary)
				{
					current_value = last_value;
				}

				current_value = DoUnaryOperation(current_value, NEG);
				UpdateNumberString();

				if (!expression_collector.LastExpressionIsOperand())
				{
					expression_collector.AddOperandCommand(number_string, current_value);
				}

				expression_collector.AddUnaryCommand(NEG);
			}
		}
		break;
	case MRECALL: /* ����ǰ�����������ʾ��������Ӧ��ť [MR] */
		current_value = memorized_number;
		UpdateNumberString();
		expression_collector.AddOperandCommand(number_string, current_value);
		break;
	case MPLUS: /* ����ǰֵ�ۼӵ������ֵ��ȥ����Ӧ��ť [M+] */
		mpf::add(memorized_number, current_value, memorized_number);
		break;
	case MMINUS: /* ����ǰֵ�Ը�����ʽ�ۼӵ������ֵ��ȥ����Ӧ��ť [M-] */
		mpf::sub(memorized_number, current_value, memorized_number);
		break;
	case MSTORE: /* ���䵱ǰ���֣���Ӧ��ť [MS] */
		memorized_number = current_value;
		break;
	case MCLEAR: /* �����������ݣ���Ӧ��ť [MC] */
		memorized_number = 0;
		break;
	case PI: /* ���뵱ǰ�����µ�Բ���ʣ���Ӧ��ť [��] */
	case EULER: /* ���뵱ǰ�����µ���Ȼ���� e ����Ӧ��ť [e] */
		if (current_calculator_mode == CalculatorMode::Programmer)
		{
			ProcessErrorCommand(command);
		}
		else
		{
			// PI, Euler ָ���൱��������һ������
			current_value = (command == PI ? mpf::const_pi() : mpf::const_e());
			UpdateNumberString();
			expression_collector.AddOperandCommand(number_string, current_value);
		}
		break;
	case RAND: /* ��ȡһ�����������Ӧ��ť [RAND] */
		if (current_calculator_mode == CalculatorMode::Programmer)
		{
			ProcessErrorCommand(command);
		}
		else
		{
			current_value = GenerateRandomNumber();
			UpdateNumberString();
			expression_collector.AddOperandCommand(number_string, current_value);
		}
		break;
	case FLOFMT:
		current_number_format = NumberFormatType::Float;
		UpdateNumberString();
		break;
	case SCIFMT:
		current_number_format = NumberFormatType::Scientific;
		UpdateNumberString();
		break;
	case ENGFMT:
		current_number_format = NumberFormatType::Engineering;
		UpdateNumberString();
		break;
	case EXP: /* ����ָ������Ӧ��ť [exp] */
		if (is_recording && current_calculator_mode != CalculatorMode::Programmer && input.TryAddExponentBegin())
		{
			UpdateNumberString();
		}
		else
		{
			ProcessErrorCommand(command);
		}
		break;
	case PNT: /* ����С���㣬��Ӧ��ť [.] */
		if (is_recording && current_calculator_mode != CalculatorMode::Programmer && input.TryAddDecimalPoint())
		{
			UpdateNumberString();
		}
		else
		{
			ProcessErrorCommand(command);
		}
		break;
	}
}

void CalculatorEngine::ProcessErrorCommand(Command command)
{
	if (GetCommandType(command) != CommandType::GUISetting)
	{
		hold_command = last_command;
	}
}

void CalculatorEngine::ProcessError(uint32_t err_code)
{
	string error_string = Resource::GetStringFromErrorCode(err_code);

	SetPrimaryDisplay(error_string, true/* is_error */);

	is_in_error_state = true;

	expression_collector.Clear();
}

double CalculatorEngine::GenerateRandomNumber()
{
	if (random_engine == nullptr)
	{
		random_device rd;
		random_engine = std::make_unique<std::mt19937>(rd());
		distr = std::make_unique<std::uniform_real_distribution<>>(0, 1);
	}
	return (*distr.get())(*random_engine.get());
}

mpf CalculatorEngine::CalculateExpression(ExpressionCommands const& cmds)
{
	stack<pair<CommandType, Command>> optr_s;
	stack<pair<const mpf*, bool>> opnd_s;

	for (shared_ptr<IExpressionCommand> const& cur_cmd : *cmds)
	{
		switch (cur_cmd->GetCommandType())
		{
		case CommandType::Unary:
		{
			shared_ptr<UnaryCommand> temp = static_pointer_cast<UnaryCommand>(cur_cmd);
			optr_s.emplace(CommandType::Unary, temp->GetCommand());
		}
		break;
		case CommandType::Binary:
		{
			shared_ptr<BinaryCommand> temp = static_pointer_cast<BinaryCommand>(cur_cmd);
			Command bin_optr = temp->GetCommand();
			while (!optr_s.empty() && optr_s.top().second != OPENP &&
				CompareBinaryCommandPrecedence(optr_s.top().second, bin_optr) >= 0)
			{
				pair<const mpf*, bool> opnd_y = opnd_s.top();
				opnd_s.pop();
				pair<const mpf*, bool> opnd_x = opnd_s.top();
				opnd_s.pop();

				mpf* rat = new mpf(DoBinaryOperation(*(opnd_x.first), *(opnd_y.first), optr_s.top().second));
				optr_s.pop();
				opnd_s.emplace(rat, true);

				if (opnd_x.second)
				{
					delete opnd_x.first;
				}
				if (opnd_y.second)
				{
					delete opnd_y.first;
				}

			}
			optr_s.emplace(CommandType::Binary, bin_optr);
		}
		break;
		case CommandType::Operand:
		{
			shared_ptr<OperandCommand> temp = static_pointer_cast<OperandCommand>(cur_cmd);
			opnd_s.emplace(&(temp->GetMpf()), false);
		}
		break;
		case CommandType::Parentheses:
		{
			shared_ptr<ParenthesisCommand> temp = static_pointer_cast<ParenthesisCommand>(cur_cmd);
			// ����ǿ����ţ�ֱ��ѹջ��
			if (temp->IsOpenParenthesis())
			{
				optr_s.emplace(CommandType::Parentheses, OPENP);
			}
			else
			{
				// ����Ǳ����š�
				while (!optr_s.empty())
				{
					// �ȵ���һ���������
					pair<CommandType, Command> optr = optr_s.top();
					optr_s.pop();
					// ����������ǿ����š�˵��Ϊ (opnd) ����ʽ��
					if (optr.second == OPENP)
					{
						// �����п���Ϊ UNARY(opnd) ��ʽ������ Sin(1)��
						if (!optr_s.empty() && optr_s.top().first == CommandType::Unary)
						{
							pair<const mpf*, bool> opnd = opnd_s.top();
							opnd_s.pop();
							mpf* rat = new mpf(DoUnaryOperation(*(opnd.first), optr_s.top().second));
							optr_s.pop();
							opnd_s.emplace(rat, true);

							if (opnd.second)
							{
								delete opnd.first;
							}
						}

						// �ҵ��������������Ŀ����ţ�����ѭ����
						break;
					}
					else if (optr.first == CommandType::Binary) // �����������һ����Ԫ���������ô���������������������㡣
					{
						pair<const mpf*, bool> opnd_y = opnd_s.top();
						opnd_s.pop();
						pair<const mpf*, bool> opnd_x = opnd_s.top();
						opnd_s.pop();

						mpf* rat = new mpf(DoBinaryOperation(*(opnd_x.first), *(opnd_y.first), optr.second));
						opnd_s.emplace(rat, true);
						if (opnd_x.second)
						{
							delete opnd_x.first;
						}
						if (opnd_y.second)
						{
							delete opnd_y.first;
						}
					}
				}
			}
		}
		break;
		}
	}

	// �������ʣ������������ô�ض��Ƕ�Ԫ�������
	while (!optr_s.empty() && opnd_s.size() > 1)
	{
		pair<const mpf*, bool> opnd_y = opnd_s.top();
		opnd_s.pop();
		pair<const mpf*, bool> opnd_x = opnd_s.top();
		opnd_s.pop();

		mpf* rat = new mpf(DoBinaryOperation(*(opnd_x.first), *(opnd_y.first), optr_s.top().second));
		optr_s.pop();
		opnd_s.emplace(rat, true);

		if (opnd_x.second)
		{
			delete opnd_x.first;
		}
		if (opnd_y.second)
		{
			delete opnd_y.first;
		}
	}

	mpf result = opnd_s.size() == 1 ? *(opnd_s.top().first) : mpf();

	while (!opnd_s.empty())
	{
		if (opnd_s.top().second)
		{
			delete opnd_s.top().first;
		}
		opnd_s.pop();
	}

	return result;
}

CM_END