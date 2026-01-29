#include <math.h>
#include "libm.h"

double lgamma(double x)
{
	return _MuslLite_lgamma_r(x, &_MuslLite_signgam);
}
