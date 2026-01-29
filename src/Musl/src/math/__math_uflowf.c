#include "libm.h"

float _MuslLite_math_uflowf(uint32_t sign)
{
	return _MuslLite_math_xflowf(sign, 0x1p-95f);
}
