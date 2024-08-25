/**
 * This file has no copyright assigned and is placed in the Public Domain.
 * This file is part of the mingw-w64 runtime package.
 * No warranty is given; refer to the file DISCLAIMER.PD within this package.
 */

#include <_mingw_mac.h>
#include <math.h>

float MINGW_CDECL _copysignf(float aX, float aY)
{
  return copysignf(aX, aY);
}

float (MINGW_CDECL *__MINGW_IMP_SYMBOL(_copysignf))(float, float) = _copysignf;
