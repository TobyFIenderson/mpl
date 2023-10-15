#pragma once
#ifndef __INCLUDE_CALCULATOR_ENGINE_H__
#define __INCLUDE_CALCULATOR_ENGINE_H__
#include "MpfMath.h"
#include "Radix.h"
#include "NumberWidth.h"
#include "NumberFormat.h"
#include "CalculatorMode.h"
#include "ICalculatorDisplay.h"
#include "ICalculatorHistory.h"
#include "ExpressionCollector.h"
#include "CalculatorInput.h"
#include "Resource.h"
#include <unordered_map>
#include <random>
CM_BEGIN

inline constexpr const char* DEFAULT_NUM_STR = "0";
inline constexpr int DEFAULT_MAX_DIGITS = 1024;
inline constexpr int DEFAULT_PRECISION = 1024;
inline constexpr RadixType DEFAULT_RADIX = RadixType::Decimal;
inline constexpr NumberFormatType DEFAULT_NUMBER_FORMAT = NumberFormatType::Float;
inline  NumberWidth DEFAULT_NUMBER_WIDTH = NumberWidthType::QWord;

inline constexpr char DEFAULT_DEC_SEPARATOR = '.';
inline constexpr char DEFAULT_GRP_SEPARATOR = ',';
inline constexpr const char* DEFAULT_G_STR = "3;0";
inline constexpr const char* DEFAULT_NUMBER_STR = "0";

class CalculatorEngine
{
public:
	CalculatorEngine(CalculatorMode calculator_mode, ICalculatorDisplay* calc_disp, ICalculatorHistory* calc_hist);
	Radix const& GetCurrentRadix();
	mpl::mpf const& GetMemorizedNumber();
	std::string GetStringForDisplay(mpl::mpf const& num, Radix const& radix);
	std::string GroupDigitsPerRadix(std::string_view number_string, Radix const& radix);
	std::string GetCurrentValueForRadix(Radix const& radix, bool group_digit_per_radix);
	char GetDecimalSeparator();

	void UpdateNumberString();
	void UpdateMaxIntegerDigit();

	bool IsInErrorState();
	bool IsInRecordingState();
	bool IsInputEmpty();
	void SetMemorizedNumber(mpl::mpf const& mem_rat);
	void SetRadixAndNumberWidth(Radix const& radix, NumberWidth const& number_width);

	/* 此函数输入的值为外部精度
	SetPrecision(1024) ===> MPF_ORIGINAL_PRECISION = 128 */
	void SetPrecision(int new_precision);

	void ProcessCommand(Command command);
	void ProcessErrorCommand(Command command);
	void ProcessError(uint32_t err_code);

	CalculatorMode GetCalculatorMode() const;
	double GenerateRandomNumber();
	mpl::mpf DoUnaryOperation(mpl::mpf const& rat, Command uncmd);
	mpl::mpf DoBinaryOperation(mpl::mpf const& x, mpl::mpf const& y, Command bincmd);
	mpl::mpf ToggleBit(mpl::mpf const& rat, uint32_t bit_index);
	mpl::mpf CalculateExpression(ExpressionCommands const& cmds);

	void SetPrimaryDisplay(std::string const& dis_str, bool is_err = false);

	void SetExpressionDisplay();
	void SetPrimaryDisplay();
private:
	ICalculatorDisplay* calculator_display;
	ICalculatorHistory* calculator_history;
	CalculatorMode current_calculator_mode;
	Radix current_radix;
	NumberWidth current_number_width;
	NumberFormat current_number_format;
	int current_precision;
	int current_max_int_digit;
	char group_separator;
	char decimal_separator;
	bool is_in_error_state;
	bool is_recording;

	std::vector<unsigned int> group_vector;

	std::string number_string;

	mpl::mpf current_value;
	mpl::mpf last_value;
	mpl::mpf max_trigonometric_value;
	mpl::mpf memorized_number;
	uint64_t carry_bit;
	Command last_command;
	Command hold_command;
	Command last_bin_command;
	ExpressionCollector expression_collector;
	CalculatorInput input;

	std::unique_ptr<std::mt19937> random_engine;
	std::unique_ptr<std::uniform_real_distribution<>> distr;

	double radix_ratio;
};

CM_END

#endif // !__INCLUDE_CALCULATOR_ENGINE_H__
