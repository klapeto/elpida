#include "libm.h"

float _MuslLite_math_divzerof(uint32_t sign)
{
	return fp_barrierf(sign ? -1.0f : 1.0f) / 0.0f;
}
