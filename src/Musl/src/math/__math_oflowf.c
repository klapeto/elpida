#include "libm.h"

float _MuslLite_math_oflowf(uint32_t sign)
{
	return _MuslLite_math_xflowf(sign, 0x1p97f);
}
