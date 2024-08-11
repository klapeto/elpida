//
// Created by klapeto on 11/8/2024.
//

#include "FFTCalculator.hpp"

namespace Elpida
{
	static const double Pi = 3.1415926535897932384626433;

	using namespace std::complex_literals;

	void FFTCalculator::CalculateFFTInPlace(std::valarray<std::complex<double>>& input)
	{
		const auto n = input.size();

		if (n <= 1) return;

		std::valarray<std::complex<double>> fftEven = input[std::slice(0, n / 2, 2)];
		std::valarray<std::complex<double>> fftOdd = input[std::slice(1, n / 2, 2)];

		CalculateFFTInPlace(fftEven);
		CalculateFFTInPlace(fftOdd);

		for (std::size_t k = 0; k < n / 2; ++k)
		{
			auto w = (-2i * Pi * std::complex<double>(k / n)) * fftOdd[k];
			input[k] = fftEven[k] + w;
			input[k + n / 2] = fftEven[k] - w;
		}
	}
} // Elpida