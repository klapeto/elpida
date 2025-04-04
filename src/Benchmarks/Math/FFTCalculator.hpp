//
// Copyright (C) 2025. Ioannis Panagiotopoulos
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

//
// Created by klapeto on 11/8/2024.
//

#ifndef ELPIDA_FFTCALCULATOR_HPP
#define ELPIDA_FFTCALCULATOR_HPP

#include <vector>
#include <complex>
#include "Elpida/Core/Float.hpp"

namespace Elpida
{

	class FFTCalculator
	{

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

		static std::size_t Reverse(std::size_t v)
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

	public:
		template<typename T>
		static void CalculateFFTInPlace(std::vector<std::complex<T>>& x)
		{
			static constexpr T Pi = 3.1415926535897932384626433;

			// based on https://introcs.cs.princeton.edu/java/97data/InplaceFFT.java.html
			auto n = x.size();

			using namespace std::complex_literals;
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
					Float jth = 2 * Pi * j / L;
					auto w = std::complex<T>(std::cos(jth), -std::sin(jth));
					for (std::size_t k = 0; k < n / L; k++)
					{
						auto tao = w * (x[k * L + j + L / 2]);
						x[k * L + j + L / 2] = x[k * L + j] - tao;
						x[k * L + j] = x[k * L + j] + tao;
					}

				}
			}
		}
	};

} // Elpida

#endif //ELPIDA_FFTCALCULATOR_HPP
