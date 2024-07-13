//
// Created by klapeto on 24/3/2024.
//

#ifndef ELPIDA_SVGCIRCLE_HPP
#define ELPIDA_SVGCIRCLE_HPP

#include <vector>

#include "SvgLength.hpp"
#include "SvgPathInstance.hpp"

namespace Elpida
{
	class XmlMap;
	class SvgCalculationContext;

	class SvgCircle
	{
	public:
		[[nodiscard]]
		std::vector<SvgPathInstance> CalculatePaths(const SvgCalculationContext& calculationContext) const;

		explicit SvgCircle(const XmlMap& properties);

	private:
		SvgLength _cx;
		SvgLength _cy;
		SvgLength _r;
	};
} // Elpida

#endif //ELPIDA_SVGCIRCLE_HPP
