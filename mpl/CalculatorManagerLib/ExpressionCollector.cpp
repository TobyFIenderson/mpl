#include "ExpressionCollector.h"
#include "Resource.h"
using namespace std;
CM_BEGIN

#define IS_OPEN_BRACE(x) ((x)->GetCommandType()== CommandType::Parentheses && static_pointer_cast<ParenthesisCommand>(x)->IsOpenParenthesis())
#define IS_CLOSE_BRACE(x) ((x)->GetCommandType()== CommandType::Parentheses && static_pointer_cast<ParenthesisCommand>(x)->IsCloseParenthesis())

ExpressionCollector::~ExpressionCollector()
{
	Clear();
	tokens = nullptr;
	commands = nullptr;
	calculator_display = nullptr;
	calculator_history = nullptr;
}

ExpressionCollector::ExpressionCollector(ICalculatorDisplay* calc_disp, ICalculatorHistory* clac_hist, char decimal_pt, CalculatorMode calc_mode) :
	decimal_symbol(decimal_pt),
	calculator_mode(calc_mode),
	calculator_history(clac_hist),
	calculator_display(calc_disp)
{
	Clear();
}

int ExpressionCollector::AddCommandAndToken(shared_ptr<IExpressionCommand> const& command, string_view token)
{
	if (commands == nullptr)
	{
		commands = make_shared<vector<shared_ptr<IExpressionCommand>>>();
	}
	if (tokens == nullptr)
	{
		tokens = make_shared<vector<string>>();
	}

	commands->push_back(command);
	tokens->push_back(string(token));
	return static_cast<int>(commands->size() - 1);
}

void ExpressionCollector::InsertCommandAndToken(int idx, shared_ptr<IExpressionCommand> const& command, string_view token)
{
	if (commands == nullptr)
	{
		commands = make_shared<vector<shared_ptr<IExpressionCommand>>>();
	}
	if (idx >= commands->size())
	{
		return;
	}
	if (tokens == nullptr)
	{
		tokens = make_shared<vector<string>>();
	}
	if (idx >= tokens->size())
	{
		return;
	}
	commands->insert(commands->begin() + idx, command);
	tokens->insert(tokens->begin() + idx, string(token));
}

shared_ptr<OperandInfo> ExpressionCollector::GetOperandCommandsFromString(string_view num_str)
{
	shared_ptr<OperandInfo> result = make_shared<OperandInfo>();
	result->is_negative = num_str[0] == L'-';
	for (char c : num_str)
	{
		if (c == decimal_symbol)
		{
			result->opnd_cmds->push_back(PNT);
			result->has_decimal_point = true;
		}
		else if (c == L'e')
		{
			result->opnd_cmds->push_back(EXP);
			result->is_scientific_format = true;
		}
		else if (c == L'-')
		{
			result->opnd_cmds->push_back(NEG);
		}
		else if (c == L'+')
		{

		}
		else
		{
			char sub = c >= L'0' && c <= L'9' ? L'0' : L'A';
			Command start = c >= L'0' && c <= L'9' ? NUM_0 : NUM_A;
			result->opnd_cmds->push_back(start + c - sub);
		}
	}

	if (result->is_scientific_format)
	{
		result->has_decimal_point = false;
	}
	return result;
}

void ExpressionCollector::AddOperandCommand(string_view num_str, mpl::mpf const& rat)
{
	shared_ptr<OperandInfo> temp = GetOperandCommandsFromString(num_str);
	shared_ptr<OperandCommand> opnd = make_shared<OperandCommand>(temp->opnd_cmds, temp->is_negative, temp->has_decimal_point, temp->is_scientific_format);
	opnd->Initialize(rat);
	if (LastExpressionIsOperand())
	{
		RemoveLastOperand();
	}
	last_operand_index = AddCommandAndToken(opnd, num_str);
}

bool ExpressionCollector::AddBinaryCommand(Command cmd)
{
	if (IsEmpty())
	{
		return false;
	}

	shared_ptr<IExpressionCommand> const& last_cmd = commands->back();

	// 如果最后一个操作数为开括号 "1+(" 这时加入一个二元运算符 "+"
	// " 1+( + " 这样的话加号 "+" 不会加入到现有的指令序列中。
	if (IS_OPEN_BRACE(last_cmd))
	{
		return false;
	}

	bool change_last_bin_op = false;

	// 如果最后一个已经是二元运算符，就将其覆盖。
	if (last_cmd->GetCommandType() == CommandType::Binary)
	{
		commands->pop_back();
		tokens->pop_back();
		change_last_bin_op = true;
	}

	// 只有在标准计算器下才需要插入改变优先级的括号。
	if (calculator_mode == CalculatorMode::Standard && last_operand_index > 0)
	{
		shared_ptr<IExpressionCommand> before_opnd_cmd = commands->at(static_cast<size_t>(last_operand_index) - 1);
		if (before_opnd_cmd->GetCommandType() == CommandType::Binary)
		{
			shared_ptr<BinaryCommand> bin_cmd = static_pointer_cast<BinaryCommand>(before_opnd_cmd);
			if (CompareBinaryCommandPrecedence(cmd, bin_cmd->GetCommand()) > 0)
			{
				InsertPrecedenceEncloseBrackets();
			}
		}
	}

	AddCommandAndToken(make_shared<BinaryCommand>(cmd), Resource::GetStringFromCommand(cmd));
	return change_last_bin_op;
}

void ExpressionCollector::AddUnaryCommand(Command cmd)
{
	// 假设输入的是Sin


	//
	// 由于标准计算器不允许手动改变计算的优先级（不允许括号操作）
	// 那么标准计算器就不能像科学计算器那样线性的插入一元运算符（详情查看下列注释）
	// 所以标准计算器的表达式输入就会与科学计算器有些不同：
	// 例如假设用户输入序列为下： 1 ADD 2 SIN SIGN
	// 在科学模式下表达式为： 1 + 2 × Sin( negate(
	// 在标准模式下表达式为： 1 + negate( Sin( 2 ) )

	if (LastExpressionIsOperand()) // 最后一个表达式是操作数
	{
		// 1 ---> Sin( 1 )
		// 1 + 2 ---> 1 + Sin( 2 )
		// 1 + ( 2 ---> 1 + ( Sin( 2 )
		// 1 + ( 2 ) ---> 1 + Sin( 2 )
		// 1 + √( 2 ) ---> 1 + Sin( √( 2 ) )
		if (calculator_mode == CalculatorMode::Standard || (calculator_mode == CalculatorMode::Programmer && cmd == NEG))
		{
			// SIGN 是一个比较特殊的指令。

			// 最后的操作数没有被括号包裹，那么就为他添加一对括号。
			if (commands->at(last_operand_index)->GetCommandType() != CommandType::Parentheses)
			{
				InsertBracketsAroundLastOperand();
			}
			InsertCommandAndToken(last_operand_index, make_shared<UnaryCommand>(cmd), Resource::GetStringFromCommand(cmd));
			// 这里 last_operand_index 不变，不需要更新。
		}
		// 1 ---> 1 × Sin(
		// 1 + 2 ---> 1 + 2 × Sin(
		// 1 + ( 2 ---> 1 + ( 2 × Sin(
		// 1 + ( 2 ) ---> 1 + ( 2 ) × Sin(
		// 1 + √( 2 ) ---> 1 + √( 2 ) × Sin(
		else
		{
			AddBinaryCommand(MUL);
			last_operand_index = AddCommandAndToken(make_shared<UnaryCommand>(cmd), Resource::GetStringFromCommand(cmd));
			PushLastOperandStart();
			AddCommandAndToken(make_shared<ParenthesisCommand>(OPENP), Resource::GetStringFromCommand(OPENP));
			open_brace_count++;
		}
	}
	else // 最后一个表达式不是操作数
	{
		// null ---> null
		// ( ---> ( 
		// 1 + ---> 1 + 
		// 1 + Sin( ---> 1 + Sin( 
		if (calculator_mode == CalculatorMode::Standard)
		{
			// 什么也不做
		}
		// null ---> Sin(
		// ( ---> ( Sin(
		// 1 + ---> 1 + Sin(
		// 1 + Sin( ---> 1 + Sin( Sin(
		else
		{
			last_operand_index = AddCommandAndToken(make_shared<UnaryCommand>(cmd), Resource::GetStringFromCommand(cmd));
			PushLastOperandStart();
			AddCommandAndToken(make_shared<ParenthesisCommand>(OPENP), Resource::GetStringFromCommand(OPENP));
			open_brace_count++;
		}
	}
}

void ExpressionCollector::RemoveLastOperand()
{
	commands->erase(commands->begin() + last_operand_index, commands->end());
	tokens->erase(tokens->begin() + last_operand_index, tokens->end());

	if (commands->empty())
	{
		// (1+2)
		Clear();
	}
	else if ((commands->size() == 1 ||
		commands->at(commands->size() - 2)->GetCommandType() == CommandType::Unary ||
		IS_OPEN_BRACE(commands->back())) && !operand_start_per_brace.empty())
	{
		// ((1+2)			-OR-
		// Sin((1+2)		-OR- 
		// 1*(2+3)			-OR- 
		// (1+2)*(3+4)		-OR-
		// 1+(1+2)*(3+4)	-OR-
		// 1+((1+2)

		last_operand_index = operand_start_per_brace.back();
	}
	else
	{

		last_operand_index = 0;
	}
}

void ExpressionCollector::AddOpenBrace()
{
	if (LastExpressionIsOperand())
	{
		AddBinaryCommand(MUL);
	}
	last_operand_index = AddCommandAndToken(make_shared<ParenthesisCommand>(OPENP), Resource::GetStringFromCommand(OPENP));
	PushLastOperandStart();
	open_brace_count++;
}

void ExpressionCollector::AddCloseBrace()
{
	if (open_brace_count == 0)
	{
		return;
	}
	shared_ptr<IExpressionCommand> const& last_cmd = commands->back();
	// 如果有两个配对的括号内没有任何内容的情况下，在两括号内加入一个 0
	if (IS_OPEN_BRACE(last_cmd))
	{
		AddOperandCommand("0", 0);
	}
	AddCommandAndToken(make_shared<ParenthesisCommand>(CLOSEP), Resource::GetStringFromCommand(CLOSEP));
	PopLastOprandStart();
	open_brace_count--;
}

bool ExpressionCollector::LastExpressionIsOperand() const
{
	if (!IsEmpty())
	{
		shared_ptr<IExpressionCommand> last_cmd = commands->back();
		if (last_cmd->GetCommandType() == CommandType::Operand || IS_CLOSE_BRACE(last_cmd))
		{
			return true;
		}
	}

	return false;
}

void ExpressionCollector::SetCalculatorMode(CalculatorMode calc_mode)
{
	calculator_mode = calc_mode;
	Clear();
}

void ExpressionCollector::SetDecimalSymbol(char dec_pt)
{
	decimal_symbol = dec_pt;
	Clear();
}

void ExpressionCollector::InsertPrecedenceEncloseBrackets()
{
	// 更新 last_operand_index 的值。避免将括号插入错误的地方。
	last_operand_index = operand_start_per_brace.empty() ? 0 : operand_start_per_brace.back();

	CommandType last_opnd_cmd_t = commands->at(last_operand_index)->GetCommandType();
	if (last_opnd_cmd_t == CommandType::Unary)
	{
		last_operand_index += 2;
	}
	else if (last_opnd_cmd_t == CommandType::Parentheses)
	{
		// ((1+2)-(3+4)*5 ---> ( (^here^ (1+2)-(3+4))*5
		if (last_operand_index > 0 && commands->at(static_cast<size_t>(last_operand_index) - 1)->GetCommandType() == CommandType::Parentheses)
		{
			last_operand_index++;
		}
		// (1+2)-(3+4)*5 ---> (^here^ (1+2)-(3+4))*5
	}
	InsertBracketsAroundLastOperand();
}

void ExpressionCollector::InsertBracketsAroundLastOperand()
{
	InsertCommandAndToken(last_operand_index, make_shared<ParenthesisCommand>(OPENP), Resource::GetStringFromCommand(OPENP));
	AddCommandAndToken(make_shared<ParenthesisCommand>(CLOSEP), Resource::GetStringFromCommand(CLOSEP));
	// 这里 last_operand_index 不变，不需要更新。
}

ExpressionCommands const& ExpressionCollector::GetCommands() const
{
	return commands;
}

ExpressionTokens const& ExpressionCollector::GetTokens() const
{
	return tokens;
}

void ExpressionCollector::CompleteEquation()
{
	// 空表达式默认为 0 
	if (IsEmpty())
	{
		AddOperandCommand("0", 0);
	}

	// 1 + 3 + ---> 1 + 3
	if (commands->back()->GetCommandType() == CommandType::Binary)
	{
		commands->pop_back();
		tokens->pop_back();
	}

	while (open_brace_count > 0)
	{
		AddCloseBrace();
	}

	// 只有在这里，tokens 会比 commands 多出来一个 'EQU' 
	tokens->push_back(Resource::GetStringFromCommand(EQU));
}

void ExpressionCollector::CompleteHistoryLine(string_view expression_result)
{
	if (calculator_history != nullptr)
	{
		unsigned int added_item_index = calculator_history->AddToHistory(commands, tokens, expression_result);
		if (calculator_display != nullptr)
		{
			calculator_display->OnHistoryItemAdded(added_item_index);
		}
	}

	commands = nullptr;
	tokens = nullptr;
	Clear();
}

bool ExpressionCollector::IsEmpty() const
{
	return (commands == nullptr || commands->empty()) || (tokens == nullptr || tokens->empty());
}

void ExpressionCollector::Clear()
{
	open_brace_count = 0;
	last_operand_index = 0;
	operand_start_per_brace.clear();

	if (commands != nullptr)
	{
		commands->clear();
	}

	if (tokens != nullptr)
	{
		tokens->clear();
	}
}

void ExpressionCollector::PushLastOperandStart()
{
	shared_ptr<IExpressionCommand> last_operand_cmd = commands->at(last_operand_index);
	if (last_operand_cmd->GetCommandType() == CommandType::Unary || IS_OPEN_BRACE(last_operand_cmd))
	{
		operand_start_per_brace.push_back(last_operand_index);
	}
}

void ExpressionCollector::PopLastOprandStart()
{
	if (!operand_start_per_brace.empty())
	{
		last_operand_index = operand_start_per_brace.back();
		operand_start_per_brace.pop_back();
	}
	else
	{
		last_operand_index = 0;
	}
}

void ExpressionCollector::UpdateCollectedExpression(Radix const& radix, int precision)
{
	if (IsEmpty())
	{
		return;
	}

	for (int i = 0; i < commands->size(); i++)
	{
		shared_ptr<IExpressionCommand>& cur_cmd = commands->at(i);
		if (cur_cmd->GetCommandType() == CommandType::Operand)
		{
			shared_ptr<OperandCommand> opnd_cmd = static_pointer_cast<OperandCommand>(cur_cmd);
			if (opnd_cmd != nullptr)
			{
				tokens->at(i) = opnd_cmd->GetString(radix, precision);
				opnd_cmd->SetCommands(GetOperandCommandsFromString(tokens->at(i))->opnd_cmds);
			}
		}
	}
}

void ExpressionCollector::SetExpressionDisplay()
{
	if (calculator_display != nullptr)
	{
		calculator_display->SetExpressionDisplay(commands, tokens);
	}
}

CM_END