//
// Created by klapeto on 7/12/2023.
//

#ifndef ELPIDA_SRC_BENCHMARKS_IMAGE_SVG_SVGGRADIENT_HPP
#define ELPIDA_SRC_BENCHMARKS_IMAGE_SVG_SVGGRADIENT_HPP

#include "Benchmarks/Image/Svg/SvgCoordinate.hpp"
#include "SvgGradientStop.hpp"
#include "SvgTransform.hpp"
#include <vector>
#include <string>

namespace Elpida
{

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

		SvgGradient(std::string&& id,
			std::string&& href,
			std::vector<SvgGradientStop>&& stops,
			const SvgTransform& transform,
			SvgSpreadType spreadType,
			SvgGradientUnits units,
			const SvgCoordinate& x1,
			const SvgCoordinate& y1,
			const SvgCoordinate& x2,
			const SvgCoordinate& y2)
			: _id(std::move(id)),
			  _href(std::move(href)),
			  _stops(std::move(stops)),
			  _transform(transform),
			  _spreadType(spreadType),
			  _type(SvgGradientType::Linear),
			  _units(units)
		{
			linear.x1 = x1;
			linear.y1 = y1;
			linear.x2 = x2;
			linear.y2 = y2;
		}

		SvgGradient(std::string&& id,
			std::string&& href,
			std::vector<SvgGradientStop>&& stops,
			const SvgTransform& transform,
			SvgSpreadType spreadType,
			SvgGradientUnits units,
			const SvgCoordinate& cx,
			const SvgCoordinate& cy,
			const SvgCoordinate& r,
			const SvgCoordinate& fx,
			const SvgCoordinate& fy
		)
			: _id(std::move(id)),
			_href(std::move(href)),
			  _stops(std::move(stops)),
			  _transform(transform),
			  _spreadType(spreadType),
			  _type(SvgGradientType::Radial),
			  _units(units)
		{
			radial.cx = cx;
			radial.cy = cy;
			radial.r = r;
			radial.fx = fx;
			radial.fy = fy;
		}
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
	};

} // Elpida

#endif //ELPIDA_SRC_BENCHMARKS_IMAGE_SVG_SVGGRADIENT_HPP
