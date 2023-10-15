#pragma once
#ifndef __INCLUDE_CALCULATOR_MEMORY_H__
#define __INCLUDE_CALCULATOR_MEMORY_H__

#include "CalculatorEngine.h"

CM_BEGIN

using MemoryItem = std::unique_ptr<std::pair<mpl::mpf, std::string>>;

inline constexpr size_t MAX_MEM_SIZE = 128;

class CalculatorMemory
{
public:
	CalculatorMemory();
	CalculatorMemory(CalculatorEngine* calc_engine);

	void SetCalculatorEngine(CalculatorEngine* calc_engine);

	bool IsEmpty() const;

	// 将从 calc_engine 里获取到的 rat 插入进 memory 的头部
	void MemorizeNumber();

	// 将 calc_engine 里已经存储的 rational 送入 memory[mem_index]
	void MemorizedNumberChanged(size_t mem_index);

	// 选择 memory[mem_index] 并将其送给 calc_engine
	void MemorizedNumberSelect(size_t mem_index);

	// 调用 calc_engine 的 ProcessCommand(MPLUS);
	void MemorizedNumberPlus(size_t mem_index);

	// 调用 calc_engine 的 ProcessCommand(MPLUS);
	void MemorizedNumberMinus(size_t mem_index);

	// 将 calc_engine 的 current_value 置为 memory[mem_index]
	void MemorizedNumberRecall(size_t mem_index);

	void MemorizedNumberClear(size_t mem_index);

	void MemorizedNumberClearAll();

	void UpdateDisplayStrings();

	std::vector<std::string_view> GetDisplayStrings() const;

	MemoryItem CreateMemoryItem(mpl::mpf const& val);

private:
	// 仅仅用于获取不同计算器引擎的显示字符串格式。
	CalculatorEngine* calculator_engine;
	std::vector<MemoryItem> memory;
};

CM_END


#endif // !__INCLUDE_CALCULATOR_MEMORY_H__