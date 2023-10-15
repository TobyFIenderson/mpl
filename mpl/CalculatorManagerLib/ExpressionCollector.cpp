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

	// ������һ��������Ϊ������ "1+(" ��ʱ����һ����Ԫ����� "+"
	// " 1+( + " �����Ļ��Ӻ� "+" ������뵽���е�ָ�������С�
	if (IS_OPEN_BRACE(last_cmd))
	{
		return false;
	}

	bool change_last_bin_op = false;

	// ������һ���Ѿ��Ƕ�Ԫ��������ͽ��串�ǡ�
	if (last_cmd->GetCommandType() == CommandType::Binary)
	{
		commands->pop_back();
		tokens->pop_back();
		change_last_bin_op = true;
	}

	// ֻ���ڱ�׼�������²���Ҫ����ı����ȼ������š�
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
	// �����������Sin


	//
	// ���ڱ�׼�������������ֶ��ı��������ȼ������������Ų�����
	// ��ô��׼�������Ͳ������ѧ�������������ԵĲ���һԪ�����������鿴����ע�ͣ�
	// ���Ա�׼�������ı��ʽ����ͻ����ѧ��������Щ��ͬ��
	// ��������û���������Ϊ�£� 1 ADD 2 SIN SIGN
	// �ڿ�ѧģʽ�±��ʽΪ�� 1 + 2 �� Sin( negate(
	// �ڱ�׼ģʽ�±��ʽΪ�� 1 + negate( Sin( 2 ) )

	if (LastExpressionIsOperand()) // ���һ�����ʽ�ǲ�����
	{
		// 1 ---> Sin( 1 )
		// 1 + 2 ---> 1 + Sin( 2 )
		// 1 + ( 2 ---> 1 + ( Sin( 2 )
		// 1 + ( 2 ) ---> 1 + Sin( 2 )
		// 1 + ��( 2 ) ---> 1 + Sin( ��( 2 ) )
		if (calculator_mode == CalculatorMode::Standard || (calculator_mode == CalculatorMode::Programmer && cmd == NEG))
		{
			// SIGN ��һ���Ƚ������ָ�

			// ���Ĳ�����û�б����Ű�������ô��Ϊ�����һ�����š�
			if (commands->at(last_operand_index)->GetCommandType() != CommandType::Parentheses)
			{
				InsertBracketsAroundLastOperand();
			}
			InsertCommandAndToken(last_operand_index, make_shared<UnaryCommand>(cmd), Resource::GetStringFromCommand(cmd));
			// ���� last_operand_index ���䣬����Ҫ���¡�
		}
		// 1 ---> 1 �� Sin(
		// 1 + 2 ---> 1 + 2 �� Sin(
		// 1 + ( 2 ---> 1 + ( 2 �� Sin(
		// 1 + ( 2 ) ---> 1 + ( 2 ) �� Sin(
		// 1 + ��( 2 ) ---> 1 + ��( 2 ) �� Sin(
		else
		{
			AddBinaryCommand(MUL);
			last_operand_index = AddCommandAndToken(make_shared<UnaryCommand>(cmd), Resource::GetStringFromCommand(cmd));
			PushLastOperandStart();
			AddCommandAndToken(make_shared<ParenthesisCommand>(OPENP), Resource::GetStringFromCommand(OPENP));
			open_brace_count++;
		}
	}
	else // ���һ�����ʽ���ǲ�����
	{
		// null ---> null
		// ( ---> ( 
		// 1 + ---> 1 + 
		// 1 + Sin( ---> 1 + Sin( 
		if (calculator_mode == CalculatorMode::Standard)
		{
			// ʲôҲ����
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
	// �����������Ե�������û���κ����ݵ�����£����������ڼ���һ�� 0
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
	// ���� last_operand_index ��ֵ�����⽫���Ų������ĵط���
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
	// ���� last_operand_index ���䣬����Ҫ���¡�
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
	// �ձ��ʽĬ��Ϊ 0 
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

	// ֻ�������tokens ��� commands �����һ�� 'EQU' 
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