#include "libm.h"

double _MuslLite_math_oflow(uint32_t sign)
{
	return _MuslLite_math_xflow(sign, 0x1p769);
}
