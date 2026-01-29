#include "libm.h"

double _MuslLite_math_uflow(uint32_t sign)
{
	return _MuslLite_math_xflow(sign, 0x1p-767);
}
