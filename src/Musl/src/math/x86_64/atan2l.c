//
// Created by klapeto on 5/9/2024.
//

long double atan2l(long double y, long double x)
{
	__asm__("fpatan": "+t"(x): "u"(y): "st(1)");

	return x;
}