//
// Created by klapeto on 5/9/2024.
//

long double asinl(long double x)
{
	__asm__("fld %%st(0)\n\t"
			"fld1\n\t"
			"fsub %%st(0),%%st(1)\n\t"
			"fadd %%st(2)\n\t"
			"fmulp\n\t"
			"fsqrt\n\t"
			"fpatan": "+t"(x));

	return x;
}