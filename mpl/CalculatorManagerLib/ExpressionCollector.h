#pragma once
#ifndef __INCLUDE_EXPRESSION_COLLECTOR_H__
#define __INCLUDE_EXPRESSION_COLLECTOR_H__
#include "CalculatorManagerLibNS.h"
#include "ExpressionCommand.h"
#include "CalculatorMode.h"
#include "ICalculatorHistory.h" /* 添加条目去 history */
#include "ICalculatorDisplay.h" /* 显示表达式 */
CM_BEGIN

// 该结构体仅被 GetOperandCommandsFromString 函数使用，
// 旨在实现返回更多的信息。
struct OperandInfo
{
	std::shared_ptr<std::vector<Command>> opnd_cmds;
	bool is_negative;
	bool has_decimal_point;
	bool is_scientific_format;
	OperandInfo() : opnd_cmds(std::make_shared<std::vector<Command>>()), is_negative(false), has_decimal_point(false), is_scientific_format(false) {}
	~OperandInfo()
	{
		opnd_cmds = nullptr;
	}
};

// 所有的表达式默认起始为一个 0 ，即使这个表达式是空表达式。

class ExpressionCollector
{
public:
	~ExpressionCollector();
	ExpressionCollector(ICalculatorDisplay* calc_disp, ICalculatorHistory* clac_hist, char decimal_pt, CalculatorMode calc_mode);

	/* 添加操作指令和相应的 token 到已收集的表达式之后 */
	int AddCommandAndToken(std::shared_ptr<IExpressionCommand> const& command, std::string_view token);
	/* 插入操作指令和相应的 token 到已收集的表达式的具体位置 */
	void InsertCommandAndToken(int idx, std::shared_ptr<IExpressionCommand> const& command, std::string_view str);
	/* 将输入的字符串解析为操作数（OperandInfo） */
	std::shared_ptr<OperandInfo> GetOperandCommandsFromString(std::string_view num_str);
	/* 将操作数加入表达式末尾，注意如果表达式末尾已经为操作数，那么将会覆盖旧的操作数 */
	void AddOperandCommand(std::string_view num_str, mpl::mpf const& rat);
	bool AddBinaryCommand(Command cmd);
	void AddUnaryCommand(Command cmd);

	void RemoveLastOperand();
	void AddOpenBrace();
	void AddCloseBrace();
	bool LastExpressionIsOperand() const;
	void SetCalculatorMode(CalculatorMode calc_mode);
	void SetDecimalSymbol(char dec_pt);

	/* 只有在标准计算器下才需要插入改变优先级的括号 */
	void InsertPrecedenceEncloseBrackets();
	void InsertBracketsAroundLastOperand();

	void CompleteEquation();
	void CompleteHistoryLine(std::string_view expression_result);

	bool IsEmpty() const;

	void Clear();
	void PushLastOperandStart();
	void PopLastOprandStart();

	/* 在程序员计算器中进制改变了就要用此函数更新操作数的字符串表示 */
	void UpdateCollectedExpression(Radix const& radix, int precision);

	void SetExpressionDisplay();


	ExpressionCommands const& GetCommands() const;
	ExpressionTokens const& GetTokens() const;

private:
	ICalculatorHistory* calculator_history;
	ICalculatorDisplay* calculator_display;

	char decimal_symbol;
	int open_brace_count;
	std::vector<int> operand_start_per_brace;	// 用于记录比如 "1+( <-- here" 或者 "1+Sin <-- here (" (左括号或者一元运算符)的位置。
	int last_operand_index;						// 标记最后一个操作数的在 commands 中的索引。
	ExpressionCommands commands;				// 记录每一个操作符和操作数
	ExpressionTokens tokens;					// 与 commands 一一对应的字符串表达形式
	CalculatorMode calculator_mode;
};



CM_END
#endif // !__INCLUDE_EXPRESSION_COLLECTOR_H__