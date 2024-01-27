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

	enum class SvgGradientType
	{
		Linear,
		Radial
	};

	class SvgGradient : public SvgElement
	{
	public:
		union GradientData
		{
			struct
			{
				SvgLength x1;
				SvgLength y1;
				SvgLength x2;
				SvgLength y2;
			} linear;
			struct
			{
				SvgLength cx;
				SvgLength cy;
				SvgLength r;
				SvgLength fx;
				SvgLength fy;
			} radial;
			GradientData(){}
		};

		[[nodiscard]]
		const std::string& GetHref() const
		{
			return _href;
		}

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

		[[nodiscard]]
		SvgGradientType GetType() const
		{
			return _type;
		}

		[[nodiscard]]
		const GradientData& GetData() const
		{
			return _data;
		}

		explicit SvgGradient(const XmlElement& element, SvgDocument& document);

	protected:
		std::string _href;
		std::vector<SvgGradientStop> _stops;
		SvgTransform _gradientTransform;
		SvgSpreadType _spreadType;
		SvgGradientUnits _units;
		SvgGradientType _type;
		GradientData _data;

		void ParseAsRadial();
		void ParseAsLinear();
	};
} // Elpida

#endif //ELPIDA_SVG_SVGGRADIENT_HPP
