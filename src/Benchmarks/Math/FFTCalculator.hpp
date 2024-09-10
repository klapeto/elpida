//
// Created by klapeto on 11/8/2024.
//

#ifndef ELPIDA_FFTCALCULATOR_HPP
#define ELPIDA_FFTCALCULATOR_HPP

#include <valarray>
#include <complex>

#include "ValueArray.hpp"

namespace Elpida
{

	class FFTCalculator
	{
	public:
		static void CalculateFFTInPlace(std::vector<std::complex<double>>& input);
	};

} // Elpida

#endif //ELPIDA_FFTCALCULATOR_HPP
