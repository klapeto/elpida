//
// Created by klapeto on 8/3/2024.
//

#ifndef ELPIDA_SVGRECTANGLE_HPP
#define ELPIDA_SVGRECTANGLE_HPP

#include "SvgLength.hpp"
#include "SvgPathInstance.hpp"

#include <optional>

namespace Elpida
{
	class XmlMap;
	class SvgCalculationContext;

	class SvgRectangle
	{
	public:
		[[nodiscard]]
		std::vector<SvgPathInstance> CalculatePaths(const SvgCalculationContext& calculationContext) const;

		explicit SvgRectangle(const XmlMap& properties);
	private:
		SvgLength _x;
		SvgLength _y;
		SvgLength _width;
		SvgLength _height;
		std::optional<SvgLength> _rX;
		std::optional<SvgLength> _rY;
	};

} // Elpida

#endif //ELPIDA_SVGRECTANGLE_HPP
