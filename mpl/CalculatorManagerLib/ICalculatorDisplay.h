#pragma once
#ifndef __INCLUDE_ICALCULATOR_DISPLAY_H__
#define __INCLUDE_ICALCULATOR_DISPLAY_H__
#include "IExpressionCommand.h"
CM_BEGIN
class ICalculatorDisplay
{
public:
	virtual void SetPrimaryDisplay(std::string const& primary_str, bool is_err) = 0;
	virtual void SetErrorState(bool is_err) = 0;
	virtual void SetExpressionDisplay(ExpressionCommands const& commands, ExpressionTokens const& tokens) = 0;
	virtual void OnHistoryItemAdded(unsigned int added_item_index) = 0;
};
CM_END
#endif // !__INCLUDE_ICALCULATOR_DISPLAY_H__