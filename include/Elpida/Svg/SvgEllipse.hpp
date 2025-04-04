//
// Created by klapeto on 24/3/2024.
//

#ifndef ELPIDA_SVGELLIPSE_HPP
#define ELPIDA_SVGELLIPSE_HPP

#include "Elpida/Svg/SvgLength.hpp"
#include "Elpida/Svg/SvgPathInstance.hpp"
#include <vector>

namespace Elpida
{
	class XmlMap;
	class SvgCalculationContext;

	class SvgEllipse
	{
	public:
		[[nodiscard]]
		std::vector<SvgPathInstance> CalculatePaths(const SvgCalculationContext& calculationContext) const;

		explicit SvgEllipse(const XmlMap& properties);

	private:
		SvgLength _cx;
		SvgLength _cy;
		SvgLength _rx;
		SvgLength _ry;
	};

} // Elpida

#endif //ELPIDA_SVGELLIPSE_HPP
