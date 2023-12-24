//
// Created by klapeto on 7/12/2023.
//

#ifndef ELPIDA_SVG_SVGGRADIENT_HPP
#define ELPIDA_SVG_SVGGRADIENT_HPP

#include "Elpida/Svg/SvgGradientStop.hpp"
#include "Elpida/Svg/SvgTransform.hpp"

#include <string>
#include <vector>

namespace Elpida
{
	enum class SvgSpreadType
	{
		Pad = 0,
		Reflect = 1,
		Repeat = 2
	};

	enum class SvgGradientUnits
	{
		Object,
		User
	};

	class SvgGradient : public SvgElement
	{
	public:
		const std::vector<SvgGradientStop>& GetStops() const
		{
			return _stops;
		}

		const SvgTransform& GetGradientTransform() const
		{
			return _gradientTransform;
		}

		SvgSpreadType GetSpreadType() const
		{
			return _spreadType;
		}

		SvgGradientUnits GetUnits() const
		{
			return _units;
		}

		explicit SvgGradient(const XmlElement& element, SvgDocument& document);

	protected:
		std::string _href;
		std::vector<SvgGradientStop> _stops;
		SvgTransform _gradientTransform;
		SvgSpreadType _spreadType;
		SvgGradientUnits _units;
	};
} // Elpida

#endif //ELPIDA_SVG_SVGGRADIENT_HPP
