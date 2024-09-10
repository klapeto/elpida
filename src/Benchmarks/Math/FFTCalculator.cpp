//
// Created by klapeto on 11/8/2024.
//

#include "FFTCalculator.hpp"

namespace Elpida
{
	static const double Pi = 3.1415926535897932384626433;

	using namespace std::complex_literals;

	static int LeadingZeros(std::size_t x)
	{
		int c = 0;
		std::size_t n = (std::size_t)1 << (sizeof(std::size_t) * 8 - 1);
		while ((x & n) == 0)
		{
			n >>= 1;
			c++;
		}
		return c;
	}

	std::size_t Reverse(std::size_t v)
	{
		// from https://graphics.stanford.edu/~seander/bithacks.html#BitReverseObvious
		std::size_t r = v; // r will be reversed bits of v; first get LSB of v
		int s = sizeof(v) * 8 - 1; // extra shift needed at end

		for (v >>= 1; v; v >>= 1)
		{
			r <<= 1;
			r |= v & 1;
			s--;
		}
		r <<= s;

		return r;
	}

	void FFTCalculator::CalculateFFTInPlace(std::vector<std::complex<double>>& x)
	{
		// based on https://introcs.cs.princeton.edu/java/97data/InplaceFFT.java.html
		auto n = x.size();

		// bit reversal permutation
		auto shift = 1 + LeadingZeros(n);
		for (std::size_t k = 0; k < n; k++)
		{
			auto j = Reverse(k) >> shift;
			if (j > k)
			{
				auto temp = x[j];
				x[j] = x[k];
				x[k] = temp;
			}
		}

		// butterfly updates
		for (std::size_t L = 2; L <= n; L = L + L)
		{
			for (std::size_t j = 0; j < L / 2; j++)
			{
				double jth = 2 * Pi * j / L;
				auto w = std::complex<double>(std::cos(jth), -std::sin(jth));
				for (std::size_t k = 0; k < n / L; k++)
				{
					auto tao = w * (x[k * L + j + L / 2]);
					x[k * L + j + L / 2] = x[k * L + j] - tao;
					x[k * L + j] = x[k * L + j] + tao;
				}

			}
		}
	}
} // Elpida