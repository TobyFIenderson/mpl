#include "mpf.h"
#include <list>
MPL_BEGIN

//-----------------------------------------------------------------------------
// 
//    START      ��ʼ					^1.0
//    MANTS      β������				-^1.0
//    LZ         ǰ����					0^1.0
//    LZDP       С����֮ǰ��ǰ����		000.^1
//    LD         ǰ��λ					1^.0
//    DZ         LZDP֮�����			000.0^1
//    DD         С����֮���λ��		.01^2
//    DDP        ǰ��λС����			1.^2
//    EXPB       ָ����ʼ				1.0e^2
//    EXPS       ָ������				1.0e+^5
//    EXPD       ָ��λ��				1.0e1^2 ���� 1.0e0^1
//    EXPBZ      ��֮���ָ��			0.000e^+1
//    EXPSZ      ��֮���ָ������       0.000e+^1
//    EXPDZ      ��֮��ָ��λ��			0.000e+1^2
//    ERR        ��������				0.0.^
//
//    ָ��������
//
//    DP         '.'
//    ZR         '0'
//    NZ         '1'..'9' 'A'..'F'
//    SG         '+' '-'
//    EX         'e' '^' eΪʮ����ʹ�ã�^Ϊ�������
//
//-----------------------------------------------------------------------------
static constexpr uint8_t DP = 0;
static constexpr uint8_t ZR = 1;
static constexpr uint8_t NZ = 2;
static constexpr uint8_t SG = 3;
static constexpr uint8_t EX = 4;

static constexpr uint8_t START = 0;
static constexpr uint8_t MANTS = 1; /* mantissa sign */
static constexpr uint8_t LZ = 2;
static constexpr uint8_t LZDP = 3;
static constexpr uint8_t LD = 4;    /* leading digit */
static constexpr uint8_t DZ = 5;
static constexpr uint8_t DD = 6;
static constexpr uint8_t DDP = 7;
static constexpr uint8_t EXPB = 8;
static constexpr uint8_t EXPS = 9;   /* exponent sign */
static constexpr uint8_t EXPD = 10;  /* exponent digit */
static constexpr uint8_t EXPBZ = 11;
static constexpr uint8_t EXPSZ = 12; /* exponent sign after zero */
static constexpr uint8_t EXPDZ = 13; /* exponent digit after zero */
static constexpr uint8_t ERR = 14;

// ͨ�����������ָ��ָ���ѯ״̬�� state MACHINE
static constexpr uint8_t MACHINE[ERR + 1][EX + 1] = {
	//        DP,     ZR,      NZ,      SG,     EX
	// START
	{     LZDP,   LZ,      LD,      MANTS,  ERR },
	// MANTS
	{     LZDP,   LZ,      LD,      ERR,    ERR },
	// LZ
	{     LZDP,   LZ,      LD,      ERR,    EXPBZ },
	// LZDP
	{     ERR,    DZ,      DD,      ERR,    EXPB },
	// LD
	{     DDP,    LD,      LD,      ERR,    EXPB },
	// DZ
	{     ERR,    DZ,      DD,      ERR,    EXPBZ },
	// DD
	{     ERR,    DD,      DD,      ERR,    EXPB },
	// DDP
	{     ERR,    DD,      DD,      ERR,    EXPB },
	// EXPB
	{     ERR,    EXPD,    EXPD,    EXPS,   ERR },
	// EXPS
	{     ERR,    EXPD,    EXPD,    ERR,    ERR },
	// EXPD
	{     ERR,    EXPD,    EXPD,    ERR,    ERR },
	// EXPBZ
	{     ERR,    EXPDZ,   EXPDZ,   EXPSZ,  ERR },
	// EXPSZ
	{     ERR,    EXPDZ,   EXPDZ,   ERR,    ERR },
	// EXPDZ
	{     ERR,    EXPDZ,   EXPDZ,   ERR,    ERR },
	// ERR
	{     ERR,    ERR,     ERR,     ERR,    ERR }
};


/* ע�� exp ֮ǰ����һ��С���㣺1.e+0 or 1.23e-1 */
mpf mpf::str_to_mpf(std::string_view str, uint32_t radix)
{
	if (radix < 2 || radix > 16)
	{
		throw MPL_ERR_INVALID_VAL;
	}

	if (str.empty())
	{
		return mpf();
	}

	/* �� mpi::str_to_mpi һ�������Ǻܴ��ԵĹ��� */
	mpi mant = mpi::create_size(str.length() / (size_t(mpi::get_radix_ratio(radix)) - 1));
	int32_t expsign = 1;
	mpi expvalue = 0;
	int32_t exp = 0, mantsign = 1;

	std::list<mpi_digit> digit_dq;
	uint8_t state = START;
	for (char c : str)
	{
		/*�����������ַ��л�״̬*/
		switch (c)
		{
		case L'-':
		case L'+':
			state = MACHINE[state][SG];
			break;
		case L'.':
			state = MACHINE[state][DP];
			break;
		case L'0':
			state = MACHINE[state][ZR];
			break;
		case L'^':
		case L'e':
			if (c == L'^' || radix == 10)
			{
				state = MACHINE[state][EX];
				break;
			}
			/*����͸*/
			__fallthrough;
		default:
			state = MACHINE[state][NZ];
			break;
		}

		switch (state)
		{
		case MANTS:
			mantsign = c == '-' ? -1 : 1;
			break;
		case EXPSZ:
		case EXPS:
			expsign = c == '-' ? -1 : 1;
			break;
		case EXPDZ:
		case EXPD:
		{
			uint32_t expdigit = c_to_u(c);
			if (expdigit < radix)
			{
				mpi::mul_d(expvalue, radix, expvalue);
				mpi::add_d(expvalue, expdigit, expvalue);
			}
			else
			{
				state = ERR;
			}
		}
		break;
		case LD:
			exp++;
			__fallthrough;
		case DD:
		{
			mpi_digit digit = mpi_digit(c_to_u(c));
			if (digit < radix)
			{
				digit_dq.push_back(digit);
				exp--;
			}
			else
			{
				state = ERR;
			}
		}
		break;
		case DZ:
			exp--;
			break;
		case LZ:
		case LZDP:
		case DDP:
			break;
		}
	}

	if (state == DZ || state == EXPDZ)
	{
		/* ���� 0  */
		return mpf(mpi(), 0);
	}
	else
	{
		expvalue.sign = expsign;
		mpi::add(expvalue, exp, expvalue);
	}

	while (!digit_dq.empty() && digit_dq.front() == 0)
	{
		digit_dq.pop_front();
	}
	while (!digit_dq.empty() && digit_dq.back() == 0)
	{
		digit_dq.pop_back();
		expvalue.incr();
	}

	if ((radix & (radix - 1)) == 0)
	{
		uint32_t w = 0;
		while ((1u << w) < radix)
		{
			++w;
		}
		for (mpi_digit d : digit_dq)
		{
			mant.lsh(w);
			mpi::add_d(mant, d, mant);
		}
	}
	else
	{
		for (mpi_digit d : digit_dq)
		{
			mpi::mul_d(mant, radix, mant);
			mpi::add_d(mant, d, mant);
		}
	}
	mant.sign = mantsign;

	mpf result = mant;
	mpf exponent = mpi(radix);
	expsign = expvalue.sign;
	expvalue.sign = 1;
	mpf::pow(exponent, expvalue, exponent);
	if (expsign == 1)
	{
		mpf::mul(result, exponent, result);
	}
	else
	{
		mpf::div(result, exponent, result);
	}
	return result;
}
MPL_END