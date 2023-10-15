#pragma once
#ifndef __INCLUDE_CALCULATOR_MANAGER_H__
#define __INCLUDE_CALCULATOR_MANAGER_H__
#include "CalculatorHistory.h"
#include "CalculatorMemory.h"
#include <array>
CM_BEGIN

inline constexpr AngleType DEFAULT_ANGLE_TYPE = AngleType::Degree;
inline constexpr ShiftType DEFAULT_SHIFT_TYPE = ShiftType::Arthmetic;

inline constexpr int STANDARD_PRECISION = 16;
inline constexpr int SCIENTIFIC_PRECISION = 1024;
inline constexpr int PROGRAMMER_PRECISION = 64;
inline constexpr int CALCULATOR_PRECISION[] = { STANDARD_PRECISION ,SCIENTIFIC_PRECISION ,PROGRAMMER_PRECISION };

class CalculatorManager
{
public:
	CalculatorManager(ICalculatorDisplay* display_callback, IResourceProvider* resource_provider);
	~CalculatorManager();

	void ProcessCommand(Command command);

	void Reset(bool clear_memory = true);
	void SetCurrentCalculatorEngineRadix(RadixType radix_type);
	void SetCurrentCalculatorEngineDisplayStringFormat(NumberFormat number_format);
	void SetCurrentCalculatorEngineNumberWidth(NumberWidthType number_width_type);
	void SetCurrentCalculatorEngineMemorizedNumber(mpl::mpf const& rat);
	void SetCurrentCalculatorEngineMode(CalculatorMode calculator_mode);
	void SetCurrentCalculatorEnginePrecision(int new_precision);
	void UpdateCurrentCalculatorEngineMaxIntegerDigit();
	char GetCurrentCalcualtorEngineDecimalSeparator();
	std::string GetCurrentCalculatorEngineCalcualteResultForRadix(Radix const& radix, bool group_digit);

	bool IsCurrentCalculatorEngineRecording();
	bool isCurrentCalculatorEngineInputEmpty();

	void MemorizeNumber();
	void MemorizedNumberLoad(unsigned int idx_of_mem);
	void MemorizedNumberClear(unsigned int idx_of_mem);
	void MemorizedNumberPlus(unsigned int idx_of_mem);
	void MemorizedNumberMinus(unsigned int idx_of_mem);
	void MemorizedNumberClearAll();

	std::vector<std::shared_ptr<HistoryItem>> const& GetCurrentHistoryItems();
	std::vector<std::shared_ptr<HistoryItem>> const& GetCurrentHistoryItems(CalculatorMode calculator_mode);
	std::shared_ptr<HistoryItem> const& GetCurrentHistoryItem(unsigned int idx_of_his);
	bool RemoveCurrentHistoryItem(unsigned int idx_of_his);
	void ClearCurrentHistory();
	size_t GetCurrentHistoryMaxSize();
	void SetIsInHistoryItemLoadModeFlag(bool ishis_item_load_mode);

	AngleType GetCurrentAngleType();

private:
	ICalculatorDisplay* calculator_display;
	IResourceProvider* resource_provider;

	CalculatorEngine* current_calculator_engine;
	std::array<CalculatorEngine*, 3> calculator_engines;

	CalculatorHistory* current_calculator_history;
	std::array<CalculatorHistory*, 3> calculator_historys;

	CalculatorMemory calculator_memory;			// 三个计算器引擎共享一个内存。

	AngleType angle_type;
	ShiftType shift_type;
	bool is_invert;
	bool is_hyp;
	bool ishistory_item_load_mode;
};

CM_END

#endif // !__INCLUDE_CALCULATOR_MANAGER_H__
