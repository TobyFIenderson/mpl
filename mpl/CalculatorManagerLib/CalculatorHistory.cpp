#include "CalculatorHistory.h"
#include <assert.h>
using namespace std;
CM_BEGIN
CalculatorHistory::CalculatorHistory(const size_t o_max_size) : max_size(o_max_size) {}

uint32_t CalculatorHistory::AddToHistory(ExpressionCommands const& commands, ExpressionTokens const& tokens, string_view result)
{
	shared_ptr<HistoryItem> his_item = make_shared<HistoryItem>();

	his_item->tokens = tokens;
	his_item->commands = commands;
	his_item->result = string(result);

	string temp{};

	if (!tokens->empty())
	{
		temp += tokens->front();
		for (size_t i = 1; i < tokens->size(); i++)
		{
			temp += L' ';
			temp += tokens->at(i);
		}
	}

	his_item->expression = move(temp);

	return AddHistoryItem(his_item);
}

vector<shared_ptr<HistoryItem>> const& CalculatorHistory::GetHistory() const
{
	return history_items;
}

shared_ptr<HistoryItem> const& CalculatorHistory::GetHistoryItem(uint32_t idx) const
{
	assert(idx < history_items.size());
	return history_items.at(idx);
}

void CalculatorHistory::Clear()
{
	history_items.clear();
}

uint32_t CalculatorHistory::AddHistoryItem(shared_ptr<HistoryItem> const& history_item)
{
	if (history_items.size() >= max_size)
	{
		history_items.erase(history_items.begin());
	}

	history_items.push_back(history_item);
	return static_cast<uint32_t>(history_items.size() - 1);
}

bool CalculatorHistory::RemoveItem(uint32_t idx)
{
	if (idx < history_items.size())
	{
		history_items.erase(history_items.begin() + idx);
		return true;
	}
	return false;
}

size_t CalculatorHistory::MaxHistorySize() const
{
	return max_size;
}
CM_END