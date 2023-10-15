#include "mpf.h"
MPL_BEGIN
// EXPLANATION: This uses Taylor series
//
//      n
//     ___    2j
//   n \  ]  A       1          A
//  A   \   -----[ ---- - ---------------]
//      /   (2j)!  n+2j   (n+2j+1)(2j+1)
//     /__]
//     j=0
//
//                        / oo
//                        |    n-1 -x     __
//  This was derived from |   x   e  dx = |
//                        |               | (n) { = (n-1)! for +integers}
//                        / 0
//
//  It can be shown that the above series is within precision if A is chosen
//  big enough.
//                          A    n  precision
//  Based on the relation ne  = A 10            A was chosen as
//
//             precision
//  A = ln(Base         /n)+1
//  A += n*ln(A)  This is close enough for precision > base and n < 1.5

void mpf::gamma_taylor(mpf const & x, mpf & y)
{
	mpf lnbase = MPI_BASE;
	mpf::ln(lnbase, lnbase);
	/* A = precision * ln(base) */
	mpf A = lnbase;
	mpf::mul_d(A, mpi_digit(MPF_ORIGINAL_PRECISION), A);

	/* A = A - ln(x) + 1 */
	/* 但是 gamma_taylor 函数的调用条件是 0.5 < x < 1.5 这时有
	-ln(x) < 1 ，A 的值越大越好，这里我们就认为 -ln(x) = 1
	于是 A - ln(x) + 1 约等于 A + 2 */
	mpf::add(A, 2, A);

	mpf tmp = A;
	mpf::ln(tmp, tmp);
	mpf::mul(tmp, x, tmp);
	mpf::add(A, tmp, A);
	mpf::add(A, 1, A);

	tmp = x;

	/* precision += ln(e^A * A^(n + 1.5)) - ln(base) */
	/* ln(e^A * A^(n + 1.5))
	=  ln(e^A) + ln(A^(n + 1.5))
	=  A + (n + 1.5) * ln(A) */

	/* tmp = x + 1.5 */
	mpf _1_5 = 3;
	mpf::div_2(_1_5, _1_5);
	mpf::add(tmp, _1_5, tmp);

	/* term = A^(x + 1.5) */
	mpf term = A;
	mpf::pow(term, tmp, term);

	/* term = ln(e^A * A^(n + 1.5)) */
	tmp = A;
	mpf::e_expt(tmp, tmp);
	mpf::mul(term, tmp, term);
	mpf::ln(term, term);

	(void)y;
}
MPL_END