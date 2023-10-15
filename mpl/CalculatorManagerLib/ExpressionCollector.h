#pragma once
#ifndef __INCLUDE_EXPRESSION_COLLECTOR_H__
#define __INCLUDE_EXPRESSION_COLLECTOR_H__
#include "CalculatorManagerLibNS.h"
#include "ExpressionCommand.h"
#include "CalculatorMode.h"
#include "ICalculatorHistory.h" /* �����Ŀȥ history */
#include "ICalculatorDisplay.h" /* ��ʾ���ʽ */
CM_BEGIN

// �ýṹ����� GetOperandCommandsFromString ����ʹ�ã�
// ּ��ʵ�ַ��ظ������Ϣ��
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

// ���еı��ʽĬ����ʼΪһ�� 0 ����ʹ������ʽ�ǿձ��ʽ��

class ExpressionCollector
{
public:
	~ExpressionCollector();
	ExpressionCollector(ICalculatorDisplay* calc_disp, ICalculatorHistory* clac_hist, char decimal_pt, CalculatorMode calc_mode);

	/* ��Ӳ���ָ�����Ӧ�� token �����ռ��ı��ʽ֮�� */
	int AddCommandAndToken(std::shared_ptr<IExpressionCommand> const& command, std::string_view token);
	/* �������ָ�����Ӧ�� token �����ռ��ı��ʽ�ľ���λ�� */
	void InsertCommandAndToken(int idx, std::shared_ptr<IExpressionCommand> const& command, std::string_view str);
	/* ��������ַ�������Ϊ��������OperandInfo�� */
	std::shared_ptr<OperandInfo> GetOperandCommandsFromString(std::string_view num_str);
	/* ��������������ʽĩβ��ע��������ʽĩβ�Ѿ�Ϊ����������ô���Ḳ�ǾɵĲ����� */
	void AddOperandCommand(std::string_view num_str, mpl::mpf const& rat);
	bool AddBinaryCommand(Command cmd);
	void AddUnaryCommand(Command cmd);

	void RemoveLastOperand();
	void AddOpenBrace();
	void AddCloseBrace();
	bool LastExpressionIsOperand() const;
	void SetCalculatorMode(CalculatorMode calc_mode);
	void SetDecimalSymbol(char dec_pt);

	/* ֻ���ڱ�׼�������²���Ҫ����ı����ȼ������� */
	void InsertPrecedenceEncloseBrackets();
	void InsertBracketsAroundLastOperand();

	void CompleteEquation();
	void CompleteHistoryLine(std::string_view expression_result);

	bool IsEmpty() const;

	void Clear();
	void PushLastOperandStart();
	void PopLastOprandStart();

	/* �ڳ���Ա�������н��Ƹı��˾�Ҫ�ô˺������²��������ַ�����ʾ */
	void UpdateCollectedExpression(Radix const& radix, int precision);

	void SetExpressionDisplay();


	ExpressionCommands const& GetCommands() const;
	ExpressionTokens const& GetTokens() const;

private:
	ICalculatorHistory* calculator_history;
	ICalculatorDisplay* calculator_display;

	char decimal_symbol;
	int open_brace_count;
	std::vector<int> operand_start_per_brace;	// ���ڼ�¼���� "1+( <-- here" ���� "1+Sin <-- here (" (�����Ż���һԪ�����)��λ�á�
	int last_operand_index;						// ������һ������������ commands �е�������
	ExpressionCommands commands;				// ��¼ÿһ���������Ͳ�����
	ExpressionTokens tokens;					// �� commands һһ��Ӧ���ַ��������ʽ
	CalculatorMode calculator_mode;
};



CM_END
#endif // !__INCLUDE_EXPRESSION_COLLECTOR_H__