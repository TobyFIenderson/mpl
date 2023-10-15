#pragma once
#ifndef __INCLUDE_ICALCULATOR_HISTORY_H__
#define __INCLUDE_ICALCULATOR_HISTORY_H__

#include "IExpressionCommand.h"

CM_BEGIN

struct HistoryItem
{
	ExpressionCommands commands;
	ExpressionTokens tokens;
	std::string expression;
	std::string result;
};

class ICalculatorHistory
{
public:
	virtual ~ICalculatorHistory() {}
	virtual unsigned int AddToHistory(
		ExpressionCommands const& commands,
		ExpressionTokens const& tokens,
		/* expression ”…tokens…˙≥… */
		std::string_view result) = 0;
	virtual void Clear() = 0;
};

CM_END

#endif // !__INCLUDE_ICALCULATOR_HISTORY_H__