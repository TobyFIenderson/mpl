#include "CalculatorManager.h"
using namespace std;
using namespace mpl;
CM_BEGIN

CalculatorManager::CalculatorManager(ICalculatorDisplay* display_callback, IResourceProvider* o_resource_provider) :
	calculator_display(display_callback),
	resource_provider(o_resource_provider),
	current_calculator_engine(nullptr),
	calculator_engines{ nullptr,nullptr,nullptr },
	current_calculator_history(nullptr),
	calculator_historys{ nullptr,nullptr,nullptr },
	calculator_memory(), // nullptr
	angle_type(DEFAULT_ANGLE_TYPE),
	shift_type(DEFAULT_SHIFT_TYPE),
	is_invert(false),
	is_hyp(false),
	ishistory_item_load_mode(false)
{}


CalculatorManager::~CalculatorManager()
{
	for (int i = 0; i < calculator_engines.size(); i++)
	{
		delete calculator_engines[i];
		calculator_engines[i] = nullptr;
	}

	for (int i = 0; i < calculator_historys.size(); i++)
	{
		delete calculator_historys[i];
		calculator_historys[i] = nullptr;
	}

	current_calculator_engine = nullptr;
	current_calculator_history = nullptr;
}

void CalculatorManager::SetCurrentCalculatorEngineRadix(RadixType radix_type)
{
	switch (radix_type)
	{
	case RadixType::Binary:
	case RadixType::Octal:
	case RadixType::Decimal:
	case RadixType::Hexadecimal:
		current_calculator_engine->ProcessCommand(BIN + (static_cast<unsigned int>(radix_type) - static_cast<unsigned int>(RadixType::Binary)));
		break;
	case RadixType::Null:
	default:
		break;
	}
}

void CalculatorManager::SetCurrentCalculatorEngineDisplayStringFormat(NumberFormat number_format)
{
	switch (num_format(number_format))
	{
	case NumberFormat::Float:
	case NumberFormat::Scientific:
	case NumberFormat::Engineering:
		current_calculator_engine->ProcessCommand(FLOFMT + (static_cast<int>(number_format) - static_cast<unsigned int>(NumberFormat::Float)));
		break;
	}
}

void CalculatorManager::Reset(bool clear_memory)
{
	ProcessCommand(STDMODE);

	CalculatorEngine* calc_eng = calculator_engines[static_cast<size_t>(CalculatorMode::Scientific)];
	if (calc_eng != nullptr)
	{
		calc_eng->ProcessCommand(DEG);
		calc_eng->ProcessCommand(CLEAR);
		calc_eng->ProcessCommand(FLOFMT);
	}

	calc_eng = calculator_engines[static_cast<size_t>(CalculatorMode::Programmer)];
	if (calc_eng != nullptr)
	{
		calc_eng->ProcessCommand(CLEAR);
	}

	if (clear_memory)
	{
		calculator_memory.MemorizedNumberClearAll();
	}
}


void CalculatorManager::SetCurrentCalculatorEngineNumberWidth(NumberWidthType number_width_type)
{
	switch (number_width_type)
	{
	case NumberWidthType::Byte:
	case NumberWidthType::Word:
	case NumberWidthType::DWord:
	case NumberWidthType::QWord:
		current_calculator_engine->ProcessCommand(BYTE + (static_cast<unsigned int>(number_width_type) - static_cast<unsigned int>(NumberWidthType::Byte)));
		break;
	case NumberWidthType::Null:
	default:
		break;
	}
}

void CalculatorManager::SetCurrentCalculatorEngineMode(CalculatorMode calculator_mode)
{
	switch (calculator_mode)
	{
	case CalculatorMode::Standard:
	case CalculatorMode::Scientific:
	case CalculatorMode::Programmer:
		ProcessCommand(STDMODE + (static_cast<unsigned int>(calculator_mode) - static_cast<unsigned int>(CalculatorMode::Standard)));
		break;
	case CalculatorMode::Statistical: /* 暂未实现统计计算器 */
	case CalculatorMode::Null:
	default:
		break;
	}
}

void CalculatorManager::SetCurrentCalculatorEnginePrecision(int new_precision)
{
	current_calculator_engine->SetPrecision(new_precision);
}

void CalculatorManager::UpdateCurrentCalculatorEngineMaxIntegerDigit()
{
	current_calculator_engine->UpdateMaxIntegerDigit();
}

char CalculatorManager::GetCurrentCalcualtorEngineDecimalSeparator()
{
	return current_calculator_engine->GetDecimalSeparator();
}

string CalculatorManager::GetCurrentCalculatorEngineCalcualteResultForRadix(Radix const& radix, bool group_digit)
{
	return current_calculator_engine->GetCurrentValueForRadix(radix, group_digit);
}

bool CalculatorManager::IsCurrentCalculatorEngineRecording()
{
	return current_calculator_engine->IsInRecordingState();
}

bool CalculatorManager::isCurrentCalculatorEngineInputEmpty()
{
	return current_calculator_engine->IsInputEmpty();
}

void CalculatorManager::MemorizeNumber()
{
	calculator_memory.MemorizeNumber();
}

void CalculatorManager::MemorizedNumberLoad(unsigned int idx_of_mem)
{
	calculator_memory.MemorizedNumberRecall(idx_of_mem);
}

void CalculatorManager::MemorizedNumberClear(unsigned int idx_of_mem)
{
	calculator_memory.MemorizedNumberClear(idx_of_mem);
}

void CalculatorManager::MemorizedNumberPlus(unsigned int idx_of_mem)
{
	calculator_memory.MemorizedNumberPlus(idx_of_mem);
}

void CalculatorManager::MemorizedNumberMinus(unsigned int idx_of_mem)
{
	calculator_memory.MemorizedNumberMinus(idx_of_mem);
}

void CalculatorManager::MemorizedNumberClearAll()
{
	calculator_memory.MemorizedNumberClearAll();
}

vector<shared_ptr<HistoryItem>> const& CalculatorManager::GetCurrentHistoryItems()
{
	return current_calculator_history->GetHistory();
}

vector<shared_ptr<HistoryItem>> const& CalculatorManager::GetCurrentHistoryItems(CalculatorMode calculator_mode)
{
	switch (calculator_mode)
	{
	case CalculatorMode::Standard:
	case CalculatorMode::Scientific:
	case CalculatorMode::Programmer:
		return calculator_historys[static_cast<size_t>(calculator_mode)]->GetHistory();
		break;
	case CalculatorMode::Statistical:
	case CalculatorMode::Null:
	default:
		return current_calculator_history->GetHistory();
		break;
	}
}

shared_ptr<HistoryItem> const& CalculatorManager::GetCurrentHistoryItem(unsigned int idx_of_his)
{
	return current_calculator_history->GetHistoryItem(idx_of_his);
}

bool CalculatorManager::RemoveCurrentHistoryItem(unsigned int idx_of_his)
{
	return current_calculator_history->RemoveItem(idx_of_his);
}

void CalculatorManager::ClearCurrentHistory()
{
	current_calculator_history->Clear();
}

size_t CalculatorManager::GetCurrentHistoryMaxSize()
{
	return current_calculator_history->MaxHistorySize();
}

void CalculatorManager::SetIsInHistoryItemLoadModeFlag(bool ishis_item_load_mode)
{
	ishistory_item_load_mode = ishis_item_load_mode;
}

AngleType CalculatorManager::GetCurrentAngleType()
{
	return angle_type;
}

void CalculatorManager::SetCurrentCalculatorEngineMemorizedNumber(mpf const& rat)
{
	current_calculator_engine->SetMemorizedNumber(rat);
}

void CalculatorManager::ProcessCommand(Command command)
{
	switch (command)
	{
	case DEG:
	case RAD:
	case GRAD:
		angle_type = AngleType(command - DEG);
		break;
	case STDMODE:  // 每次切换计算器，都会清除其状态，但会保留历史记录和内存。
	case SCIMODE:
	case PROMODE:
	{
		unsigned int idx = command - STDMODE;
		if (calculator_engines[idx] == nullptr)
		{
			calculator_engines[idx] = new CalculatorEngine(CalculatorMode(idx), calculator_display, calculator_historys[idx]);
		}

		current_calculator_engine = calculator_engines[idx];
		current_calculator_engine->ProcessCommand(DEC);
		current_calculator_engine->ProcessCommand(CLEAR);
		current_calculator_engine->SetPrecision(CALCULATOR_PRECISION[idx]);
		current_calculator_engine->UpdateMaxIntegerDigit();

		if (calculator_historys[idx] == nullptr)
		{
			calculator_historys[idx] = new CalculatorHistory(DEFAULT_HISTORY_MAX_SIZE);
		}
		current_calculator_history = calculator_historys[idx];

		calculator_memory.SetCalculatorEngine(current_calculator_engine);
	}
	break;
	case INVC:
		is_invert = !is_invert;
		break;
	case HYP:
		is_hyp = !is_hyp;
		break;
	case SIN:
	case COS:
	case TAN:
	case SEC:
	case CSC:
	case COT:
	{
		Command trigonometric_command = GetTrigonometricFunctionCommand(TrigonometricFunction(command - SIN), angle_type, is_invert, is_hyp);
		current_calculator_engine->ProcessCommand(trigonometric_command);
	}
	break;
	case SHA:
	case SHL:
	case ROSH:
	case RCSH:
		shift_type = ShiftType(command - SHA);
		break;
	case LSH:
	case RSH:
	{
		Command shift_command = GetShiftCommand(shift_type, command);
		current_calculator_engine->ProcessCommand(shift_command);
	}
	break;
	default:
		current_calculator_engine->ProcessCommand((is_invert ? GetInvertCommand(command) : command));
		break;
	}
}

CM_END
