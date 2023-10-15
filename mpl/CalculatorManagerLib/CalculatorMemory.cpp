#include "CalculatorMemory.h"
using namespace std;
using namespace mpl;
CM_BEGIN

CalculatorMemory::CalculatorMemory() :calculator_engine(nullptr), memory() {}

CalculatorMemory::CalculatorMemory(CalculatorEngine* calc_engine) : calculator_engine(calc_engine), memory() {}

void CalculatorMemory::SetCalculatorEngine(CalculatorEngine* calc_engine)
{
	calculator_engine = calc_engine;
}

bool CalculatorMemory::IsEmpty() const
{
	return memory.empty();
}

void CalculatorMemory::MemorizeNumber()
{
	if (calculator_engine->IsInErrorState())
	{
		return;
	}

	calculator_engine->ProcessCommand(MSTORE);
	memory.insert(memory.begin(), MemoryItem{});
	MemorizedNumberChanged(0);

	if (memory.size() > MAX_MEM_SIZE)
	{
		memory.resize(MAX_MEM_SIZE);
	}
}

void CalculatorMemory::MemorizedNumberChanged(size_t mem_index)
{
	memory[mem_index] = CreateMemoryItem(calculator_engine->GetMemorizedNumber());
}

void CalculatorMemory::MemorizedNumberSelect(size_t mem_index)
{
	calculator_engine->SetMemorizedNumber(memory.at(mem_index)->first);
}

void CalculatorMemory::MemorizedNumberPlus(size_t mem_index)
{
	if (calculator_engine->IsInErrorState())
	{
		return;
	}

	if (IsEmpty())
	{
		MemorizeNumber();
	}
	else
	{
		MemorizedNumberSelect(mem_index);
		calculator_engine->ProcessCommand(MPLUS);
		MemorizedNumberChanged(mem_index);
	}
}

void CalculatorMemory::MemorizedNumberMinus(size_t mem_index)
{
	if (calculator_engine->IsInErrorState())
	{
		return;
	}

	if (IsEmpty())
	{
		calculator_engine->ProcessCommand(MSTORE);
		mpf memorized_number = calculator_engine->GetMemorizedNumber();
		memorized_number.mant.sign *= -1;;

		memory.emplace_back(CreateMemoryItem(memorized_number));
	}
	else
	{
		MemorizedNumberSelect(mem_index);
		calculator_engine->ProcessCommand(MMINUS);
		MemorizedNumberChanged(mem_index);
	}
}

void CalculatorMemory::MemorizedNumberRecall(size_t mem_index)
{
	if (calculator_engine->IsInErrorState())
	{
		return;
	}
	MemorizedNumberSelect(mem_index);
	calculator_engine->ProcessCommand(MRECALL);
}

void CalculatorMemory::MemorizedNumberClear(size_t mem_index)
{
	if (mem_index < memory.size())
	{
		memory.erase(memory.begin() + mem_index);
	}
}

void CalculatorMemory::MemorizedNumberClearAll()
{
	memory.clear();
	calculator_engine->ProcessCommand(MCLEAR);
}

void CalculatorMemory::UpdateDisplayStrings()
{
	Radix radix = calculator_engine->GetCurrentRadix();
	for (MemoryItem& item : memory)
	{
		item->second = calculator_engine->GroupDigitsPerRadix(calculator_engine->GetStringForDisplay(item->first, radix), radix);
	}
}

vector<string_view> CalculatorMemory::GetDisplayStrings() const
{
	vector<string_view> result;

	for (MemoryItem const& item : memory)
	{
		result.push_back(item->second);
	}

	return result;
}

MemoryItem CalculatorMemory::CreateMemoryItem(mpf const& val)
{
	Radix const& radix = calculator_engine->GetCurrentRadix();
	string&& display_string = calculator_engine->GroupDigitsPerRadix(calculator_engine->GetStringForDisplay(val, radix), radix);
	return make_unique<pair<mpf, string>>(val, move(display_string));
}

CM_END
