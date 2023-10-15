#include "mpf.h"
MPL_BEGIN

static int32_t prec = MPF_ORIGINAL_PRECISION;
mpf::init_struct mpf_constant_quarter_init_struct =
{
	-1,
	{
		1,
		1,
		{
			67108864,
		}
	}
};
static const mpf::init_struct mpf_constant_pi_init_struct =
{
	-129,
	{
		1,
		130,
		{
			131615310, 91201460, 250110356, 51284108, 93579300, 24536778, 30644976, 144830821,
			133149353, 171700304, 128171595, 252406803, 54146798, 120105809, 109666554, 47731759,
			111483405, 198412528, 13223450, 158112359, 109326190, 190718406, 78242297, 74041584,
			172507648, 235619458, 256123794, 225440643, 181162230, 243389334, 142751891, 48981275,
			230842928, 159741361, 138794462, 209728239, 180749790, 160516220, 214938138, 56743945,
			107119129, 79685659, 79403772, 256415851, 140997560, 58796181, 96969509, 57418959,
			184380323, 193434398, 96723305, 137492174, 206952257, 204090281, 18233083, 62123540,
			130494105, 22316719, 69111018, 207369233, 17525580, 157952683, 67460259, 40101908,
			91523462, 173386644, 89284410, 6054118, 50177366, 187136175, 203150100, 248976759,
			184674467, 211685003, 60915942, 165458016, 59701479, 152615131, 252486679, 52977797,
			207428354, 183656467, 51205010, 173651356, 172088357, 183401300, 92807508, 141659085,
			120122213, 227898511, 154392544, 150905844, 82221637, 14184791, 23475858, 42307324,
			8396590, 59862263, 169448571, 209688868, 151281426, 244759164, 248947303, 129556911,
			218213883, 268268251, 234576578, 17540760, 263306515, 98142585, 24715629, 95766281,
			66604379, 159142109, 180525948, 151809216, 107803470, 58179156, 241405185, 155527201,
			247785160, 137296536, 166927616, 154673705, 120867392, 170787696, 221327768, 142975752,
			38008488, 3,
		}
	}
};

static mpf quarter = mpf_constant_quarter_init_struct;
static mpf pi = mpf_constant_pi_init_struct;

mpf const& mpf::const_pi()
{
	if (MPF_ORIGINAL_PRECISION <= prec)
	{
		return pi;
	}

	/* quarter 只使用 1 位不用更新 */
	mpf a = 1;
	mpf b = mpf::const_sqrt2();
	mpf::inv(b, b);
	mpf t = quarter;
	mpf p = 1;

	mpf m;
	do
	{
		m = a;
		mpf::add(a, b, a);
		mpf::div_2(a, a);

		mpf::mul(m, b, b);
		mpf::sqrt(b, b);

		mpf::sub(m, a, m);
		mpf::sqr(m, m);
		mpf::mul(p, m, m);
		mpf::sub(t, m, t);

		mpf::mul_2(p, p);
		mpf::sub(a, b, m);
	} while (!m.is_small_enough());

	mpf::add(a, b, a);
	mpf::sqr(a, a);
	mpf::mul_2d(t, 2, t);
	mpf::div(a, t, a);
	pi.swap(a);

	prec = MPF_ORIGINAL_PRECISION;
	return pi;
}
MPL_END