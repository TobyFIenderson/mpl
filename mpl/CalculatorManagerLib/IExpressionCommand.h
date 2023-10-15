#pragma once
#ifndef __INCLUDE_IEXPRESSION_COMMAND_H__
#define __INCLUDE_IEXPRESSION_COMMAND_H__
#include <memory>
#include <vector>
#include "Command.h"
#include <string>
CM_BEGIN

class ISerializeCommandVisitor;

class IExpressionCommand
{
public:
	virtual CommandType GetCommandType() const = 0;
	virtual void Accept(ISerializeCommandVisitor& command_visitor) = 0;
};

class IOperatorCommand : public IExpressionCommand
{
public:
	virtual void SetCommand(Command cmd) = 0;
	virtual Command GetCommand() const = 0;
};

class IParenthesisCommand : public IOperatorCommand {};
class IUnaryCommand : public IOperatorCommand {};
class IBinaryCommand : public IOperatorCommand {};

class IOperandCommand : public IExpressionCommand
{
public:
	virtual std::shared_ptr<std::vector<Command>>const& GetCommands() const = 0;
	virtual void AppendCommand(Command cmd) = 0;
	virtual void ToggleSign() = 0;
	virtual void RemoveBack() = 0;
	virtual bool IsNegative() const = 0;
	virtual bool IsScientificFormat() const = 0;
	virtual bool IsDecimalPointExist() const = 0;
	virtual std::string GetToken(char dec_sep) = 0;
	virtual void SetCommands(std::shared_ptr<std::vector<Command>>const& cmds) = 0;
};

using ExpressionCommands = std::shared_ptr<std::vector<std::shared_ptr<IExpressionCommand>>>;

using ExpressionTokens = std::shared_ptr<std::vector<std::string>>; // 与 ExpressionCommands 一一对应的字符串表达形式。

CM_END
#endif // !__INCLUDE_IEXPRESSION_COMMAND_H__