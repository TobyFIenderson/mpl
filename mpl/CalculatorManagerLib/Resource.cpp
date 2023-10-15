#include "Resource.h"
#include <string_view>
#include "ResourceStringID.h"
using namespace std;
CM_BEGIN

bool Resource::is_inited = false;

unordered_map< string_view, string> Resource::ENGINE_RES_STR;

unordered_map<Command, string_view> Resource::CMD_STR_ID
{
	{NUM_0,RS_NUM_0},
	{NUM_1,RS_NUM_1},
	{NUM_2,RS_NUM_2},
	{NUM_3,RS_NUM_3},
	{NUM_4,RS_NUM_4},
	{NUM_5,RS_NUM_5},
	{NUM_6,RS_NUM_6},
	{NUM_7,RS_NUM_7},
	{NUM_8,RS_NUM_8},
	{NUM_9,RS_NUM_9},
	{NUM_A,RS_NUM_A},
	{NUM_B,RS_NUM_B},
	{NUM_C,RS_NUM_C},
	{NUM_D,RS_NUM_D},
	{NUM_E,RS_NUM_E},
	{NUM_F,RS_NUM_F},
	{ADD,RS_ADD},
	{SUB,RS_SUB},
	{MUL,RS_MUL},
	{DIV,RS_DIV},
	{REM,RS_REM},
	{MOD,RS_MOD},
	{LSHA,RS_LSHA},
	{LSHL,RS_LSHL},
	{RSHA,RS_RSHA},
	{RSHL,RS_RSHL},
	{AND,RS_AND},
	{OR,RS_OR},
	{XOR,RS_XOR},
	{NAND,RS_NAND},
	{NOR,RS_NOR},
	{PWR,RS_PWR},
	{YROOT,RS_YROOT},
	{LOGBASEY,RS_LOGBASEY},
	{ROR,RS_ROR},
	{ROL,RS_ROL},
	{RORC,RS_RORC},
	{ROLC,RS_ROLC},
	{NOT,RS_NOT},
	{DMS,RS_DMS},
	{DEGREE,RS_DEGREE},
	{FRAC,RS_FRAC},
	{INT,RS_INT},
	{SIN_DEG,RS_SIN_DEG},
	{SIN_RAD,RS_SIN_RAD},
	{SIN_GRAD,RS_SIN_GRAD},
	{COS_DEG,RS_COS_DEG},
	{COS_RAD,RS_COS_RAD},
	{COS_GRAD,RS_COS_GRAD},
	{TAN_DEG,RS_TAN_DEG},
	{TAN_RAD,RS_TAN_RAD},
	{TAN_GRAD,RS_TAN_GRAD},
	{SINH,RS_SINH},
	{COSH,RS_COSH},
	{TANH,RS_TANH},
	{ARCSIN_DEG,RS_ARCSIN_DEG},
	{ARCSIN_RAD,RS_ARCSIN_RAD},
	{ARCSIN_GRAD,RS_ARCSIN_GRAD},
	{ARCCOS_DEG,RS_ARCCOS_DEG},
	{ARCCOS_RAD,RS_ARCCOS_RAD},
	{ARCCOS_GRAD,RS_ARCCOS_GRAD},
	{ARCTAN_DEG,RS_ARCTAN_DEG},
	{ARCTAN_RAD,RS_ARCTAN_RAD},
	{ARCTAN_GRAD,RS_ARCTAN_GRAD},
	{ARSINH,RS_ARSINH},
	{ARCOSH,RS_ARCOSH},
	{ARTANH,RS_ARTANH},
	{SEC_DEG,RS_SEC_DEG},
	{SEC_RAD,RS_SEC_RAD},
	{SEC_GRAD,RS_SEC_GRAD},
	{CSC_DEG,RS_CSC_DEG},
	{CSC_RAD,RS_CSC_RAD},
	{CSC_GRAD,RS_CSC_GRAD},
	{COT_DEG,RS_COT_DEG},
	{COT_RAD,RS_COT_RAD},
	{COT_GRAD,RS_COT_GRAD},
	{SECH,RS_SECH},
	{CSCH,RS_CSCH},
	{COTH,RS_COTH},
	{ARCSEC_DEG,RS_ARCSEC_DEG},
	{ARCSEC_RAD,RS_ARCSEC_RAD},
	{ARCSEC_GRAD,RS_ARCSEC_GRAD},
	{ARCCSC_DEG,RS_ARCCSC_DEG},
	{ARCCSC_RAD,RS_ARCCSC_RAD},
	{ARCCSC_GRAD,RS_ARCCSC_GRAD},
	{ARCCOT_DEG,RS_ARCCOT_DEG},
	{ARCCOT_RAD,RS_ARCCOT_RAD},
	{ARCCOT_GRAD,RS_ARCCOT_GRAD},
	{ARSECH,RS_ARSECH},
	{ARCSCH,RS_ARCSCH},
	{ARCOTH,RS_ARCOTH},
	{LN,RS_LN},
	{LOG,RS_LOG},
	{INV,RS_INV},
	{SQU,RS_SQU},
	{SQRT,RS_SQRT},
	{CUB,RS_CUB},
	{CUBT,RS_CUBT},
	{PWR10,RS_PWR10},
	{PWR2,RS_PWR2},
	{PWRE,RS_PWRE},
	{PERCENT,RS_PERCENT},
	{ABS,RS_ABS},
	{FLOOR,RS_FLOOR},
	{CEIL,RS_CEIL},
	{NEG,RS_NEG},
	{EXP,RS_EXP},
	{LSH,RS_LSH},
	{RSH,RS_RSH},
	{SHA,RS_SHA},
	{SHL,RS_SHL},
	{ROSH,RS_ROSH},
	{RCSH,RS_RCSH},
	{CLEAR,RS_CLEAR},
	{CENTR,RS_CENTR},
	{BACK,RS_BACK},
	{OPENP,RS_OPENP},
	{CLOSEP,RS_CLOSEP},
	{EQU,RS_EQU},
	{PNT,RS_PNT},
	{SIGN,RS_SIGN},
	{MSTORE,RS_MSTORE},
	{MRECALL,RS_MRECALL},
	{MPLUS,RS_MPLUS},
	{MMINUS,RS_MMINUS},
	{MCLEAR,RS_MCLEAR},
	{BIN,RS_BIN},
	{OCT,RS_OCT},
	{DEC,RS_DEC},
	{HEX,RS_HEX},
	{BYTE,RS_BYTE},
	{WORD,RS_WORD},
	{DWORD,RS_DWORD},
	{QWORD,RS_QWORD},
	{DEG,RS_DEG},
	{RAD,RS_RAD},
	{GRAD,RS_GRAD},
	{FLOFMT,RS_FLOFMT},
	{SCIFMT,RS_SCIFMT},
	{ENGFMT,RS_ENGFMT},
	{STDMODE,RS_STDMODE},
	{SCIMODE,RS_SCIMODE},
	{PROMODE,RS_PROMODE},
	{INVC,RS_INVC},
	{HYP,RS_HYP},
	{SIN,RS_SIN},
	{COS,RS_COS},
	{TAN,RS_TAN},
	{SEC,RS_SEC},
	{CSC,RS_CSC},
	{COT,RS_COT},
	{PI,RS_PI},
	{EULER,RS_EULER},
	{RAND,RS_RAND},
	{SET_RET,RS_SET_RET},
	{CMD_NULL,RS_CMD_NULL}
};

unordered_map<ErrorCode, string_view> Resource::ERR_STR_ID =
{
	{MPL MPL_ERR_INVALID_VAL			, RS_MPL_ERR_INVALID_VAL},
	{MPL MPL_ERR_OUT_OF_MEM				, RS_MPL_ERR_OUT_OF_MEM},
	{MPL MPL_ERR_MAX_ITER_REACHED		, RS_MPL_ERR_MAX_ITER_REACHED},
	{MPL MPL_ERR_BUF_OVERFLOW			, RS_MPL_ERR_BUF_OVERFLOW},
	{MPL MPL_ERR_UNKNOWN_ERR			, RS_MPL_ERR_UNKNOWN_ERR},
	{MPL MPL_ERR_RESULT_UNDEF			, RS_MPL_ERR_RESULT_UNDEF},
	{MPL MPL_ERR_DIV_BY_ZERO			, RS_MPL_ERR_DIV_BY_ZERO},
	{MPL MPL_ERR_RESULT_INF				, RS_MPL_ERR_RESULT_INF},
	{MPL MPL_ERR_NULL					, RS_MPL_ERR_NULL},
	{CE_ERR_INPUT_OVERFLOW				, RS_CE_ERR_INPUT_OVERFLOW},
	{CE_ERR_OUTPUT_OVERFLOW				, RS_CE_ERR_OUTPUT_OVERFLOW},
	{CE_ERR_UNKNOW_NUM_REPRESENTATION	, RS_CE_ERR_UNKNOW_NUM_REPRESENTATION},
	{CE_ERR_WRONG_OPERATOR_CODE			, RS_CE_ERR_WRONG_OPERATOR_CODE},
	{CE_ERR_NULL						, RS_CE_ERR_NULL},
	{GS_ERR_TOO_MANY_SC					, RS_GS_ERR_TOO_MANY_SC},
	{GS_ERR_ILLEGAL_CH_EXIST			, RS_GS_ERR_ILLEGAL_CH_EXIST},
	{GS_ERR_TOO_MANY_ZERO				, RS_GS_ERR_TOO_MANY_ZERO},
	{GS_ERR_NOT_ZERO_AFTER_ZERO			, RS_GS_ERR_NOT_ZERO_AFTER_ZERO},
	{GS_ERR_NULL						, RS_GS_ERR_NULL}
};

string_view Resource::GetCommandStringIDFromCommand(Command cmd)
{
	return CMD_STR_ID[cmd];
}

string_view Resource::GetErrorStringIDFromErrorCode(ErrorCode ret_code)
{
	return ERR_STR_ID[ret_code];
}

void Resource::LoadRresource(IResourceProvider* res_provider)
{
	for (pair<Command, string_view> const& p : CMD_STR_ID)
	{
		string str = res_provider->GetResourceString(p.second);
		if (!str.empty())
		{
			ENGINE_RES_STR[p.second] = str;
		}
	}

	for (pair<ErrorCode, string_view> const& p : ERR_STR_ID)
	{
		string str = res_provider->GetResourceString(p.second);
		if (!str.empty())
		{
			ENGINE_RES_STR[p.second] = str;
		}
	}

	is_inited = true;
}

string Resource::GetString(string_view str_id)
{
	return ENGINE_RES_STR[str_id];
}

string Resource::GetStringFromCommand(Command cmd)
{
	return GetString(GetCommandStringIDFromCommand(cmd));
}

string Resource::GetStringFromErrorCode(ErrorCode ret_code)
{
	return GetString(GetErrorStringIDFromErrorCode(ret_code));
}

bool Resource::AlreadyLoadedRresource()
{
	return is_inited;
}

void Resource::AlreadyLoadedRresource(bool flag)
{
	is_inited = flag;
}

CM_END