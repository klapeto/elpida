#include "libm.h"

double _MuslLite_math_invalid(double x)
{
	return (x - x) / (x - x);
}
