#include "ExpressionCommand.h"
#include "NumberFormat.h"
using namespace std;
CM_BEGIN

constexpr char NEGATE_SIGN = L'-';
constexpr char EXPONENT_SIGN = L'e';
constexpr char PLUS_SIGN = L'+';
constexpr char CHAR_ZERO = L'0';

ParenthesisCommand::ParenthesisCommand(Command cmd) :cmd(cmd) {}

void ParenthesisCommand::SetCommand(Command cmd_)
{
	this->cmd = cmd_;
}

Command ParenthesisCommand::GetCommand() const
{
	return cmd;
}

CommandType ParenthesisCommand::GetCommandType() const
{
	return CommandType::Parentheses;
}

void ParenthesisCommand::Accept(_In_ ISerializeCommandVisitor& cmd_visitor)
{
	cmd_visitor.Visit(*this);
}

bool ParenthesisCommand::IsOpenParenthesis() const
{
	return cmd == OPENP;
}

bool ParenthesisCommand::IsCloseParenthesis() const
{
	return cmd == CLOSEP;
}

UnaryCommand::UnaryCommand(Command o_cmd) : cmd{ o_cmd } {}

CommandType UnaryCommand::GetCommandType() const
{
	return CommandType::Unary;
}

void UnaryCommand::SetCommand(Command cmd_)
{
	this->cmd = cmd_;
}

Command UnaryCommand::GetCommand() const
{
	return cmd;
}

void UnaryCommand::Accept(_In_ ISerializeCommandVisitor& cmd_visitor)
{
	cmd_visitor.Visit(*this);
}

BinaryCommand::BinaryCommand(Command command) :cmd(command) {}

void BinaryCommand::SetCommand(Command command)
{
	cmd = command;
}

Command BinaryCommand::GetCommand() const
{
	return cmd;
}

CommandType BinaryCommand::GetCommandType() const
{
	return CommandType::Binary;
}

void BinaryCommand::Accept(_In_ ISerializeCommandVisitor& commandVisitor)
{
	commandVisitor.Visit(*this);
}

OperandCommand::OperandCommand(shared_ptr<vector<Command>> const& o_cmds, bool is_neg, bool has_dec_pnt, bool sci_fmt) :
	cmds(o_cmds),
	is_negative(is_neg),
	is_scientific_format(sci_fmt),
	is_have_decimal_point(has_dec_pnt),
	is_initialized(false),
	value(0)
{}

void OperandCommand::Initialize(mpl::mpf const& rat)
{
	value = rat;
	is_initialized = true;
}

shared_ptr<vector<Command>> const& OperandCommand::GetCommands() const
{
	return cmds;
}

void OperandCommand::SetCommands(shared_ptr<vector<Command>> const& cmds_)
{
	this->cmds = cmds_;
}

void OperandCommand::AppendCommand(Command command)
{
	if (is_scientific_format)
	{
		ClearAllAndAppendCommand(command);
	}
	else
	{
		cmds->push_back(command);
	}

	if (command == PNT)
	{
		is_have_decimal_point = true;
	}
}

void OperandCommand::ToggleSign()
{
	for (auto const& command : *cmds)
	{
		// 零永远是正数。
		if (command != NUM_0)
		{
			is_negative = !is_negative;
			break;
		}
	}
}

void OperandCommand::RemoveBack()
{
	if (is_scientific_format)
	{
		ClearAllAndAppendCommand(NUM_0);
	}
	else
	{
		if (cmds->size() == 1)
		{
			ClearAllAndAppendCommand(NUM_0);
		}
		else
		{
			if (cmds->back() == PNT)
			{
				is_have_decimal_point = false;
			}

			cmds->pop_back();
		}
	}
}

bool OperandCommand::IsNegative() const
{
	return is_negative;
}

bool OperandCommand::IsScientificFormat() const
{
	return is_scientific_format;
}

bool OperandCommand::IsDecimalPointExist() const
{
	return is_have_decimal_point;
}

string OperandCommand::GetToken(char decimal_symbol)
{
	string token{};

	size_t cmds_size = cmds->size();

	for (size_t i = 0; i < cmds_size; i++)
	{
		Command command = cmds->at(i);
		switch (command)
		{
		case PNT:
		{
			token += decimal_symbol;
		}
		break;
		case EXP:
		{
			token += EXPONENT_SIGN;
			if (cmds->at(i + 1) != NEG)
			{
				token += PLUS_SIGN;
			}
		}
		break;
		case NEG:
		{
			token += NEGATE_SIGN;
		}
		break;
		default:
		{
			if (command >= NUM_0 && command <= NUM_9)
			{
				token += static_cast<char>(command - NUM_0 + L'0');
			}
			else
			{
				token += static_cast<char>(command - NUM_A + L'A');
			}
		}
		break;
		}
	}

	// 去除先导零
	// 算法有点奇怪：先标记 0 的起始和结束，再最后一并 erase 。
	for (size_t i = 0; i < token.size(); i++)
	{
		if (token.at(i) != CHAR_ZERO)
		{
			if (token.at(i) == decimal_symbol)
			{
				token.erase(0, i - 1);
			}
			else
			{
				token.erase(0, i);
			}

			if (is_negative)
			{
				token.insert(0, 1, NEGATE_SIGN);
			}

			return token;
		}
	}

	token = CHAR_ZERO;

	return token;
}

CommandType OperandCommand::GetCommandType() const
{
	return CommandType::Operand;
}

void OperandCommand::Accept(_In_ ISerializeCommandVisitor& commandVisitor)
{
	commandVisitor.Visit(*this);
}

string OperandCommand::GetString(Radix const& radix, int32_t precision)
{
	if (is_initialized)
	{
		return value.to_str(radix, NumberFormat::Float, precision);
	}
	return string();
}

mpl::mpf const& OperandCommand::GetMpf()
{
	return value;
}

void OperandCommand::ClearAllAndAppendCommand(Command command)
{
	cmds->clear();
	cmds->push_back(command);
	is_scientific_format = false;
	is_negative = false;
	is_have_decimal_point = false;
}

CM_END