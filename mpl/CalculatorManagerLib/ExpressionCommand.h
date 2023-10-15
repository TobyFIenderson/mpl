#pragma once
#ifndef __INCLUDE_EXPRESSION_COMMAND_H__
#define __INCLUDE_EXPRESSION_COMMAND_H__
#include "IExpressionCommand.h"
#include "Radix.h"
#include "../mpl/mpf/mpf.h"
CM_BEGIN

class ParenthesisCommand final : public IParenthesisCommand
{
public:
	ParenthesisCommand(Command cmd);
	void SetCommand(Command cmd) override;
	Command GetCommand() const override;
	CommandType GetCommandType() const override;
	void Accept(ISerializeCommandVisitor& cmd_visitor) override;
	bool IsOpenParenthesis() const;
	bool IsCloseParenthesis() const;
private:
	Command cmd;
};

class UnaryCommand final : public IUnaryCommand
{
public:
	UnaryCommand(Command cmd);
	void SetCommand(Command cmd) override;
	Command GetCommand() const override;
	CommandType GetCommandType() const override;
	void Accept(ISerializeCommandVisitor& cmd_visitor) override;
private:
	Command cmd;
};

class BinaryCommand final : public IBinaryCommand
{
public:
	BinaryCommand(Command command);
	void SetCommand(Command command) override;
	Command GetCommand() const override;
	CommandType GetCommandType() const override;
	void Accept(ISerializeCommandVisitor& commandVisitor) override;
private:
	Command cmd;
};

class OperandCommand final : public IOperandCommand
{
public:
	OperandCommand(std::shared_ptr<std::vector<Command>> const& cmds, bool is_neg, bool has_dec_pnt, bool sci_fmt);
	void Initialize(mpl::mpf const& rat);

	std::shared_ptr<std::vector<Command>> const& GetCommands() const override;
	void SetCommands(std::shared_ptr<std::vector<Command>> const& cmds) override;
	void AppendCommand(Command command) override;
	void ToggleSign() override;
	void RemoveBack() override;
	bool IsNegative() const override;
	bool IsScientificFormat() const override;
	bool IsDecimalPointExist() const override;
	std::string GetToken(char decimalSymbol) override;
	CommandType GetCommandType() const override;
	void Accept(ISerializeCommandVisitor& commandVisitor) override;
	std::string GetString(Radix const& radix, int32_t precision);
	mpl::mpf const& GetMpf();

private:
	std::shared_ptr<std::vector<Command>> cmds;
	bool is_negative;
	bool is_scientific_format;
	bool is_have_decimal_point;
	bool is_initialized;
	mpl::mpf value;
	void ClearAllAndAppendCommand(Command command);
};

class ISerializeCommandVisitor
{
public:
	virtual void Visit(OperandCommand& opndCmd) = 0;
	virtual void Visit(UnaryCommand& unaryCmd) = 0;
	virtual void Visit(BinaryCommand& binaryCmd) = 0;
	virtual void Visit(ParenthesisCommand& paraCmd) = 0;
};

CM_END
#endif // !__INCLUDE_EXPRESSION_COMMAND_H__