#include "mpf.h"
MPL_BEGIN

static int32_t prec = MPF_ORIGINAL_PRECISION;
static const mpf::init_struct mpf_constant_ln10_init_struct =
{
	-129,
	{
		1,
		130,
		{
			265030998, 125750607, 266161133, 112815654, 62074037, 179677403, 36501814, 116410759,
			103246119, 217263820, 85363383, 199414520, 256570369, 138054523, 27686324, 103832058,
			227840880, 109108418, 174870384, 253015989, 104316456, 214945969, 258736858, 266075352,
			240297435, 57084289, 17049640, 198673088, 210669703, 135769432, 163036694, 153742538,
			66273804, 238939990, 146039697, 11426857, 17811481, 13710714, 185080206, 142374105,
			193356121, 26454529, 61929263, 135435465, 138586777, 119735190, 32340268, 146132149,
			249315126, 71952824, 9985542, 125771813, 165882135, 215541258, 159229395, 197027619,
			141465891, 46209461, 141460115, 267386720, 191654746, 201588600, 137945749, 20599013,
			232754648, 229089943, 41593527, 107823781, 33316547, 254517761, 77065158, 148972323,
			220437339, 133493061, 231737231, 18987686, 68692962, 95923356, 117252074, 144313575,
			22910775, 139479465, 50131719, 66519952, 171611637, 88042612, 161866519, 110230316,
			176801158, 56021177, 247178791, 239020724, 174094481, 104483863, 64071593, 257406856,
			113388565, 31617341, 80585553, 21108282, 215402712, 250554782, 41191025, 155551469,
			44888510, 228276929, 101482235, 177934019, 71802262, 42609202, 94243646, 758875,
			249814561, 70224747, 143197712, 204841137, 49285911, 24326595, 99379716, 173587467,
			8129372, 253262344, 125715360, 263448425, 42612899, 145623884, 96114101, 111846064,
			81224567, 2,
		}
	}
};
static mpf ln10 = mpf_constant_ln10_init_struct;

mpf const& mpf::const_ln10()
{
	if (MPF_ORIGINAL_PRECISION <= prec)
	{
		return ln10;
	}
	mpf::ln(10, ln10);
	prec = MPF_ORIGINAL_PRECISION;
	return ln10;
}
MPL_END