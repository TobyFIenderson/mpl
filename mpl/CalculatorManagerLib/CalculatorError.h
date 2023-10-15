#pragma once
#ifndef __INCLUDE_CALCULATOR_ERROR_H__
#define __INCLUDE_CALCULATOR_ERROR_H__

#include "CalculatorManagerLibNS.h"
#include "../mpl/mpl_err.h"
CM_BEGIN

// ���еĴ����붼�� [CALC_ERR_BEGIN, CALC_ERR_END] ֮��

using ErrorCode = MPL mpl_err;

#define ERRCODE inline constexpr ErrorCode

ERRCODE CALC_ERR_BEGIN = MPL  MPL_ERR_BEGIN;
// ���� MPL_ERR ֮��
ERRCODE CE_ERR_BEGIN = MPL MPL_ERR_END + 1;
// ���볬�ޡ�
ERRCODE CE_ERR_INPUT_OVERFLOW = CE_ERR_BEGIN;

// ������ޡ�
ERRCODE CE_ERR_OUTPUT_OVERFLOW = CE_ERR_INPUT_OVERFLOW + 1;

// δ֪�����ֱ�
ERRCODE CE_ERR_UNKNOW_NUM_REPRESENTATION = CE_ERR_OUTPUT_OVERFLOW + 1;

// ����Ĳ�������
ERRCODE CE_ERR_WRONG_OPERATOR_CODE = CE_ERR_UNKNOW_NUM_REPRESENTATION + 1;

// û�д���
ERRCODE CE_ERR_NULL = CE_ERR_WRONG_OPERATOR_CODE + 1;

ERRCODE CE_ERR_END = CE_ERR_NULL;
ERRCODE CE_ERR_SIZE = CE_ERR_END - CE_ERR_BEGIN + 1;

//-------------------------------------------------------------------------------------------------------------\\

ERRCODE GS_ERR_BEGIN = CE_ERR_END + 1; // ���� CalculatorEngineError ֮��

// ̫��ķֺš�
ERRCODE GS_ERR_TOO_MANY_SC = GS_ERR_BEGIN;

// ���ڷǷ��ַ���
ERRCODE GS_ERR_ILLEGAL_CH_EXIST = GS_ERR_TOO_MANY_SC + 1;

// ̫��������� 0 �ˡ�
ERRCODE GS_ERR_TOO_MANY_ZERO = GS_ERR_ILLEGAL_CH_EXIST + 1;

// 0 ����ӵĲ��� 0 ��
ERRCODE GS_ERR_NOT_ZERO_AFTER_ZERO = GS_ERR_TOO_MANY_ZERO + 1;

// û�д���
ERRCODE GS_ERR_NULL = GS_ERR_NOT_ZERO_AFTER_ZERO + 1;

ERRCODE GS_ERR_END = GS_ERR_NULL;
ERRCODE GS_ERR_SIZE = GS_ERR_END - GS_ERR_BEGIN + 1;

//-------------------------------------------------------------------------------------------------------------\\

ERRCODE CALC_ERR_END = GS_ERR_END;

#undef ERRCODE

inline constexpr bool ErrorCodeIsNull(ErrorCode e)
{
	return e == MPL MPL_ERR_NULL || e == CE_ERR_NULL || e == GS_ERR_NULL;
}

CM_END

#endif // !__INCLUDE_CALCULATOR_ERROR_H__
