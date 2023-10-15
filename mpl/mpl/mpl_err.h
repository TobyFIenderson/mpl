#pragma once
#ifndef __INCLUDE_MPL_ERR_H__
#define __INCLUDE_MPL_ERR_H__

#include "mpl_ns.h"

MPL_BEGIN

typedef unsigned int mpl_err;


inline constexpr mpl_err MPL_ERR_BEGIN = 0;

/* ���볬�ޡ���Ч���� */
inline constexpr mpl_err MPL_ERR_INVALID_VAL = MPL_ERR_BEGIN;

/* ���ڴ�ľ� */
inline constexpr mpl_err MPL_ERR_OUT_OF_MEM = MPL_ERR_INVALID_VAL + 1;

/* �ﵽ���������� */
inline constexpr mpl_err MPL_ERR_MAX_ITER_REACHED = MPL_ERR_OUT_OF_MEM + 1;

/* ������̫С */
inline constexpr mpl_err MPL_ERR_BUF_OVERFLOW = MPL_ERR_MAX_ITER_REACHED + 1;

/* δ֪���� */
inline constexpr mpl_err MPL_ERR_UNKNOWN_ERR = MPL_ERR_BUF_OVERFLOW + 1;

/* ���δ���� */
inline constexpr mpl_err MPL_ERR_RESULT_UNDEF = MPL_ERR_UNKNOWN_ERR + 1;

/* ������� */
inline constexpr mpl_err MPL_ERR_DIV_BY_ZERO = MPL_ERR_RESULT_UNDEF + 1;

/* ���������� */
inline constexpr mpl_err MPL_ERR_RESULT_INF = MPL_ERR_DIV_BY_ZERO + 1;

inline constexpr mpl_err MPL_ERR_NULL = MPL_ERR_RESULT_INF + 1;

/* �����������Ҫ���ڱ���ʹ�ã������� */
inline constexpr mpl_err MPL_ERR_END = MPL_ERR_NULL;
inline constexpr mpl_err MPL_ERR_SIZE = MPL_ERR_END - MPL_ERR_BEGIN + 1;

MPL_END

#endif // !__INCLUDE_MPL_ERR_H__