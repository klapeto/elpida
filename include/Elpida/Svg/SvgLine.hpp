//
// Created by klapeto on 25/3/2024.
//

#ifndef ELPIDA_SVGLINE_HPP
#define ELPIDA_SVGLINE_HPP

#include "Elpida/Svg/SvgLength.hpp"
#include "Elpida/Svg/SvgPathInstance.hpp"
#include <vector>

namespace Elpida
{
	class XmlMap;
	class SvgCalculationContext;

	class SvgLine
	{
	public:
		[[nodiscard]]
		std::vector<SvgPathInstance> CalculatePaths(const SvgCalculationContext& calculationContext) const;

		explicit SvgLine(const XmlMap& properties);
	private:
		SvgLength _x1;
		SvgLength _y1;
		SvgLength _x2;
		SvgLength _y2;
	};

} // Elpida

#endif //ELPIDA_SVGLINE_HPP
