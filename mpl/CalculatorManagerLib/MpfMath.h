#ifndef __INCLUDE_MPF_MATH_H__
#define __INCLUDE_MPF_MATH_H__
#include "CalculatorManagerLibNS.h"
#include "../mpl/mpf/mpf.h"
#include "AngleType.h"
#include "NumberWidth.h"
CM_BEGIN

mpl::mpf Sin(mpl::mpf const& x, AngleType angle);
mpl::mpf Cos(mpl::mpf const& x, AngleType angle);
mpl::mpf Tan(mpl::mpf const& x, AngleType angle);

// 1/cos(x).
mpl::mpf Sec(mpl::mpf const& x, AngleType angle);

// 1/sin(x).
mpl::mpf Csc(mpl::mpf const& x, AngleType angle);

// 1/tan(x).
mpl::mpf Cot(mpl::mpf const& x, AngleType angle);

mpl::mpf ArcSin(mpl::mpf const& x, AngleType angle);
mpl::mpf ArcCos(mpl::mpf const& x, AngleType angle);
mpl::mpf ArcTan(mpl::mpf const& x, AngleType angle);

// arccos(1/x)
mpl::mpf ArcSec(mpl::mpf const& x, AngleType angle);

// arcsin(1/x)
mpl::mpf ArcCsc(mpl::mpf const& x, AngleType angle);

// arctan(1/x)
mpl::mpf ArcCot(mpl::mpf const& x, AngleType angle);

mpl::mpf SinH(mpl::mpf const& x);
mpl::mpf CosH(mpl::mpf const& x);
mpl::mpf TanH(mpl::mpf const& x);

// 1/CosH
mpl::mpf SecH(mpl::mpf const& x);

// 1/SinH
mpl::mpf CscH(mpl::mpf const& x);

// 1/TanH
mpl::mpf CotH(mpl::mpf const& x);

mpl::mpf ArSinH(mpl::mpf const& x);
mpl::mpf ArCosH(mpl::mpf const& x);
mpl::mpf ArTanH(mpl::mpf const& x);

// ArCosH(1/x)
mpl::mpf ArSecH(mpl::mpf const& x);

// ArSinH(1/x)
mpl::mpf ArCscH(mpl::mpf const& x);

// ArTanH(1/x)
mpl::mpf ArCotH(mpl::mpf const& x);

// logX / logY
mpl::mpf LogXY(mpl::mpf const& Y, mpl::mpf const& X);
mpl::mpf Log2(mpl::mpf const& x);
mpl::mpf Root(mpl::mpf const& base, mpl::mpf const& pow);
mpl::mpf Power(mpl::mpf const& base, mpl::mpf const& pow);
mpl::mpf Power10(mpl::mpf const& power);
mpl::mpf Power2(mpl::mpf const& power);
mpl::mpf Exp(mpl::mpf const& x);
mpl::mpf Cube(mpl::mpf const& x);
mpl::mpf CubeRoot(mpl::mpf const& x);
mpl::mpf Random();

mpl::mpf Ln(mpl::mpf const& x);
mpl::mpf Log(mpl::mpf const& x);
mpl::mpf Invert(mpl::mpf const& x);
mpl::mpf Square(mpl::mpf const& x);
mpl::mpf SquareRoot(mpl::mpf const& x);
mpl::mpf Exp(mpl::mpf const& x);
mpl::mpf Fraction(mpl::mpf const& x);
mpl::mpf Integer(mpl::mpf const& x);
mpl::mpf Mod(mpl::mpf const& x, mpl::mpf const& y);
mpl::mpf Remainder(mpl::mpf const& x, mpl::mpf const& y);

// 向下取整。
mpl::mpf Floor(mpl::mpf const& x);

// 向上取整。
mpl::mpf Ceil(mpl::mpf const& x);

void TrueValueToComplement(mpl::mpf& x, NumberWidth const& number_width);
void ComplementToTrueValue(mpl::mpf& x, NumberWidth const& number_width);

//
// 所有的和二进制有关的操作，例如 not xor lsh
// 再调用相应的函数时，输入的都应该是其真值，例如 -1 ，63，-59
// 而不是其的补码 1111 1111 ，0011 1111 ，1100 0101
//

mpl::mpf Not(mpl::mpf const& x, NumberWidth const& number_width);

mpl::mpf And(mpl::mpf const& x, mpl::mpf const& y, NumberWidth const& number_width);
mpl::mpf Or(mpl::mpf const& x, mpl::mpf const& y, NumberWidth const& number_width);
mpl::mpf Xor(mpl::mpf const& x, mpl::mpf const& y, NumberWidth const& number_width);

// 非于，NOT(x&y)
mpl::mpf Nand(mpl::mpf const& x, mpl::mpf const& y, NumberWidth const& number_width);

// 非或，NOT(x|y)
mpl::mpf Nor(mpl::mpf const& x, mpl::mpf const& y, NumberWidth const& number_width);

// 所有移位运算仅限于整数，这里不进行整数判定。

// 根据移位的特性，仅有算术右移需要特判负数（高位补 1 ），
// 其他的，不论算术运算还是逻辑运算，不论是左移还是右移，都可以视为逻辑移位进行运算。

// 逻辑移位
mpl::mpf RightShiftLogical(mpl::mpf const& x, mpl::mpf const& y, NumberWidth const& word_bit_width);
mpl::mpf LeftShiftLogical(mpl::mpf const& x, mpl::mpf const& y, NumberWidth const& word_bit_width);

// 算数移位
mpl::mpf RightShiftArithmetic(mpl::mpf const& x, mpl::mpf const& y, NumberWidth const& word_bit_width);
mpl::mpf LeftShiftArithmetic(mpl::mpf const& x, mpl::mpf const& y, NumberWidth const& word_bit_width);

// 旋转循环移位
mpl::mpf RotateRightShift(mpl::mpf const& x, NumberWidth const& word_bit_width);
mpl::mpf RotateLeftShift(mpl::mpf const& x, NumberWidth const& word_bit_width);

// 通过循环移位旋转
// 附加的 carry_bit 参数，用于记忆进位位，需要调用者自行维护。
// 一般来说对于一个数它如果第一次进行循环移位旋转 carry_bit 总是 0 。
mpl::mpf RotateLeftShiftCircle(mpl::mpf const& x, NumberWidth const& word_bit_width, uint64_t& carry_bit);
mpl::mpf RotateRightShiftCircle(mpl::mpf const& x, NumberWidth const& word_bit_width, uint64_t& carry_bit);

// 将度数转换成度分秒的形式。
// 32.86 --> 32 51 36 用 mpl::mpf 表示为 32.5136
mpl::mpf DegreeToDegreeMinuteSecond(mpl::mpf const& x);


// 这两个函数均为做好，有更优雅的实现方式。
// 上下两个函数为互逆运算。

// 将度分秒形式的数转化成度数。
// 32 51 36 --> 32.86
mpl::mpf DegreeMinuteSecondToDegree(mpl::mpf const& x);

CM_END

#endif // !__INCLUDE_MPL_MATH_H__