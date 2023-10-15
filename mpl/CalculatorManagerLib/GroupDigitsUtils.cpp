#include "GroupDigitsUtils.h"
#include <regex>
using namespace std;
CM_BEGIN

// 每分组数字的最长长度。
constexpr uint32_t MAX_GROUPING_SIZE = 16; constexpr size_t MAX_EXPONENT = 4;
constexpr string_view DEC_PRE_SEP_STR = "[+-]?(\\d*)[";
constexpr string_view DEC_POS_SEP_STR = "]?(\\d*)(?:e[+-]?(\\d*))?$";

ErrorCode IsNumberInvalid(string& num_str, size_t max_exp_len, size_t max_mant_len, Radix const& radix, char dec_pt)
{
	ErrorCode err = GS_ERR_NULL;

	// 十进制下：
	if (radix == 10)
	{
		// 以可选的 + 或 - 号开始
		// 后接 0 或者其他数字
		// 后接可选的小数点
		// 后接 0 或者其他数字
		// 后接可选的指数符号 e 
		// 如果存在指数符号 e 
		//		后接可选的 + 或 - 号
		//		后接 0 或者其他数字
		regex rx{ string(DEC_PRE_SEP_STR) + dec_pt + string(DEC_POS_SEP_STR) };
		smatch matches;
		if (regex_match(num_str, matches, rx))
		{
			// 看看是否指数部分太长
			if (matches.length(3) > static_cast<ptrdiff_t>(max_exp_len))
			{
				err = CE_ERR_INPUT_OVERFLOW;
			}
			else
			{
				string exp = matches.str(1);
				string::iterator exp_itr = exp.begin();
				string::iterator exp_end = exp.end();
				while (exp_itr != exp_end && *exp_itr == L'0')
				{
					exp_itr++;
				}

				long long mant_len = distance(exp_itr, exp_end) + matches.length(2);
				if (mant_len > static_cast<ptrdiff_t>(max_mant_len))
				{
					err = CE_ERR_INPUT_OVERFLOW;
				}
			}
		}
		else
		{
			err = CE_ERR_UNKNOW_NUM_REPRESENTATION;
		}
	}
	else
	{
		for (char const& wc : num_str)
		{
			if (radix == 16)
			{
				if (!(iswdigit(wc) || (wc >= L'A' && wc <= L'F')))
				{
					err = CE_ERR_UNKNOW_NUM_REPRESENTATION;
				}
			}
			else if (wc < L'0' || wc >= L'0' + radix)
			{
				err = CE_ERR_UNKNOW_NUM_REPRESENTATION;
			}
		}
	}
	return err;
}

/****************************************************************************\
*
* IsGroupingStringInvalid
*
* 描述：
*   这个函数将分析用户自定义的数字分组字符串是否合法。
*
*	根据微软的定义，GroupingString 应满足以下语法：
*	由数字开头，且由数字结尾；
*	每两个数字之间由半角分号 ; 隔开；
*	0 只能出现在末尾或者 0 的后面必须是 0 ；
*	0 最多只能叠加两次，表示分组结束；
*	数字只能是十进制数；
*	除了数字和半角分号，不能出现其他字符。
*
* 例如：
*	a
*	0
*	a;b;c
*	a;0
*	a;0;0;
* 不能是：
*	a;0;b
*	a;0;0;b
*	a;0;0;0;0
*
* GroupingString 的具体含义：
*   0;0      - no grouping
*   3;0      - group every 3 digits
*   3        - group 1st 3, then no grouping after
*   3;0;0    - group 1st 3, then no grouping after
*   3;2;0    - group 1st 3 and then every 2 digits
*   4;0      - group every 4 digits
*   5;3;2;0  - group 5, then 3, then every 2
*   5;3;2    - group 5, then 3, then 2, then no grouping after
*
*	在 GroupingString 中一个 0 代表重复上一次的分组方式，单独的一个 0 代表不进行分组。
*	而连续的两个 0 （0;0）将被理解为一直重复长度为 0 的分组方式，也是不进行分组了。
*
* 返回值：由 GroupingString 解析得来的 vector 。
*
\****************************************************************************/

//-----------------------------------------------------------------------------
//
//	SATRT	^3;0			Start
//	FZR		0^;0 or 3;0^	First number or first after semicolon number is zero
//	FNZ		3^;0 or 3;3^	First number or first after semicolon number not zero
//	ZRAZR	00^;00			Zero after zero
//	NZAZR	03^;00			Not zero after zero
//	ZRANZ	30^;00			Zero after not zero
//	NZANZ	33^;00			Not zero after not zero
//	SCAZR	03;^00			Semicolon after zero
//	SCANZ	30;^00			Semicolon after not zero
//
//	ZR		Zero
//	NZ		Not Zero
//	SC		Semicolon
//	ERR		Error
//
//-----------------------------------------------------------------------------


static constexpr int ZR = 0, NZ = 1, SC = 2;
static constexpr int START = 0, FZR = 1, FNZ = 2, ZRAZR = 3, NZAZR = 4, ZRANZ = 5, NZANZ = 6, SCAZR = 7, SCANZ = 8, ERR = 9;

static int state_machine[10][3] =
{			//	zr		nz		sc
	/*start	*/{	FZR,	FNZ,	ERR		},
	/*fzr	*/{	ZRAZR,	NZAZR,	SCAZR	},
	/*fnz	*/{	ZRANZ,	NZANZ,	SCANZ	},
	/*zrazr	*/{	ZRAZR,	NZAZR,	SCAZR	},
	/*nzazr	*/{	ZRANZ,	NZANZ,	SCANZ	},
	/*zranz	*/{	ZRAZR,	NZAZR,	SCANZ	},
	/*nzanz	*/{	ZRANZ,	NZANZ,	SCANZ	},
	/*scazr	*/{	FZR,	FNZ,	ERR		},
	/*scanz	*/{	FZR,	FNZ,	ERR		},
	/*err	*/{	ERR,	ERR,	ERR		}
};

ErrorCode IsGroupStringInvalid(string_view grouping_string)
{
	bool have_one_zero = false, have_two_zero = false;
	bool have_nz = false;
	int state = START;
	for (auto c : grouping_string)
	{
		if (c == L';')
		{
			state = state_machine[state][SC];
		}
		else if (c >= L'0' && c <= L'9')
		{
			state = c == L'0' ? state_machine[state][ZR] : state_machine[state][NZ];
		}
		else
		{
			return GS_ERR_ILLEGAL_CH_EXIST;
		}

		switch (state)
		{
		case FNZ:
		case NZAZR:
		case ZRANZ:
		case NZANZ:
		case SCANZ:
		{
			if (have_one_zero)
			{
				return GS_ERR_NOT_ZERO_AFTER_ZERO;
			}
			else if (have_two_zero)
			{
				return GS_ERR_TOO_MANY_ZERO;
			}
			have_nz = state != SCANZ;
		}
		break;
		case FZR:
		case SCAZR:
		{
			if (have_two_zero)
			{
				return GS_ERR_TOO_MANY_ZERO;
			}

			if (state == FZR)
			{
				have_nz = false;
			}
			else
			{
				have_two_zero = have_one_zero && !have_nz;
				have_one_zero = !have_nz;
			}
		}
		break;
		case ERR: return GS_ERR_TOO_MANY_SC;
		default:
			break;
		}
	}

	return state != SCANZ && state != SCAZR ? GS_ERR_NULL : GS_ERR_TOO_MANY_SC;
}

vector<uint32_t> GetGroupVectorFromString(string_view grouping_string)
{
	uint32_t grouping_value = grouping_string[0] - L'0';
	vector<uint32_t> grouping_vector;
	auto it = grouping_string.begin();
	while (it != grouping_string.end())
	{
		++it;
		while (it != grouping_string.end() && *it != L';')
		{
			grouping_value = grouping_value * 10 + *it++ - L'0';
		}
		grouping_vector.emplace_back(grouping_value <= MAX_GROUPING_SIZE ? grouping_value : MAX_GROUPING_SIZE);
		grouping_value = 0;
	}

	return grouping_vector;
}

string GroupDigits(string_view display_strings, vector<uint32_t> const& grouping_vector, char delimiter, char dec_pt, bool is_negative)
{
	if (grouping_vector.empty())
	{
		return string{ display_strings };
	}

	size_t exp = display_strings.find(L'e');
	bool has_exp = (exp != string_view::npos);

	size_t dec = display_strings.find(dec_pt);
	bool has_dec = (dec != string_view::npos);

	reverse_iterator<string_view::const_iterator> ritr = display_strings.crend();
	// 指数和小数点同时出现的情况下，小数点一定是比指数更靠前的。
	// 4.6886e+564
	if (has_dec)
	{
		ritr -= dec;
	}
	else if (has_exp)
	{
		ritr -= exp;
	}
	else
	{
		ritr = display_strings.crbegin();
	}

	string result{};
	uint32_t grouping_size = 0;

	vector<uint32_t, allocator<uint32_t>>::const_iterator group_itr = grouping_vector.cbegin();
	uint32_t current_group = *group_itr;
	// 符号位不参与分组
	reverse_iterator<string_view::const_iterator> reverse_end = display_strings.crend() - (is_negative ? 1 : 0);
	while (ritr != reverse_end)
	{
		result += *ritr++;
		grouping_size++;

		// 如果分组完成，追加分组符号，
		// 但是以下两种情况不要追加：
		// 1. 分组长度为 0 
		// 2. 到达字符串末尾
		if (current_group != 0 && (grouping_size % current_group) == 0 && ritr != reverse_end)
		{
			result += delimiter;
			grouping_size = 0;

			// 获取新的分组。
			if (group_itr != grouping_vector.end())
			{
				while (++group_itr != grouping_vector.end())
				{
					if (*group_itr != 0)
					{
						current_group = *group_itr;
						break;
					}

					// 当前分组方式为 0 获取上一种分组方式 
					current_group = *(group_itr - 1);
				}
			}
		}
	}

	if (is_negative)
	{
		result += display_strings[0];
	}
	reverse(result.begin(), result.end());

	if (has_dec)
	{
		result += display_strings.substr(dec);
	}
	else if (has_exp)
	{
		result += display_strings.substr(exp);
	}
	return move(result);
}

CM_END
