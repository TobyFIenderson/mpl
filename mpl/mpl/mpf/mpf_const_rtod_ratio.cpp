#include "mpf.h"
MPL_BEGIN
static int32_t prec = MPF_ORIGINAL_PRECISION;
static const mpf::init_struct mpf_constant_rtod_ratio_init_struct =
{
    -129,
    {
        1,
        130,
        {
            31041062, 99918760, 239194742, 238043726, 15153122, 240024194, 210248939, 8883788,
            104367372, 126501084, 172014865, 260430665, 44217107, 205399487, 145539489, 169925923,
            266223936, 37920326, 62952402, 58139735, 129637216, 209292836, 205638952, 199446138,
            184542092, 124914510, 182932027, 159504648, 240209504, 256404391, 95232108, 245562730,
            156568861, 72740300, 55826083, 183292939, 198931899, 190145641, 202994191, 242604454,
            180953465, 93519861, 20436239, 42342882, 110962658, 200935940, 84705810, 26784260,
            38702930, 185520761, 171212179, 128107943, 12475951, 228330231, 213662284, 30066124,
            93588230, 175044841, 181810815, 263203752, 54536759, 133923264, 169707554, 139615807,
            96110461, 248240134, 165878153, 101886447, 18534700, 150089103, 100309299, 17740414,
            98268079, 262111665, 177564864, 58753836, 208912866, 219888251, 186564385, 222323878,
            192908854, 216367347, 1164213, 184970682, 37185437, 175286536, 256652861, 238886648,
            24849230, 106931269, 8021663, 174130849, 240465736, 172551894, 263389050, 61716563,
            233514145, 224488049, 42552639, 173870181, 45079162, 93984396, 255909751, 155018607,
            211386956, 109680416, 69599216, 89143339, 103736276, 228288914, 195170294, 40752662,
            198995843, 145017885, 139100516, 224276822, 132413242, 46282170, 129299906, 174247616,
            81802821, 144509548, 202076982, 144170799, 257741322, 265303189, 204102989, 126087024,
            79397708, 57,
        }
    }
};
static mpf rtod_ratio = mpf_constant_rtod_ratio_init_struct;
mpf const& mpf::const_rtod_ratio()
{
	if (MPF_ORIGINAL_PRECISION <= prec)
	{
		return rtod_ratio;
	}
	mpf::div(180, mpf::const_pi(), rtod_ratio);
	prec = MPF_ORIGINAL_PRECISION;
	return rtod_ratio;
}

MPL_END