#include <math.h>
#include "libm.h"

float lgammaf(float x)
{
	return _MuslLite_lgammaf_r(x, &_MuslLite_signgam);
}
