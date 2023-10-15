#include "CalculatorEngine.h"
CM_BEGIN

CalculatorEngine::CalculatorEngine(CalculatorMode calculator_mode, ICalculatorDisplay* calc_disp, ICalculatorHistory* calc_hist) :
	calculator_display(calc_disp),
	calculator_history(calc_hist),
	carry_bit(0),
	current_calculator_mode(calculator_mode),
	current_max_int_digit(DEFAULT_MAX_DIGITS),
	current_number_format(DEFAULT_NUMBER_FORMAT),
	current_precision(DEFAULT_PRECISION),
	current_value(0),
	current_radix(DEFAULT_RADIX),
	current_number_width(DEFAULT_NUMBER_WIDTH),
	last_value(0),
	last_bin_command(CMD_NULL),
	decimal_separator(DEFAULT_DEC_SEPARATOR),
	group_separator(DEFAULT_GRP_SEPARATOR),
	is_in_error_state(false),
	is_recording(false),
	last_command(CMD_NULL),
	hold_command(CMD_NULL),
	expression_collector(calculator_display, calculator_history, DEFAULT_DEC_SEPARATOR, calculator_mode),
	input(DEFAULT_DEC_SEPARATOR, DEFAULT_RADIX, DEFAULT_NUMBER_WIDTH, calculator_mode, DEFAULT_PRECISION),
	radix_ratio(log(MPL MPI_BASE) / log(int(Radix(DEFAULT_RADIX))))
{}

bool CalculatorEngine::IsInRecordingState()
{
	return is_recording;
}

bool CalculatorEngine::IsInputEmpty()
{
	return input.IsEmpty() && (number_string.empty() || number_string == "0");
}

CM_END