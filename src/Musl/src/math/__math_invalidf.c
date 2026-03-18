#include "libm.h"

float _MuslLite_math_invalidf(float x)
{
	return (x - x) / (x - x);
}
