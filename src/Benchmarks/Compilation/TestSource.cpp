/*
 * musl as a whole is licensed under the following standard MIT license:

----------------------------------------------------------------------
Copyright © 2005-2020 Rich Felker, et al.

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
----------------------------------------------------------------------

Authors/contributors include:

A. Wilcox
Ada Worcester
Alex Dowad
Alex Suykov
Alexander Monakov
Andre McCurdy
Andrew Kelley
Anthony G. Basile
Aric Belsito
Arvid Picciani
Bartosz Brachaczek
Benjamin Peterson
Bobby Bingham
Boris Brezillon
Brent Cook
Chris Spiegel
Clément Vasseur
Daniel Micay
Daniel Sabogal
Daurnimator
David Carlier
David Edelsohn
Denys Vlasenko
Dmitry Ivanov
Dmitry V. Levin
Drew DeVault
Emil Renner Berthing
Fangrui Song
Felix Fietkau
Felix Janda
Gianluca Anzolin
Hauke Mehrtens
He X
Hiltjo Posthuma
Isaac Dunham
Jaydeep Patil
Jens Gustedt
Jeremy Huntwork
Jo-Philipp Wich
Joakim Sindholt
John Spencer
Julien Ramseier
Justin Cormack
Kaarle Ritvanen
Khem Raj
Kylie McClain
Leah Neukirchen
Luca Barbato
Luka Perkov
M Farkas-Dyck (Strake)
Mahesh Bodapati
Markus Wichmann
Masanori Ogino
Michael Clark
Michael Forney
Mikhail Kremnyov
Natanael Copa
Nicholas J. Kain
orc
Pascal Cuoq
Patrick Oppenlander
Petr Hosek
Petr Skocik
Pierre Carrier
Reini Urban
Rich Felker
Richard Pennington
Ryan Fairfax
Samuel Holland
Segev Finer
Shiz
sin
Solar Designer
Stefan Kristiansson
Stefan O'Rear
Szabolcs Nagy
Timo Teräs
Trutz Behn
Valentin Ochs
Will Dietz
William Haddon
William Pitcock

Portions of this software are derived from third-party works licensed
under terms compatible with the above MIT license:

The TRE regular expression implementation (src/regex/reg* and
src/regex/tre*) is Copyright © 2001-2008 Ville Laurikari and licensed
under a 2-clause BSD license (license text in the source files). The
included version has been heavily modified by Rich Felker in 2012, in
the interests of size, simplicity, and namespace cleanliness.

Much of the math library code (src/math and src/complex) is
Copyright © 1993,2004 Sun Microsystems or
Copyright © 2003-2011 David Schultz or
Copyright © 2003-2009 Steven G. Kargl or
Copyright © 2003-2009 Bruce D. Evans or
Copyright © 2008 Stephen L. Moshier or
Copyright © 2017-2018 Arm Limited
and labelled as such in comments in the individual source files. All
have been licensed under extremely permissive terms.

The ARM memcpy code (src/string/arm/memcpy.S) is Copyright © 2008
The Android Open Source Project and is licensed under a two-clause BSD
license. It was taken from Bionic libc, used on Android.

The AArch64 memcpy and memset code (src/string/aarch64) are
Copyright © 1999-2019, Arm Limited.

The implementation of DES for crypt (src/crypt/crypt_des.c) is
Copyright © 1994 David Burren. It is licensed under a BSD license.

The implementation of blowfish crypt (src/crypt/crypt_blowfish.c) was
originally written by Solar Designer and placed into the public
domain. The code also comes with a fallback permissive license for use
in jurisdictions that may not recognize the public domain.

The smoothsort implementation (src/stdlib/qsort.c) is Copyright © 2011
Valentin Ochs and is licensed under an MIT-style license.

The x86_64 port was written by Nicholas J. Kain and is licensed under
the standard MIT terms.

The mips and microblaze ports were originally written by Richard
Pennington for use in the ellcc project. The original code was adapted
by Rich Felker for build system and code conventions during upstream
integration. It is licensed under the standard MIT terms.

The mips64 port was contributed by Imagination Technologies and is
licensed under the standard MIT terms.

The powerpc port was also originally written by Richard Pennington,
and later supplemented and integrated by John Spencer. It is licensed
under the standard MIT terms.

All other files which have no copyright comments are original works
produced specifically for use as part of this library, written either
by Rich Felker, the main author of the library, or by one or more
contibutors listed above. Details on authorship of individual files
can be found in the git version control history of the project. The
omission of copyright and license comments in each file is in the
interest of source tree size.

In addition, permission is hereby granted for all public header files
(include and arch/bits) and crt files intended to be linked into
applications (crt, ldso/dlstart.c, and arch/crt_arch.h) to omit
the copyright notice and permission notice otherwise required by the
		license, and to use these files without any requirement of
		attribution. These files include substantial contributions from:

Bobby Bingham
John Spencer
Nicholas J. Kain
		Rich Felker
		Richard Pennington
		Stefan Kristiansson
		Szabolcs Nagy

		all of whom have explicitly granted such permission.

This file previously contained text expressing a belief that most of
		the files covered by the above exception were sufficiently trivial not
to be subject to copyright, resulting in confusion over whether it
negated the permissions granted in the license. In the spirit of
permissive licensing, and of not having licensing issues being an
		obstacle to adoption, that text has been removed.
*/

//
// Created by klapeto on 22/12/2024.
//

using Size = unsigned long long int;
using uint32_t = unsigned int;
using uint64_t = unsigned long long int;
using uint16_t = unsigned short;

#define FP_NAN       0
#define FP_INFINITE  1
#define FP_ZERO      2
#define FP_SUBNORMAL 3
#define FP_NORMAL    4

#define __BYTE_ORDER 1234
#define __LITTLE_ENDIAN 1234
#define __BIG_ENDIAN 4321

union ldshape {
	long double f;
	struct {
		uint64_t m;
		uint16_t se;
	} i;
};

union AsDoubleS
{
	uint64_t _i;
	double _f;
};
union AsUint64S
{
	double _f;
	uint64_t _i;
};
#define asuint(f) ((union{float _f; uint32_t _i;}){f})._i
#define asfloat(i) ((union{uint32_t _i; float _f;}){i})._f
#define asuint64(f) (AsUint64S{f})._i
#define asdouble(i) (AsDoubleS{i})._f

#define predict_true(x) (x)
#define predict_false(x) (x)

#define FENV_SUPPORT 1

void *memcpy(void *  dest, const void * src, Size n)
{
	unsigned char *d = (unsigned char*)dest;
	const unsigned char *s = (unsigned char*)src;

	for (; n; n--) *d++ = *s++;
	return dest;
}

static const uint16_t __rsqrt_tab[128] = {
		0xb451, 0xb2f0, 0xb196, 0xb044, 0xaef9, 0xadb6, 0xac79, 0xab43,
		0xaa14, 0xa8eb, 0xa7c8, 0xa6aa, 0xa592, 0xa480, 0xa373, 0xa26b,
		0xa168, 0xa06a, 0x9f70, 0x9e7b, 0x9d8a, 0x9c9d, 0x9bb5, 0x9ad1,
		0x99f0, 0x9913, 0x983a, 0x9765, 0x9693, 0x95c4, 0x94f8, 0x9430,
		0x936b, 0x92a9, 0x91ea, 0x912e, 0x9075, 0x8fbe, 0x8f0a, 0x8e59,
		0x8daa, 0x8cfe, 0x8c54, 0x8bac, 0x8b07, 0x8a64, 0x89c4, 0x8925,
		0x8889, 0x87ee, 0x8756, 0x86c0, 0x862b, 0x8599, 0x8508, 0x8479,
		0x83ec, 0x8361, 0x82d8, 0x8250, 0x81c9, 0x8145, 0x80c2, 0x8040,
		0xff02, 0xfd0e, 0xfb25, 0xf947, 0xf773, 0xf5aa, 0xf3ea, 0xf234,
		0xf087, 0xeee3, 0xed47, 0xebb3, 0xea27, 0xe8a3, 0xe727, 0xe5b2,
		0xe443, 0xe2dc, 0xe17a, 0xe020, 0xdecb, 0xdd7d, 0xdc34, 0xdaf1,
		0xd9b3, 0xd87b, 0xd748, 0xd61a, 0xd4f1, 0xd3cd, 0xd2ad, 0xd192,
		0xd07b, 0xcf69, 0xce5b, 0xcd51, 0xcc4a, 0xcb48, 0xca4a, 0xc94f,
		0xc858, 0xc764, 0xc674, 0xc587, 0xc49d, 0xc3b7, 0xc2d4, 0xc1f4,
		0xc116, 0xc03c, 0xbf65, 0xbe90, 0xbdbe, 0xbcef, 0xbc23, 0xbb59,
		0xba91, 0xb9cc, 0xb90a, 0xb84a, 0xb78c, 0xb6d0, 0xb617, 0xb560,
};

static double __math_invalid(double x)
{
	return (x - x) / (x - x);
}

/* returns a*b*2^-32 - e, with error 0 <= e < 1.  */
static inline uint32_t mul32(uint32_t a, uint32_t b)
{
	return (uint64_t)a * b >> 32;
}

/* returns a*b*2^-64 - e, with error 0 <= e < 3.  */
static inline uint64_t mul64(uint64_t a, uint64_t b)
{
	uint64_t ahi = a >> 32;
	uint64_t alo = a & 0xffffffff;
	uint64_t bhi = b >> 32;
	uint64_t blo = b & 0xffffffff;
	return ahi * bhi + (ahi * blo >> 32) + (alo * bhi >> 32);
}

static inline double eval_as_double(double x)
{
	double y = x;
	return y;
}

double sqrt(double x)
{
	uint64_t ix, top, m;

	/* special case handling.  */
	ix = asuint64(x);
	top = ix >> 52;
	if (predict_false(top - 0x001 >= 0x7ff - 0x001))
	{
		/* x < 0x1p-1022 or inf or nan.  */
		if (ix * 2 == 0)
			return x;
		if (ix == 0x7ff0000000000000)
			return x;
		if (ix > 0x7ff0000000000000)
			return __math_invalid(x);
		/* x is subnormal, normalize it.  */
		ix = asuint64(x * 0x1p52);
		top = ix >> 52;
		top -= 52;
	}

	/* argument reduction:
	   x = 4^e m; with integer e, and m in [1, 4)
	   m: fixed point representation [2.62]
	   2^e is the exponent part of the result.  */
	int even = top & 1;
	m = (ix << 11) | 0x8000000000000000;
	if (even) m >>= 1;
	top = (top + 0x3ff) >> 1;

	/* approximate r ~ 1/sqrt(m) and s ~ sqrt(m) when m in [1,4)

	   initial estimate:
	   7bit table lookup (1bit exponent and 6bit significand).

	   iterative approximation:
	   using 2 goldschmidt iterations with 32bit int arithmetics
	   and a final iteration with 64bit int arithmetics.

	   details:

	   the relative error (e = r0 sqrt(m)-1) of a linear estimate
	   (r0 = a m + b) is |e| < 0.085955 ~ 0x1.6p-4 at best,
	   a table lookup is faster and needs one less iteration
	   6 bit lookup table (128b) gives |e| < 0x1.f9p-8
	   7 bit lookup table (256b) gives |e| < 0x1.fdp-9
	   for single and double prec 6bit is enough but for quad
	   prec 7bit is needed (or modified iterations). to avoid
	   one more iteration >=13bit table would be needed (16k).

	   a newton-raphson iteration for r is
	     w = r*r
	     u = 3 - m*w
	     r = r*u/2
	   can use a goldschmidt iteration for s at the end or
	     s = m*r

	   first goldschmidt iteration is
	     s = m*r
	     u = 3 - s*r
	     r = r*u/2
	     s = s*u/2
	   next goldschmidt iteration is
	     u = 3 - s*r
	     r = r*u/2
	     s = s*u/2
	   and at the end r is not computed only s.

	   they use the same amount of operations and converge at the
	   same quadratic rate, i.e. if
	     r1 sqrt(m) - 1 = e, then
	     r2 sqrt(m) - 1 = -3/2 e^2 - 1/2 e^3
	   the advantage of goldschmidt is that the mul for s and r
	   are independent (computed in parallel), however it is not
	   "self synchronizing": it only uses the input m in the
	   first iteration so rounding errors accumulate. at the end
	   or when switching to larger precision arithmetics rounding
	   errors dominate so the first iteration should be used.

	   the fixed point representations are
	     m: 2.30 r: 0.32, s: 2.30, d: 2.30, u: 2.30, three: 2.30
	   and after switching to 64 bit
	     m: 2.62 r: 0.64, s: 2.62, d: 2.62, u: 2.62, three: 2.62  */

	static const uint64_t three = 0xc0000000;
	uint64_t r, s, d, u, i;

	i = (ix >> 46) % 128;
	r = (uint32_t)__rsqrt_tab[i] << 16;
	/* |r sqrt(m) - 1| < 0x1.fdp-9 */
	s = mul32(m >> 32, r);
	/* |s/sqrt(m) - 1| < 0x1.fdp-9 */
	d = mul32(s, r);
	u = three - d;
	r = mul32(r, u) << 1;
	/* |r sqrt(m) - 1| < 0x1.7bp-16 */
	s = mul32(s, u) << 1;
	/* |s/sqrt(m) - 1| < 0x1.7bp-16 */
	d = mul32(s, r);
	u = three - d;
	r = mul32(r, u) << 1;
	/* |r sqrt(m) - 1| < 0x1.3704p-29 (measured worst-case) */
	r = r << 32;
	s = mul64(m, r);
	d = mul64(s, r);
	u = (three << 32) - d;
	s = mul64(s, u);  /* repr: 3.61 */
	/* -0x1p-57 < s - sqrt(m) < 0x1.8001p-61 */
	s = (s - 2) >> 9; /* repr: 12.52 */
	/* -0x1.09p-52 < s - sqrt(m) < -0x1.fffcp-63 */

	/* s < sqrt(m) < s + 0x1.09p-52,
	   compute nearest rounded result:
	   the nearest result to 52 bits is either s or s+0x1p-52,
	   we can decide by comparing (2^52 s + 0.5)^2 to 2^104 m.  */
	uint64_t d0, d1, d2;
	double y, t;
	d0 = (m << 42) - s * s;
	d1 = s - d0;
	d2 = d1 + s + 1;
	s += d1 >> 63;
	s &= 0x000fffffffffffff;
	s |= top << 52;
	y = asdouble(s);
	if (FENV_SUPPORT)
	{
		/* handle rounding modes and inexact exception:
		   only (s+1)^2 == 2^42 m case is exact otherwise
		   add a tiny value to cause the fenv effects.  */
		uint64_t tiny = predict_false(d2 == 0) ? 0 : 0x0010000000000000;
		tiny |= (d1 ^ d2) & 0x8000000000000000;
		t = asdouble(tiny);
		y = eval_as_double(y + t);
	}
	return y;
}

double abs(double x)
{
	union {double f; uint64_t i;} u = {x};
	u.i &= -1ULL/2;
	return u.f;
}

static __inline unsigned __FLOAT_BITS(float __f)
{
	union {float __f; unsigned __i;} __u;
	__u.__f = __f;
	return __u.__i;
}

static __inline unsigned long long __DOUBLE_BITS(double __f)
{
	union {double __f; unsigned long long __i;} __u;
	__u.__f = __f;
	return __u.__i;
}

#define isnan(x) ( \
	sizeof(x) == sizeof(float) ? (__FLOAT_BITS(x) & 0x7fffffff) > 0x7f800000 : \
	sizeof(x) == sizeof(double) ? (__DOUBLE_BITS(x) & -1ULL>>1) > 0x7ffULL<<52 : \
	__fpclassifyl(x) == FP_NAN)

int __fpclassifyl(long double x)
{
	union ldshape u = {x};
	int e = u.i.se & 0x7fff;
	int msb = u.i.m>>63;
	if (!e && !msb)
		return u.i.m ? FP_SUBNORMAL : FP_ZERO;
	if (e == 0x7fff) {
		/* The x86 variant of 80-bit extended precision only admits
		 * one representation of each infinity, with the mantissa msb
		 * necessarily set. The version with it clear is invalid/nan.
		 * The m68k variant, however, allows either, and tooling uses
		 * the version with it clear. */
		if (__BYTE_ORDER == __LITTLE_ENDIAN && !msb)
			return FP_NAN;
		return u.i.m << 1 ? FP_NAN : FP_INFINITE;
	}
	if (!msb)
		return FP_NAN;
	return FP_NORMAL;
}

double fmod(double x, double y)
{
	union {double f; uint64_t i;} ux = {x}, uy = {y};
	int ex = ux.i>>52 & 0x7ff;
	int ey = uy.i>>52 & 0x7ff;
	int sx = ux.i>>63;
	uint64_t i;

	/* in the followings uxi should be ux.i, but then gcc wrongly adds */
	/* float load/store to inner loops ruining performance and code size */
	uint64_t uxi = ux.i;

	if (uy.i<<1 == 0 || isnan(y) || ex == 0x7ff)
		return (x*y)/(x*y);
	if (uxi<<1 <= uy.i<<1) {
		if (uxi<<1 == uy.i<<1)
			return 0*x;
		return x;
	}

	/* normalize x and y */
	if (!ex) {
		for (i = uxi<<12; i>>63 == 0; ex--, i <<= 1);
		uxi <<= -ex + 1;
	} else {
		uxi &= -1ULL >> 12;
		uxi |= 1ULL << 52;
	}
	if (!ey) {
		for (i = uy.i<<12; i>>63 == 0; ey--, i <<= 1);
		uy.i <<= -ey + 1;
	} else {
		uy.i &= -1ULL >> 12;
		uy.i |= 1ULL << 52;
	}

	/* x mod y */
	for (; ex > ey; ex--) {
		i = uxi - uy.i;
		if (i >> 63 == 0) {
			if (i == 0)
				return 0*x;
			uxi = i;
		}
		uxi <<= 1;
	}
	i = uxi - uy.i;
	if (i >> 63 == 0) {
		if (i == 0)
			return 0*x;
		uxi = i;
	}
	for (; uxi>>52 == 0; uxi <<= 1, ex--);

	/* scale result */
	if (ex > 0) {
		uxi -= 1ULL << 52;
		uxi |= (uint64_t)ex << 52;
	} else {
		uxi >>= -ex + 1;
	}
	uxi |= (uint64_t)sx << 63;
	ux.i = uxi;
	return ux.f;
}


template<typename T, Size R, Size C>
class Matrix
{
	static_assert(R > 0);
	static_assert(C > 0);
public:

	[[nodiscard]]
	constexpr Size GetRows() const
	{
		return R;
	}

	[[nodiscard]]
	constexpr Size  GetColumns() const
	{
		return C;
	}

	T operator[](Size i) const
	{
		return _values[i];
	}

	Matrix<T, R, C> CalculateInverse() const
	{
		auto det = CalculateDeterminant();
		if (det < 1e-8) return Identity();

		auto adJoint = CalculateCofactors().CalculateTranspose();
		return adJoint * (1.0 / det);
	}

	void CalculateInverse(Matrix<T, R, C>& out) const
	{
		auto det = CalculateDeterminant();
		if (det < 1e-8)
		{
			out = Identity();
			return;
		}

		Matrix<T, R, C> tmp;
		CalculateCofactors(tmp);
		tmp.CalculateTranspose(out);
		out *= (1.0 / det);
	}

	Matrix<T, R, C> CalculateMinor() const
	{
		Matrix<T, R, C> out;
		for (Size i = 0; i < R; ++i)
		{
			for (Size j = 0; j < C; ++j)
			{
				out.Get(i, j) = CalculateRemovedRowColumn(i, j).CalculateDeterminant();
			}
		}

		return out;
	}

	void CalculateMinor(Matrix<T, R, C>& out) const
	{
		Matrix<T, R - 1, C - 1> tmp;
		for (Size i = 0; i < R; ++i)
		{
			for (Size j = 0; j < C; ++j)
			{
				CalculateRemovedRowColumn(i, j, tmp);
				out.Get(i, j) = tmp.CalculateDeterminant();
			}
		}
	}

	Matrix<T, R, C> CalculateCofactors() const
	{
		auto minor = CalculateMinor();

		for (Size i = 0, s = 1; i < R; ++i, ++s)
		{
			for (Size j = s % 2; j < C; j += 2)
			{
				minor.Get(i, j) = -minor.Get(i, j);
			}
		}
		return minor;
	}

	void CalculateCofactors(Matrix<T, R, C>& out) const
	{
		CalculateMinor(out);

		for (Size i = 0, s = 1; i < R; ++i, ++s)
		{
			for (Size j = s % 2; j < C; j += 2)
			{
				out.Get(i, j) = -out.Get(i, j);
			}
		}
	}

	Matrix<T, R, C> CalculateTranspose() const
	{
		static_assert(R == C);
		Matrix<T, C, R> out;

		for (Size i = 0; i < R; ++i)
		{
			for (Size j = i; j < C; ++j)
			{
				out.Get(i, j) = Get(j, i);
				out.Get(j, i) = Get(i, j);
			}
		}

		return out;
	}

	void CalculateTranspose(Matrix<T, R, C>& out) const
	{
		static_assert(R == C);

		for (Size i = 0; i < R; ++i)
		{
			for (Size j = i; j < C; ++j)
			{
				out.Get(i, j) = Get(j, i);
				out.Get(j, i) = Get(i, j);
			}
		}
	}

	void CalculateRemovedRowColumn(Size row, Size column, Matrix<T, R - 1, C - 1>& out) const
	{
		for (Size i = 0, io = 0; i < R; ++i)
		{
			if (i == row) continue;
			for (Size j = 0, jo = 0; j < C; ++j)
			{
				if (j == column) continue;

				out.Get(io, jo) = Get(i, j);

				jo++;
			}
			io++;
		}
	}

	Matrix<T, R - 1, C - 1> CalculateRemovedRowColumn(Size row, Size column) const
	{
		Matrix<T, R - 1, C - 1> out;
		CalculateRemovedRowColumn(row, column, out);
		return out;
	}

	T CalculateDeterminant() const
	{
		static_assert(R == C);

		if constexpr (R > 2)
		{
			Matrix<T, R - 1, C - 1> tmp;
			T det = 0.0;
			T sign = 1.0;
			for (Size i = 0; i < C; ++i)
			{
				CalculateRemovedRowColumn(0, i, tmp);

				det += sign * Get(0, i) * tmp.CalculateDeterminant();

				sign = -sign;
			}

			return det;
		}
		else
		{
			return (_values[0] * _values[3]) - (_values[1] * _values[2]);
		}
	}

	Matrix<T, R, C> operator-(T value) const
	{
		auto copy = *this;
		for (Size i = 0; i < R * C; ++i)
		{
			copy._values[i] -= value;
		}
		return copy;
	}

	Matrix<T, R, C>& operator-=(T value) const
	{
		for (Size i = 0; i < R * C; ++i)
		{
			_values[i] -= value;
		}
		return *this;
	}

	Matrix<T, R, C> operator+(T value) const
	{
		auto copy = *this;
		for (Size i = 0; i < R * C; ++i)
		{
			copy._values[i] += value;
		}
		return copy;
	}

	Matrix<T, R, C>& operator+=(T value) const
	{
		for (Size i = 0; i < R * C; ++i)
		{
			_values[i] += value;
		}
		return *this;
	}

	Matrix<T, R, C>& operator*=(T value)
	{
		for (Size i = 0; i < R * C; ++i)
		{
			_values[i] *= value;
		}
		return *this;
	}

	Matrix<T, R, C> operator*(T value) const
	{
		auto copy = *this;
		for (Size i = 0; i < R * C; ++i)
		{
			copy._values[i] *= value;
		}
		return copy;
	}

	template<Size Oc>
	Matrix<T, R, Oc> operator*(const Matrix<T, C, Oc>& other) const
	{
		Matrix<T, R, Oc> returnMatrix;
		for (Size i = 0; i < R; ++i)
		{
			for (Size j = 0; j < Oc; ++j)
			{
				T product = 0.0;
				for (Size k = 0; k < C; ++k)
				{
					product += Get(i, k) * other.Get(k, j);
				}
				returnMatrix.Get(i, j) = product;
			}
		}

		return returnMatrix;
	}

	[[nodiscard]]
	double Get(Size r, Size c) const
	{
		return _values[r * C + c];
	}

	double& Get(Size r, Size c)
	{
		return _values[r * C + c];
	}

	constexpr Matrix()
			:_values{ 0.0 }
	{

	}

	static constexpr Matrix<T, R, C> Identity()
	{
		Matrix<T, R, C> ret;

		for (Size i = 0; i < R * C; i += C + 1)
		{
			ret._values[i] = 1.0;
		}

		return ret;
	}

	template<class ... TValue>
	static constexpr Matrix<T, R, C> WithOnes(const TValue... values)
	{
		Matrix<T, R, C> ret{ values... };

		auto argumentCount = sizeof...(values);
		if (argumentCount >= R * C)
		{
			return ret;
		}
		for (Size i = argumentCount; i < (R * C) - argumentCount; ++i)
		{
			ret._values[i] = 1.0;
		}

		return ret;
	}

	template<class ... TValue>
	constexpr explicit Matrix(const TValue... values)
			:_values{ values... }
	{
	}

	Matrix(const Matrix<T, R, C>&) = default;
	Matrix& operator=(const Matrix<T, R, C>&) = default;
	Matrix(Matrix<T, R, C>&&) noexcept = default;
	Matrix& operator=(Matrix<T, R, C>&&) noexcept = default;
	~Matrix() = default;
private:
	T _values[R * C];

	friend class Matrix<T, C, R>;
};

template<typename T>
class Color
{
public:

	Color<T> operator*(const T v) const
	{
		return {
				_r * v,
				_g * v,
				_b * v,
		};
	}

	Color<T>& operator*=(const T& v)
	{
		_r *= v;
		_g *= v;
		_b *= v;
		return *this;
	}

	Color<T>& operator*=(const Color<T>& other)
	{
		_r *= other._r;
		_g *= other._g;
		_b *= other._b;
		return *this;
	}

	Color<T>& operator+=(const Color<T>& other)
	{
		_r += other._r;
		_g += other._g;
		_b += other._b;
		return *this;
	}

	Color<T> operator+(const Color<T>& other) const
	{
		return {
				_r + other._r,
				_g + other._g,
				_b + other._b,
		};
	}

	Color<T> operator*(const Color<T>& other) const
	{
		return {
				_r * other._r,
				_g * other._g,
				_b * other._b,
		};
	}

	[[nodiscard]]
	constexpr T R() const
	{
		return _r;
	}

	[[nodiscard]]
	T& R()
	{
		return _r;
	}

	[[nodiscard]]
	constexpr T G() const
	{
		return _g;
	}

	[[nodiscard]]
	T& G()
	{
		return _g;
	}

	[[nodiscard]]
	constexpr T B() const
	{
		return _b;
	}

	[[nodiscard]]
	T& B()
	{
		return _b;
	}

	[[nodiscard]]
	constexpr T A() const
	{
		return _a;
	}

	[[nodiscard]]
	T& A()
	{
		return _a;
	}

	constexpr Color()
			:_r(0.0), _g(0.0), _b(0.0), _a(1.0)
	{

	}

	constexpr Color(const T r, const T g, const T b)
			:_r(r), _g(g), _b(b), _a(1.0)
	{
	}

	constexpr Color(const T r, const T g, const T b, const T a)
			:_r(r), _g(g), _b(b), _a(a)
	{
	}

	Color(const Color<T>&) = default;
	Color& operator=(const Color<T>&) = default;
	Color(Color<T>&&) noexcept = default;
	Color& operator=(Color<T>&&) noexcept = default;
	~Color() = default;
private:
	T _r;
	T _g;
	T _b;
	T _a;
};


template<typename T>
class Vector3
{
public:

	constexpr T operator[](const Size i) const
	{
		return ((T*)&_x)[i];
	}

	constexpr T X() const
	{
		return _x;
	}

	constexpr T Y() const
	{
		return _y;
	}

	constexpr T Z() const
	{
		return _z;
	}

	template<Size R, Size C>
	Vector3<T>& Transform(const Matrix<T, R, C>& matrix)
	{
		auto m = Matrix<T, R, 1>::WithOnes(_x, _y, _z);
		auto multiple = matrix * m;
		_x = multiple[0];
		_y = multiple[1];
		_z = multiple[2];
		return *this;
	}

	constexpr T LengthSquared() const
	{
		return (_x * _x) + (_y * _y) + (_z * _z);
	}

	constexpr T Length() const
	{
		return sqrt(LengthSquared());
	}

	Vector3<T>& operator+=(const Vector3<T>& other)
	{
		_x += other._x;
		_y += other._y;
		_z += other._z;
		return *this;
	}

	Vector3<T>& operator-=(const Vector3<T>& other)
	{
		_x -= other._x;
		_y -= other._y;
		_z -= other._z;
		return *this;
	}

	Vector3<T>& operator*=(T scale)
	{
		_x *= scale;
		_y *= scale;
		_z *= scale;
		return *this;
	}

	Vector3<T> operator+(const Vector3<T>& other) const
	{
		return {
				_x + other._x,
				_y + other._y,
				_z + other._z,
		};
	}

	Vector3<T> operator-(const Vector3<T>& other) const
	{
		return {
				_x - other._x,
				_y - other._y,
				_z - other._z,
		};
	}

	T DotProduct(const Vector3<T>& other) const
	{
		return (_x * other._x) + (_y * other._y) + (_z * other._z);
	}

	Vector3<T> operator*(const Vector3<T>& other) const
	{
		return {
				_x * other._x,
				_y * other._y,
				_z * other._z,
		};
	}

	Vector3<T> Cross(const Vector3<T>& other) const
	{
		return {
				(_y * other._z) - (_z * other._y),
				(_z * other._x) - (_x * other._z),
				(_x * other._y) - (_y * other._x)
		};
	}

	Vector3<T> operator*(T scale) const
	{
		return {
				_x * scale,
				_y * scale,
				_z * scale,
		};
	}

	Vector3<T>& Normalize()
	{
		auto length = Length();

		_x /= length;
		_y /= length;
		_z /= length;
		return *this;
	}

	constexpr Vector3<T> Normalized() const
	{
		auto length = Length();

		return {
				_x / length,
				_y / length,
				_z / length
		};
	}

	constexpr Vector3()
			:_x(0.0), _y(0.0), _z(0.0)
	{
	}

	constexpr Vector3(T x, T y, T z)
			:_x(x), _y(y), _z(z)
	{
	}

	Vector3(const Vector3<T>&) = default;
	Vector3& operator=(const Vector3<T>&) = default;
	Vector3(Vector3<T>&&) noexcept = default;
	Vector3& operator=(Vector3<T>&&) noexcept = default;
	~Vector3() = default;
protected:
	T _x;
	T _y;
	T _z;
};

template<typename T>
class ShaderColor: public Color<T>
{
public:

	[[nodiscard]]
	T GetReflection() const
	{
		return _reflection;
	}

	ShaderColor()
			:Color<T>(), _reflection(0.0)
	{

	}

	ShaderColor(const T r, const T g, const T b, const T a)
			:Color<T>(r, g, b, a), _reflection(0.0)
	{
	}

	ShaderColor(const T r, const T g, const T b, const T a, T reflection)
			:Color<T>(r, g, b, a), _reflection(reflection)
	{
	}

private:
	T _reflection;
};

template<typename T>
using Shader = ShaderColor<T> (*)(const Vector3<T>& position);

template<typename T>
class Light
{
public:

	const Vector3<T>& GetLocation() const
	{
		return _location;
	}

	const Color<T>& GetColor() const
	{
		return _color;
	}

	Light() = default;

	Light(const Vector3<T>& location, const Color<T>& color)
			:_location(location), _color(color)
	{
	}

private:
	Vector3<T> _location;
	Color<T> _color;
};

template<typename T>
class Triangle
{
public:

	const Vector3<T>& GetNormal() const
	{
		return _normal;
	}

	T IntersectsRay(const Vector3<T>& origin, const Vector3<T>& direction, T near = 0.001,
			T far = 1e+308) const
	{
		auto u = (_axis + 1) % 3;
		auto v = (_axis + 2) % 3;
		auto d = direction[_axis] + _nu * direction[u] + _nv * direction[v];
		auto t = (_nd - origin[_axis] - _nu * origin[u] - _nv * origin[v]) / d;
		if (t < near || t > far)
		{
			return -1;
		}
		auto Pu = origin[u] + t * direction[u] - _eu;
		auto Pv = origin[v] + t * direction[v] - _ev;
		auto a2 = Pv * _nu1 + Pu * _nv1;
		if (a2 < 0)
		{
			return -1;
		}
		auto a3 = Pu * _nu2 + Pv * _nv2;
		if (a3 < 0)
		{
			return -1;
		}

		if ((a2 + a3) > 1)
		{
			return -1;
		}
		return t;
	}

	Shader<T> GetShader() const
	{
		return _shader;
	}

	Triangle()
			:_axis(0), _nu(0.0), _nv(0.0), _nd(0.0), _eu(0.0), _nu1(0.0), _nv1(0.0), _nu2(0.0), _nv2(0.0)
	{

	}

	Triangle(const Vector3<T>& a, const Vector3<T>& b, const Vector3<T>& c, Shader<T> shader = [](auto p){ return ShaderColor<T>(); })
			:_a(a), _b(b), _c(c), _shader(shader), _axis(0), _nu(0.0), _nv(0.0), _nd(0.0), _eu(0.0), _nu1(0.0),
			 _nv1(0.0), _nu2(0.0), _nv2(0.0)
	{
		Calculate();
	}

	Triangle(const Triangle<T>&) = default;
	Triangle& operator=(const Triangle<T>&) = default;
	Triangle(Triangle<T>&&) noexcept = default;
	Triangle& operator=(Triangle<T>&&) noexcept = default;
	~Triangle() = default;
private:
	Vector3<T> _a;
	Vector3<T> _b;
	Vector3<T> _c;
	Vector3<T> _normal;
	Shader<T> _shader;

	Size _axis;
	T _nu;
	T _nv;
	T _nd;
	T _eu;
	T _ev;
	T _nu1;
	T _nv1;
	T _nu2;
	T _nv2;

	void Calculate()
	{
		auto edge1 = _c - _a;
		auto edge2 = _b - _a;
		_normal = edge1.Cross(edge2).Normalize();
		auto normal = edge1.Cross(edge2);

		if (abs(normal.X()) > abs(normal.Y()))
		{
			if (abs(normal.X()) > abs(normal.Z()))
			{
				_axis = 0;
			}
			else
			{
				_axis = 2;
			}
		}
		else
		{
			if (abs(normal.Y()) > abs(normal.Z()))
			{
				_axis = 1;
			}
			else
			{
				_axis = 2;
			}
		}

		auto u = (_axis + 1) % 3;
		auto v = (_axis + 2) % 3;

		auto u1 = edge1[u];
		auto v1 = edge1[v];

		auto u2 = edge2[u];
		auto v2 = edge2[v];

		_normal = normal.Normalized();

		_nu = normal[u] / normal[_axis];
		_nv = normal[v] / normal[_axis];
		_nd = normal.DotProduct(_a) / normal[_axis];
		auto det = u1 * v2 - v1 * u2;
		_eu = _a[u];
		_ev = _a[v];
		_nu1 = u1 / det;
		_nv1 = -v1 / det;
		_nu2 = v2 / det;
		_nv2 = -u2 / det;
	}
};

template<typename T, Size TS, Size LS>
class Scene
{
public:

	Color<T> Intersect(const Vector3<T>& origin, const Vector3<T>& direction, T near = 0.001,
			T far = 1e+308) const
	{
		const Triangle<T>* closest = nullptr;

		for (auto& triangle : _triangles)
		{
			auto d = triangle.IntersectsRay(origin, direction, near, far);
			if (d < 0.0 || d > far || d < near) continue;
			far = d;
			closest = &triangle;
		}

		if (closest == nullptr) return _background;

		auto normal = closest->GetNormal();
		auto hit = origin + (direction * far);

		if (direction.DotProduct(normal) > 0.0)
		{
			normal = { -normal.X(), -normal.Y(), -normal.Z() };
		}

		auto color = closest->GetShader()(hit);

		Color<T> reflected;
		bool isReflected = false;
		if (color.GetReflection() > 0.001)
		{
			auto reflection = (normal * (-2.0 * direction.DotProduct(normal))) + direction;
			reflected = Intersect(hit, reflection, 0.0001, 1000000);
			isReflected = true;
			if (color.GetReflection() >= 0.999999)
			{
				return reflected;
			}
		}

		auto l = _ambient;
		for (auto& light : _lights)
		{
			auto toLight = light.GetLocation() - hit;
			auto distance = toLight.Length();
			toLight *= 1.0 / distance;
			distance -= 0.0001;

			if (IsBlocked(hit, toLight, distance)) continue;

			auto nl = normal.DotProduct(toLight);
			if (nl > 0.0)
			{
				l += light.GetColor() * nl;
			}
		}
		l = l * color;
		if (isReflected)
		{
			l *= 1.0 - color.GetReflection();
			l = l + (reflected * color.GetReflection());
		}

		return l;
	}

	void SetAmbient(const Color<T>& color)
	{
		_ambient = color;
	}

//	template<typename ... R>
//	void SetLights(R&& ... lights)
//	{
//		_lights = {lights...};
//	}
//
//	template<typename ... R>
//	void SetTriangles(R&& ... triangles)
//	{
////		for (Size i = 0; i < sizeof...(R); ++i)
////		{
////			_triangles[i] = triangles...[i];
////		}
//		_triangles = { triangles... };
//	}

//	Scene()
//			:_background(0.8, 0.8, 1.0)
//	{
//
//	}

	Scene(Triangle<T> triangles[TS], Light<T> lights[LS])
			: _background(0.8, 0.8, 1.0)
	{
		memcpy(_triangles, triangles, sizeof(Triangle<T>) * TS);
		memcpy(_lights, lights, sizeof(Light<T>) * LS);
	}


private:
	Triangle<T> _triangles[TS];
	Light<T> _lights[LS];
	Color<T> _ambient;
	Color<T> _background;

	bool IsBlocked(const Vector3<T>& origin, const Vector3<T>& direction, T far) const
	{
		T near = 0.0001;
		const Triangle<T>* closest = nullptr;
		for (auto& triangle : _triangles)
		{
			auto d = triangle.IntersectsRay(origin, direction, near, far);
			if (d < 0.0 || d > far || d < near) continue;
			return true;
		}

		return false;
	}
};

class SceneCreator
{
public:
	template<typename T>
	static Scene<T, 14, 3> GenerateScene()
	{
		auto tfl = Vector3<T>(-10, 10, -10);
		auto tfr = Vector3<T>(10, 10, -10);
		auto tbl = Vector3<T>(-10, 10, 10);
		auto tbr = Vector3<T>(10, 10, 10);
		auto bfl = Vector3<T>(-10, -10, -10);
		auto bfr = Vector3<T>(10, -10, -10);
		auto bbl = Vector3<T>(-10, -10, 10);
		auto bbr = Vector3<T>(10, -10, 10);

		auto cubeShader = [](auto p)
		{ return ShaderColor<T>(0.7, 0.7, 0.7, 1.0, 0.0); };

		auto ffl = Vector3<T>(-1000.0, -30.0, -1000.0);
		auto ffr = Vector3<T>(1000.0, -30.0, -1000.0);
		auto fbl = Vector3<T>(-1000.0, -30.0, 1000.0);
		auto fbr = Vector3<T>(1000.0, -30.0, 1000.0);

		// floor

		auto floorShader = [](auto p)
		{
			T x = fmod(p.X() / 32.0, 2.0);
			x+=2;
			x = fmod(x, 2.0);

			T z = p.Z() / 32.0 + 0.3;
			z = fmod(z, 2.0);
			z+=2;
			z = fmod(z, 2.0);

			if (x < 1.0 != z < 1.0)
			{
				return ShaderColor<T>{ 0.4, 0.4, 0.4, 1.0, 1.0 };
			}
			else
			{
				return ShaderColor<T>{ 0.0, 0.4, 0.0, 1.0 };
			}
		};

		Triangle<T> triangles[] = {
				//front
				Triangle<T>(tfl, tfr, bfr, cubeShader),
				Triangle<T>(tfl, bfr, bfl, cubeShader),

				//back
				Triangle<T>(tbl, tbr, bbr, cubeShader),
				Triangle<T>(tbl, bbr, bbl, cubeShader),

				//left
				Triangle<T>(tbl, tfl, bbl, cubeShader),
				Triangle<T>(tfl, bfl, bbl, cubeShader),

				//right
				Triangle<T>(tbr, tfr, bbr, cubeShader),
				Triangle<T>(tfr, bfr, bbr, cubeShader),

				//top
				Triangle<T>(tbl, tbr, tfr, cubeShader),
				Triangle<T>(tbl, tfr, tfl, cubeShader),

				//bottom
				Triangle<T>(bbl, bbr, bfr, cubeShader),
				Triangle<T>(bbl, bfr, bfl, cubeShader),

				Triangle<T>(fbl, fbr, ffr, floorShader),
				Triangle<T>(fbl, ffr, ffl, floorShader)
		};

		Light<T> lights[] = {
				Light<T>(
						{ 20.0, 38.0, -22.0 },
						{ 0.7, 0.3, 0.3 }
				),
				Light<T>(
						{ -23.0, 40.0, 17.0 },
						{ 0.7, 0.3, 0.3 }
				),
				Light<T>(
						{ 23.0, 20.0, 17.0 },
						{ 0.7, 0.7, 0.7 }
				)
		};
		// cube
		Scene<T, 14, 3> scene = Scene<T, 14, 3>(triangles, lights);
		scene.SetAmbient({ 0.1, 0.1, 0.1 });

		return scene;
	}
};

template<typename T>
class Ray
{
public:
	const Vector3<T>& GetOrigin() const
	{
		return _origin;
	}

	const Vector3<T>& GetDirection() const
	{
		return _direction;
	}


	Ray() = default;

	Ray(const Vector3<T>& origin, const Vector3<T>& direction)
			:_origin(origin), _direction(direction)
	{
	}

	Ray(const Ray<T>&) = default;
	Ray& operator=(const Ray<T>&) = default;
	Ray(Ray<T>&&) noexcept = default;
	Ray& operator=(Ray<T>&&) noexcept = default;
	~Ray() = default;
private:
	Vector3<T> _origin;
	Vector3<T> _direction;
};

template<typename T, Size W, Size H>
class FrameBuffer
{
public:

	[[nodiscard]]
	Size GetHeight() const
	{
		return H;
	}

	[[nodiscard]]
	Size GetWidth() const
	{
		return W;
	}

	const Color<T>& Get(Size i) const
	{
		return _pixels[i];
	}

	const Color<T>& Get(Size x, Size y) const
	{
		return _pixels[y * W + x];
	}

	Color<T>& Get(Size x, Size y)
	{
		return _pixels[y * W + x];
	}

	FrameBuffer() = default;

	FrameBuffer(const FrameBuffer<T, W, H>&) = default;
	FrameBuffer& operator=(const FrameBuffer<T, W, H>&) = default;

	FrameBuffer(FrameBuffer<T, W, H>&& other) noexcept
	{
		_pixels = other._pixels;
	}

	FrameBuffer& operator=(FrameBuffer<T, W, H>&& other) noexcept
	{
		_pixels = other._pixels;

		return *this;
	}

	~FrameBuffer() = default;
private:
	Color<T> _pixels[W * H];
};

template<typename T>
class Camera
{
public:
	template<Size TS, Size LS, Size W, Size H>
	void Render(const Scene<T, TS, LS>& scene, FrameBuffer<T, W, H>& frameBuffer) const
	{
		auto height = frameBuffer.GetHeight();
		auto width = frameBuffer.GetWidth();

		Ray<T> a, b;
		for (Size y = 0; y < height; ++y)
		{
			GenerateRayPair(y / T(height), a, b);
			for (Size x = 0; x < width; ++x)
			{
				auto xp = x / T(width);
				auto origin = (a.GetOrigin() * xp) + (b.GetOrigin() * (1.0 - xp));
				auto direction = ((a.GetDirection() * xp) + (b.GetDirection() * (1.0 - xp))).Normalize();
				auto l = scene.Intersect(origin, direction);
				frameBuffer.Get(x, y) = l;
			}
		}
	}

	Camera() = default;
	Camera(const Vector3<T>& origin, const Vector3<T>& lookAt, const Vector3<T>& up)
			:_origin(origin)
	{
		auto zAxis = (lookAt - origin).Normalize();
		auto xAxis = up.Cross(zAxis).Normalize();
		auto yAxis = xAxis.Cross(Vector3<T>() - zAxis).Normalize();

		Matrix<T, 4, 4> matrix(
				xAxis.X(), xAxis.Y(), xAxis.Z(), 0.0,
				yAxis.X(), yAxis.Y(), yAxis.Z(), 0.0,
				zAxis.X(), zAxis.Y(), zAxis.Z(), 0.0,
				0.0, 0.0, 0.0, 1.0
		);

		matrix = matrix.CalculateInverse();
		matrix.Get(0, 3) = 0.0;
		matrix.Get(1, 3) = 0.0;
		matrix.Get(2, 3) = 0.0;

		_directions[0] = Vector3<T>(-0.7, 0.7, 1.0)
				.Normalize()
				.Transform(matrix);

		_directions[1] = Vector3<T>(0.7, 0.7, 1.0)
				.Normalize()
				.Transform(matrix);

		_directions[2] = Vector3<T>(0.7, -0.7, 1.0)
				.Normalize()
				.Transform(matrix);

		_directions[3] = Vector3<T>(-0.7, -0.7, 1)
				.Normalize()
				.Transform(matrix);
	}

	Camera(const Camera<T>&) = default;
	Camera& operator=(const Camera<T>&) = default;
	Camera(Camera<T>&&) noexcept = default;
	Camera& operator=(Camera<T>&&) noexcept = default;
	~Camera() = default;
private:
	Vector3<T> _origin;
	Vector3<T> _directions[4];

	void GenerateRayPair(T y, Ray<T>& a, Ray<T>& b) const
	{
		a = { _origin, (_directions[0] * y) + (_directions[3] * (1.0 - y)) };
		b = { _origin, (_directions[1] * y) + (_directions[2] * (1.0 - y)) };
	}
};

extern "C" void Test3(FrameBuffer<double, 1024, 1024>& frameBuffer)
{
	auto scene = SceneCreator::GenerateScene<double>();
	auto camera = Camera<double>(
			{ -40.0, 40.0, 40.0 },
			{ 0.0, 0.0, 0.0 },
			{ 0.0, 1.0, 0.0 });
	camera.Render(scene, frameBuffer);
}
