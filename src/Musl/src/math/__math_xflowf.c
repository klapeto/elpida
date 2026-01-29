#include "libm.h"

float _MuslLite_math_xflowf(uint32_t sign, float y)
{
	return eval_as_float(fp_barrierf(sign ? -y : y) * y);
}
