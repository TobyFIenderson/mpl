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

	// ���� calc_engine ���ȡ���� rat ����� memory ��ͷ��
	void MemorizeNumber();

	// �� calc_engine ���Ѿ��洢�� rational ���� memory[mem_index]
	void MemorizedNumberChanged(size_t mem_index);

	// ѡ�� memory[mem_index] �������͸� calc_engine
	void MemorizedNumberSelect(size_t mem_index);

	// ���� calc_engine �� ProcessCommand(MPLUS);
	void MemorizedNumberPlus(size_t mem_index);

	// ���� calc_engine �� ProcessCommand(MPLUS);
	void MemorizedNumberMinus(size_t mem_index);

	// �� calc_engine �� current_value ��Ϊ memory[mem_index]
	void MemorizedNumberRecall(size_t mem_index);

	void MemorizedNumberClear(size_t mem_index);

	void MemorizedNumberClearAll();

	void UpdateDisplayStrings();

	std::vector<std::string_view> GetDisplayStrings() const;

	MemoryItem CreateMemoryItem(mpl::mpf const& val);

private:
	// �������ڻ�ȡ��ͬ�������������ʾ�ַ�����ʽ��
	CalculatorEngine* calculator_engine;
	std::vector<MemoryItem> memory;
};

CM_END


#endif // !__INCLUDE_CALCULATOR_MEMORY_H__