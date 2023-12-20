//
// Created by klapeto on 7/12/2023.
//

#ifndef ELPIDA_SVG_SVGGRADIENT_HPP
#define ELPIDA_SVG_SVGGRADIENT_HPP

#include "Elpida/Svg/SvgCoordinate.hpp"
#include "Elpida/Svg/SvgGradientStop.hpp"
#include "Elpida/Svg/SvgTransform.hpp"

#include <string>
#include <vector>


namespace Elpida
{
	class XmlElement;

	enum class SvgSpreadType
	{
		Pad = 0,
		Reflect = 1,
		Repeat = 2
	};

	enum class SvgGradientType
	{
		Linear,
		Radial
	};

	enum class SvgGradientUnits
	{
		Object,
		User
	};

	class SvgGradient
	{
	public:

		const std::string& GetId() const
		{
			return _id;
		}
		const std::vector<SvgGradientStop>& GetStops() const
		{
			return _stops;
		}
		const SvgTransform& GetTransform() const
		{
			return _transform;
		}
		SvgSpreadType GetSpreadType() const
		{
			return _spreadType;
		}
		SvgGradientType GetType() const
		{
			return _type;
		}
		SvgGradientUnits GetUnits() const
		{
			return _units;
		}

		explicit SvgGradient(const XmlElement& element);
	private:
		std::string _id;
		std::string _href;
		std::vector<SvgGradientStop> _stops;
		union
		{
			struct
			{
				SvgCoordinate x1;
				SvgCoordinate y1;
				SvgCoordinate x2;
				SvgCoordinate y2;
			} linear;
			struct
			{
				SvgCoordinate cx;
				SvgCoordinate cy;
				SvgCoordinate r;
				SvgCoordinate fx;
				SvgCoordinate fy;
			} radial;
		};
		SvgTransform _transform;
		SvgSpreadType _spreadType;
		SvgGradientType _type;
		SvgGradientUnits _units;

		void ParseLinear(const XmlElement& element);
		void ParseRadial(const XmlElement& element);
		void ParseBasicData(const XmlElement& element);
	};

} // Elpida

#endif //ELPIDA_SVG_SVGGRADIENT_HPP
