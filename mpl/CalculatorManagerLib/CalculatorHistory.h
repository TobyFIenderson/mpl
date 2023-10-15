#pragma once
#ifndef __INCLUDE_CALCULATOR_HISTORY_H__
#define __INCLUDE_CALCULATOR_HISTORY_H__

#include "IExpressionCommand.h"
#include "ICalculatorHistory.h"
CM_BEGIN

inline constexpr size_t DEFAULT_HISTORY_MAX_SIZE = 512;

class CalculatorHistory : public ICalculatorHistory
{
public:
	CalculatorHistory(const size_t max_size);
	uint32_t AddToHistory(ExpressionCommands const& commands, ExpressionTokens const& tokens,/* expression ”…tokens…˙≥… */std::string_view result) override;
	std::vector<std::shared_ptr<HistoryItem>> const& GetHistory() const;
	std::shared_ptr<HistoryItem> const& GetHistoryItem(uint32_t idx) const;
	void Clear() override;
	uint32_t AddHistoryItem(std::shared_ptr<HistoryItem> const& history_item);
	bool RemoveItem(uint32_t idx);
	size_t MaxHistorySize() const;
private:
	std::vector<std::shared_ptr<HistoryItem>> history_items;
	const size_t max_size;
};

CM_END

#endif // !__INCLUDE_CALCULATOR_HISTORY_H__
