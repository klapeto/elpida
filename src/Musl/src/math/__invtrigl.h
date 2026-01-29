#include <features.h>

/* shared by acosl, asinl and atan2l */
#define pio2_hi _MuslLite_pio2_hi
#define pio2_lo _MuslLite_pio2_lo
hidden extern const long double pio2_hi, pio2_lo;

hidden long double _MuslLite_invtrigl_R(long double z);
