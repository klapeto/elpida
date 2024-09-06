//
// Created by klapeto on 5/9/2024.
//

long double atanl(long double x)
{
	__asm__("fld1\n\t"
			"fpatan": "+t"(x));

	return x;
}