#pragma once
#ifndef __INCLUDE_GROUP_DIGITS_UTILS_H__
#define __INCLUDE_GROUP_DIGITS_UTILS_H__

#include "CalculatorManagerLibNS.h"
#include "Radix.h"
#include <string>
#include <string_view>
#include <vector>
#include "CalculatorError.h"

CM_BEGIN

ErrorCode IsNumberInvalid(std::string& num_str, size_t max_exp_len, size_t max_mant_len, Radix const& radix, char dec_pt);

ErrorCode IsGroupStringInvalid(std::string_view grouping_string);

std::vector<uint32_t> GetGroupVectorFromString(std::string_view grouping_string);

std::string GroupDigits(std::string_view display_strings, std::vector<uint32_t> const& grouping_vector, char delimiter, char dec_pt, bool is_negative);

CM_END

#endif // !__INCLUDE_GROUP_DIGITS_UTILS_H__